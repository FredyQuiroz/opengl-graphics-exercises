#include "C3DsLoader.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "debug.h"

#ifndef	MIN
#define	MIN(a,b) ((a)<(b)?(a):(b))
#define	MAX(a,b) ((a)>(b)?(a):(b))
#endif

C3DsLoader::C3DsLoader(const char *filepath)
{
	camera=NULL;
	gl_width= gl_height=0;
	view_rotx=0;
	view_roty=0;
	view_rotz=0;
	x=z=0;
	view_camera_zoom=-50;

	m_fRotacionHelice=0;
	m_fRotacionLlantas=0;
	m_fVuelo=0;
	m_fVelocidad=0;
	m_dwInicioAceleracion=0;	
	m_bAcelerando=false;
	m_fDistacia=0;

	decompose_datapath(filepath);

	file=lib3ds_file_load(filepath);
  if (!file) {
    puts("3dsplayer: Error: Loading 3DS file failed.\n");
    exit(1);
  }

  /* No nodes?  Fabricate nodes to display all the meshes. */
  if( !file->nodes )
  {
    Lib3dsMesh *mesh;
    Lib3dsNode *node;

    for(mesh = file->meshes; mesh != NULL; mesh = mesh->next)
    {
      node = lib3ds_node_new_object();
      strcpy(node->name, mesh->name);
      node->parent_id = LIB3DS_NO_PARENT;
      lib3ds_file_insert_node(file, node);
    }
  }

  lib3ds_file_eval(file, 1.0f);
  lib3ds_file_bounding_box_of_nodes(file, LIB3DS_TRUE, LIB3DS_FALSE, LIB3DS_FALSE, bmin, bmax);
  sx = bmax[0] - bmin[0];
  sy = bmax[1] - bmin[1];
  sz = bmax[2] - bmin[2];
  size = MAX(sx, sy); size = MAX(size, sz);
  cx = (bmin[0] + bmax[0])/2;
  cy = (bmin[1] + bmax[1])/2;
  cz = (bmin[2] + bmax[2])/2;


  /* No cameras in the file?  Add four */

  if( !file->cameras ) {

    /* Add some cameras that encompass the bounding box */

    Lib3dsCamera *camera = lib3ds_camera_new("Camera_X");
    camera->target[0] = cx;
    camera->target[1] = cy;
    camera->target[2] = cz;
    memcpy(camera->position, camera->target, sizeof(camera->position));
    camera->position[0] = bmax[0] + 1.5 * MAX(sy,sz);
    camera->near_range = ( camera->position[0] - bmax[0] ) * .5;
    camera->far_range = ( camera->position[0] - bmin[0] ) * 2;
    lib3ds_file_insert_camera(file, camera);

    /* Since lib3ds considers +Y to be into the screen, we'll put
    * this camera on the -Y axis, looking in the +Y direction.
    */
    camera = lib3ds_camera_new("Camera_Y");
    camera->target[0] = cx;
    camera->target[1] = cy;
    camera->target[2] = cz;
    memcpy(camera->position, camera->target, sizeof(camera->position));
    camera->position[1] = bmin[1] - 1.5 * MAX(sx,sz);
    camera->near_range = ( bmin[1] - camera->position[1] ) * .5;
    camera->far_range = ( bmax[1] - camera->position[1] ) * 2;
    lib3ds_file_insert_camera(file, camera);

    camera = lib3ds_camera_new("Camera_Z");
    camera->target[0] = cx;
    camera->target[1] = cy;
    camera->target[2] = cz;
    memcpy(camera->position, camera->target, sizeof(camera->position));
    camera->position[2] = bmax[2] + 1.5 * MAX(sx,sy);
    camera->near_range = ( camera->position[2] - bmax[2] ) * .5;
    camera->far_range = ( camera->position[2] - bmin[2] ) * 2;
    lib3ds_file_insert_camera(file, camera);

    camera = lib3ds_camera_new("Camera_ISO");
    camera->target[0] = cx;
    camera->target[1] = cy;
    camera->target[2] = cz;
    memcpy(camera->position, camera->target, sizeof(camera->position));
    camera->position[0] = bmax[0] + .75 * size;
    camera->position[1] = bmin[1] - .75 * size;
    camera->position[2] = bmax[2] + .75 * size;
    camera->near_range = ( camera->position[0] - bmax[0] ) * .5;
    camera->far_range = ( camera->position[0] - bmin[0] ) * 3;
    lib3ds_file_insert_camera(file, camera);
  }

  /* No lights in the file?  Add some. */

  if (file->lights == NULL)
  {
    Lib3dsLight *light;

    light = lib3ds_light_new("light0");
    light->spot_light = 0;
    light->see_cone = 0;
    light->color[0] = light->color[1] = light->color[2] = .6;
    light->position[0] = cx + size * .75;
    light->position[1] = cy - size * 1.;
    light->position[2] = cz + size * 1.5;
    light->position[3] = 0.;
    light->outer_range = 100;
    light->inner_range = 10;
    light->multiplier = 1;
    lib3ds_file_insert_light(file, light);

    light = lib3ds_light_new("light1");
    light->spot_light = 0;
    light->see_cone = 0;
    light->color[0] = light->color[1] = light->color[2] = .3;
    light->position[0] = cx - size;
    light->position[1] = cy - size;
    light->position[2] = cz + size * .75;
    light->position[3] = 0.;
    light->outer_range = 100;
    light->inner_range = 10;
    light->multiplier = 1;
    lib3ds_file_insert_light(file, light);

    light = lib3ds_light_new("light2");
    light->spot_light = 0;
    light->see_cone = 0;
    light->color[0] = light->color[1] = light->color[2] = .3;
    light->position[0] = cx;
    light->position[1] = cy + size;
    light->position[2] = cz + size;
    light->position[3] = 0.;
    light->outer_range = 100;
    light->inner_range = 10;
    light->multiplier = 1;
    lib3ds_file_insert_light(file, light);

  }

  if (!file->cameras) {
    fputs("3dsplayer: Error: No Camera found.\n", stderr);
    lib3ds_file_free(file);
    file=0;
    exit(1);
  }
  if (!camera) {
    camera=file->cameras->name;
  }

  lib3ds_file_eval(file,0.);

  inicio= GetTickCount();
}

C3DsLoader::~C3DsLoader(void)
{
}

void C3DsLoader::SetDimensions(int h, int w) {

	gl_width= w;
	gl_height= h;
}

void C3DsLoader::render_node(Lib3dsNode *node)
{
  ASSERT(file);

  {
    Lib3dsNode *p;
    for (p=node->childs; p!=0; p=p->next) {
      render_node(p);
    }
  }
  if (node->type==LIB3DS_OBJECT_NODE) {
    Lib3dsMesh *mesh;

    if (strcmp(node->name,"$$$DUMMY")==0) {
      return;
    }

    mesh = lib3ds_file_mesh_by_name(file, node->data.object.morph);
    if( mesh == NULL )
      mesh = lib3ds_file_mesh_by_name(file, node->name);

    if (!mesh->user.d) {
      ASSERT(mesh);
      if (!mesh) {
        return;
      }

      mesh->user.d=glGenLists(1);
      glNewList(mesh->user.d, GL_COMPILE);

      {
        unsigned p;
        Lib3dsVector *normalL=(Lib3dsVector *)malloc(3*sizeof(Lib3dsVector)*mesh->faces);
        Lib3dsMaterial *oldmat = (Lib3dsMaterial *)-1;
        {
          Lib3dsMatrix M;
          lib3ds_matrix_copy(M, mesh->matrix);
          lib3ds_matrix_inv(M);
          glMultMatrixf(&M[0][0]);
        }
        lib3ds_mesh_calculate_normals(mesh, normalL);

        for (p=0; p<mesh->faces; ++p) {

          Lib3dsFace *f=&mesh->faceL[p];
          Lib3dsMaterial *mat=0;

          if (f->material[0]) {
            mat=lib3ds_file_material_by_name(file, f->material);
          }

          if( mat != oldmat ) {
            if (mat) {
              if( mat->two_sided )
                glDisable(GL_CULL_FACE);
              else
                glEnable(GL_CULL_FACE);

              glDisable(GL_CULL_FACE);

              /* Texturing added by Gernot < gz@lysator.liu.se > */

              if (mat->texture1_map.name[0]) {		/* texture map? */
                Lib3dsTextureMap *tex = &mat->texture1_map;
                if (!tex->user.p) {		/* no player texture yet? */
                  char texname[1024];
                  pt = (Player_texture *)malloc(sizeof(*pt));
                  tex->user.p = pt;
                  //snprintf(texname, sizeof(texname), "%s/%s", datapath, tex->name);
                  strcpy(texname, datapath);
                  strcat(texname, "/");
                  strcat(texname, tex->name);
#ifdef	DEBUG
                  printf("Loading texture map, name %s\n", texname);
#endif	/* DEBUG */
                  pt->bitmap = NULL;
                  fputs("3dsplayer: Warning: No image loading support, skipping texture.\n", stderr);

				  if (pt->bitmap) {	/* could image be loaded ? */
                    /* this OpenGL texupload code is incomplete format-wise!
                    * to make it complete, examine SDL_surface->format and
                    * tell us @lib3ds.sf.net about your improvements :-)
                    */
                    int upload_format = GL_RED; /* safe choice, shows errors */
                    pt->valid = 1;
                  }
                  else {
                    fprintf(stderr,
                      "Load of texture %s did not succeed "
                      "(format not supported !)\n",
                      texname);
                    pt->valid = 0;
                  }
                }
                else {
                  pt = (Player_texture *)tex->user.p;
                }
                tex_mode = pt->valid;
              }
              else {
                tex_mode = 0;
              }
              glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
              glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
              glMaterialf(GL_FRONT, GL_SHININESS, pow(2, 10.0*mat->shininess));
            }
            else {
              static const Lib3dsRgba a={0.7, 0.7, 0.7, 1.0};
              static const Lib3dsRgba d={0.7, 0.7, 0.7, 1.0};
              static const Lib3dsRgba s={1.0, 1.0, 1.0, 1.0};
              glMaterialfv(GL_FRONT, GL_AMBIENT, a);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
              glMaterialfv(GL_FRONT, GL_SPECULAR, s);
              glMaterialf(GL_FRONT, GL_SHININESS, pow(2, 10.0*0.5));
            }
            oldmat = mat;
          }

          else if (mat != NULL && mat->texture1_map.name[0]) {
            Lib3dsTextureMap *tex = &mat->texture1_map;
            if (tex != NULL && tex->user.p != NULL) {
              pt = (Player_texture *)tex->user.p;
              tex_mode = pt->valid;
            }
          }


          {
            int i;

            if (tex_mode) {
              //printf("Binding texture %d\n", pt->tex_id);
              glEnable(GL_TEXTURE_2D);
              glBindTexture(GL_TEXTURE_2D, pt->tex_id);
            }

            glBegin(GL_TRIANGLES);

            glNormal3fv(f->normal);
            for (i=0; i<3; ++i) {
              glNormal3fv(normalL[3*p+i]);

              if (tex_mode) {
                glTexCoord2f(mesh->texelL[f->points[i]][1]*pt->scale_x,
                  pt->scale_y - mesh->texelL[f->points[i]][0]*pt->scale_y);
              }

              glVertex3fv(mesh->pointL[f->points[i]].pos);
            }
            glEnd();

            if (tex_mode)
              glDisable(GL_TEXTURE_2D);
          }
        }

        free(normalL);
      }

      glEndList();
    }

    if (mesh->user.d) {
      Lib3dsObjectData *d;

      glPushMatrix();
      d=&node->data.object;
      glMultMatrixf(&node->matrix[0][0]);

		// alerond, aleroni, aleront
		// llantad, llantai, llantat
		if(strcmp(node->name, "helice")==0) {

			// glLoadIdentity();
			m_fRotacionHelice= ((int)m_fRotacionHelice)%360;
			glRotatef(m_fRotacionHelice,0,1,0);
			if(m_fRotacionHelice<=120) {
				float temp= m_fRotacionHelice*400/120;
				glTranslatef(-d->pivot[0], -d->pivot[1]+m_fDistacia, -d->pivot[2]-temp+m_fVuelo);
			} else if(m_fRotacionHelice<=240) {
				float tempx=106 + ((m_fRotacionHelice-130)*144/110);
				float tempz=313 - ((m_fRotacionHelice-130)*187/110);
				glTranslatef(-d->pivot[0]+tempx, -d->pivot[1]+m_fDistacia, -d->pivot[2]-tempz+m_fVuelo);
			} else {
				float tempx=291 - ((m_fRotacionHelice-250)*236/100);
				float tempz=19 + ((m_fRotacionHelice-250)*19/100);
				glTranslatef(-d->pivot[0]+tempx, -d->pivot[1]+m_fDistacia, -d->pivot[2]-tempz+m_fVuelo);
			}
		} else if(strcmp(node->name, "llantad")==0 || strcmp(node->name, "llantah")==0 || strcmp(node->name, "llantat")==0) {

			glTranslatef(-d->pivot[0], -d->pivot[1]+m_fDistacia, -d->pivot[2]+m_fVuelo);
			m_fRotacionLlantas= ((int)m_fRotacionLlantas)%360;
			glRotatef(m_fRotacionLlantas,1,0,0);
		} else if(strcmp(node->name, "alerond")==0 || strcmp(node->name, "aleroni")==0 || strcmp(node->name, "aleront")==0) {

			glTranslatef(-d->pivot[0], -d->pivot[1]+m_fDistacia, -d->pivot[2]+m_fVuelo);
			float rotacion= m_fVelocidad*60/VELOCIDAD_DESPEGUE;
			if(rotacion>60)
				rotacion=60;
			glRotatef(rotacion,1,0,0);
		} else {
			glTranslatef(-d->pivot[0], -d->pivot[1]+m_fDistacia, -d->pivot[2]+m_fVuelo);
		}

      glCallList(mesh->user.d);
      glPopMatrix();
      glFlush();
    }
  }
}

void C3DsLoader::Reset() {

	inicio= GetTickCount();
	m_fRotacionHelice=0;
	m_fRotacionLlantas=0;
	m_fVuelo=0;
	m_fVelocidad=0;
	m_dwInicioAceleracion=0;	
	m_bAcelerando=false;
	m_fDistacia=0;
}

void C3DsLoader::OnIdle() {

	if(m_fDistacia>TAMANO_PISTA) { // fin

		Reset();
		return;
	}

	DWORD temp= inicio;
	inicio= GetTickCount();
	temp=inicio-temp;
	if(m_fVelocidad>0) {
		m_fRotacionHelice+=10*temp; // milisegundos
		m_fRotacionLlantas+=10*temp;
	}

	if(m_bAcelerando) {
		m_bAcelerando=false;
	} else {
		DesAcelerar(false);
	}
	glutPostRedisplay();
}

void C3DsLoader::DesAcelerar(bool freno/*=false*/) {

	if(m_fVelocidad==0) 
		return;

	if(m_fVelocidad>VELOCIDAD_DESPEGUE) {// ya despega

		Acelerar(); // piloto automatico
		return;
	}

	DWORD tiempo= m_dwInicioAceleracion;
	m_dwInicioAceleracion= GetTickCount();
	if(tiempo)
		tiempo= m_dwInicioAceleracion-tiempo;
	float segundos= (float)tiempo/1000;
	float aceleracion= freno ? ACELERACION*5 : ACELERACION/3;
	float avance= (aceleracion*segundos*segundos/2)+(m_fVelocidad*segundos);
	if(m_fVelocidad>VELOCIDAD_DESPEGUE) {
		m_fVuelo+=avance*10;
	}
	avance*=1000;
	m_fDistacia= m_fDistacia+avance;
	m_fVelocidad= aceleracion*-segundos + m_fVelocidad;
	if(m_fVelocidad<0)
		m_fVelocidad=0;
}

void C3DsLoader::Acelerar() {

	m_bAcelerando=true;
	DWORD tiempo= m_dwInicioAceleracion;
	m_dwInicioAceleracion= GetTickCount();
	if(tiempo)
		tiempo= m_dwInicioAceleracion-tiempo;
	float segundos= (float)tiempo/1000;
	float avance= (ACELERACION*segundos*segundos/2)+(m_fVelocidad*segundos);
	if(m_fVelocidad>VELOCIDAD_DESPEGUE) {
		m_fVuelo+=avance*10;
	}
	avance*=1000;
	m_fDistacia= m_fDistacia+avance;
	m_fVelocidad= ACELERACION*segundos + m_fVelocidad;
}

float C3DsLoader::GetVelocidad() const {

	return m_fVelocidad*3.6;
}

void C3DsLoader::light_update(Lib3dsLight *l)
{
  Lib3dsNode *ln, *sn;

  ln = lib3ds_file_node_by_name(file, l->name, LIB3DS_LIGHT_NODE);
  sn = lib3ds_file_node_by_name(file, l->name, LIB3DS_SPOT_NODE);

  if( ln != NULL ) {
    memcpy(l->color, ln->data.light.col, sizeof(Lib3dsRgb));
    memcpy(l->position, ln->data.light.pos, sizeof(Lib3dsVector));
  }

  if( sn != NULL )
    memcpy(l->spot, sn->data.spot.pos, sizeof(Lib3dsVector));
}

void C3DsLoader::Draw() {
	
	Lib3dsNode *c,*t;
  Lib3dsFloat fov, roll;
  float fnear, ffar, dist;
  float *campos;
  float *tgt;
  Lib3dsMatrix M;
  Lib3dsCamera *cam;
  Lib3dsVector v;
  Lib3dsNode *p;

  if( file != NULL && file->background.solid.use )
    glClearColor(file->background.solid.col[0],
    file->background.solid.col[1],
    file->background.solid.col[2], 1.);

  /* TODO: fog */

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*if( anti_alias )
    glEnable(GL_POLYGON_SMOOTH);
  else*/
    glDisable(GL_POLYGON_SMOOTH);


  if (!file) {
    return;
  }

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, file->ambient);

  c=lib3ds_file_node_by_name(file, camera, LIB3DS_CAMERA_NODE);
  t=lib3ds_file_node_by_name(file, camera, LIB3DS_TARGET_NODE);

  if( t != NULL )
    tgt = t->data.target.pos;

  if( c != NULL ) {
    fov = c->data.camera.fov;
    roll = c->data.camera.roll;
    campos = c->data.camera.pos;
  }

  if ((cam = lib3ds_file_camera_by_name(file, camera)) == NULL)
    return;

  fnear = cam->near_range;
  ffar = cam->far_range;

  if (c == NULL || t == NULL) {
    if( c == NULL ) {
      fov = cam->fov;
      roll = cam->roll;
      campos = cam->position;
    }
    if( t == NULL )
      tgt = cam->target;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* KLUDGE alert:  OpenGL can't handle a near clip plane of zero,
  * so if the camera's near plane is zero, we give it a small number.
  * In addition, many .3ds files I've seen have a far plane that's
  * much too close and the model gets clipped away.  I haven't found
  * a way to tell OpenGL not to clip the far plane, so we move it
  * further away.  A factor of 10 seems to make all the models I've
  * seen visible.
  */
  if( fnear <= 0. ) fnear = ffar * .001;

  gluPerspective( fov-view_camera_zoom, 1.0*gl_width/gl_height, fnear, ffar);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(-90, 1.0,0,0);

  /* User rotates the view about the target point */

  lib3ds_vector_sub(v, tgt, campos);
  dist = lib3ds_vector_length(v);

  glTranslatef(0.,dist, 0.);
  glRotatef(view_rotx, 1., 0., 0.);
  glRotatef(view_roty, 0., 1., 0.);
  glRotatef(view_rotz, 0., 0., 1.);
  glTranslatef(0.,-dist, 0.);
  if(strcmp(camera, "Camera_Y")==0)
	glTranslatef(0,-m_fDistacia, 0);
  else if(strcmp(camera, "Camera_ISO")==0) {
	// glTranslatef(-m_fDistacia/3,-m_fDistacia/3, -m_fDistacia/3);
  } else glTranslatef(-m_fDistacia,0, 0);

  lib3ds_matrix_camera(M, campos, tgt, roll);
  glMultMatrixf(&M[0][0]);

  /* Lights.  Set them from light nodes if possible.  If not, use the
  * light objects directly.
  */
  {
    static const GLfloat a[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static GLfloat c[] = {1.0f, 1.0f, 1.0f, 1.0f};
    static GLfloat p[] = {0.0f, 0.0f, 0.0f, 1.0f};
    Lib3dsLight *l;

    int li=GL_LIGHT0;
    for (l=file->lights; l; l=l->next) {
      glEnable(li);

      light_update(l);

      c[0] = l->color[0];
      c[1] = l->color[1];
      c[2] = l->color[2];
      glLightfv(li, GL_AMBIENT, a);
      glLightfv(li, GL_DIFFUSE, c);
      glLightfv(li, GL_SPECULAR, c);

      p[0] = l->position[0];
      p[1] = l->position[1];
      p[2] = l->position[2];
      glLightfv(li, GL_POSITION, p);

      if (l->spot_light) {
        p[0] = l->spot[0] - l->position[0];
        p[1] = l->spot[1] - l->position[1];
        p[2] = l->spot[2] - l->position[2];
        glLightfv(li, GL_SPOT_DIRECTION, p);
      }
      ++li;
    }
  }

  for (p=file->nodes; p!=0; p=p->next) {

		render_node(p);
    }

  // glutSwapBuffers();
}

void C3DsLoader::decompose_datapath(const char *fn)
{
  const char *ptr = strrchr(fn, '/');

  if (ptr == NULL) {
    strcpy(datapath, ".");
    strcpy(filename, fn);
  }
  else {
    strcpy(filename, ptr+1);
    strcpy(datapath, fn);
    datapath[ptr - fn] = '\0';
  }
}

void C3DsLoader::SetCamara(const char *camara) {
	
	Lib3dsCamera *c= file->cameras;
	while(c) {

		if(strcmp(camara, c->name)==0) {

			view_rotx = view_roty = view_rotz = 0;
			camera= c->name;
			break;
		}
		c= c->next;
	}
}

float C3DsLoader::GetDistanciaRecorrida() const {

	return m_fDistacia;
}
