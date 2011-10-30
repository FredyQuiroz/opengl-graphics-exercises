#include <windows.h>
#include <stdio.h>
#include <gl\gl.h>	
#include <gl\glu.h>	
#include "../include/glaux.h"
# include "FramerateDisplayer.h"
# include "C3DsLoader.h"
#include <math.h>
# include "debug.h"

#define GL_CLAMP_TO_EDGE 0x812F	

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#pragma comment(lib, "../lib/GlAux.Lib")
#pragma comment(lib, "../lib/glut32.lib")

#include "../include/glut.h"

FramerateDisplayer my_framerate;
CText			m_pVelocidad;
CText			m_pNombre;
// funciones globales de animacion.
GLfloat blanco[] = {1.0f, 1.0f, 1.0f, 1.f};
GLfloat rojo[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat amarillo[] = {1.0f, 1.0f, 0.0f, 1.f};

GLfloat g_fTranslate=0;
GLfloat g_fRotate=0;
GLfloat g_fScale=1;
GLfloat g_fWait=0;
DWORD	g_dLucesPista=0;
int	g_nLuces=0;
int rotating = 0;
int	mx, my;
bool	m_bUsarAmbiente=true;
bool	m_bFullScreen=false;
C3DsLoader m_pAvion;

#define BACK_ID		0						
#define FRONT_ID	1						
#define BOTTOM_ID	2							
#define TOP_ID		3							
#define LEFT_ID		4							
#define RIGHT_ID	5						

GLuint textures[10];


AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle
	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}
	File=fopen(Filename,"r");							// Check To See If The File Exists
	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
	return NULL;										// If Load Failed Return NULL
}

bool LoadImage(char* file, GLuint tex[], GLuint id) {
	
	AUX_RGBImageRec *TextureImage= LoadBMP(file);
	if(!TextureImage) {
		MessageBox(NULL, "No se puede cargar fondos", file, MB_ICONERROR);
		return false;
	}

	glGenTextures(1, &tex[id]);
	glBindTexture(GL_TEXTURE_2D, tex[id]);

	glTexImage2D(GL_TEXTURE_2D,		
					0,				
					3,
					TextureImage->sizeX, TextureImage->sizeY,
					0,
					GL_RGB,
					// GL_BGR_EXT,
					GL_UNSIGNED_BYTE,
					TextureImage->data);

	gluBuild2DMipmaps(GL_TEXTURE_2D,
						3,
						TextureImage->sizeX, TextureImage->sizeY,
						GL_RGB,
						// GL_BGR_EXT,
						GL_UNSIGNED_BYTE,
						TextureImage->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	/*
		The functions below cause the image to be stretched properly to cover the border lines preventing
			black lines from showing. Try commenting these out to see the effect.
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (TextureImage->data)							// If Texture Image Exists
		free(TextureImage->data);					// Free The Texture Image Memory
	free(TextureImage);								// Free The Image Structure

	return true;
}

// funcion para la redimencion de la ventana
void funcionDeReescalado(int w, int h)
{
	m_pAvion.SetDimensions(h, w);
	// glViewport(0,0,w,h);

	glMatrixMode (GL_PROJECTION); //activar la matriz deproyeccion
	glLoadIdentity ();
	gluPerspective(90.0, (float)w/(float)h,2.0, 7.0);
	// glFrustum (-1.0, 1.0, -1.0, 1.0, 2.0, 10.0);

	//Definicion del volumen de
	// visualizacion
	glViewport (0, 0, w, h); // definir el viewport
	glMatrixMode (GL_MODELVIEW);
	/* restaurar la matriz de modelo-vista como activa*/
}

void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	/*
		All this function does is create 6 squares and distance them according to the measurements 
			provided by the user. Mess around with it if you want.
	*/

	glRotatef(90, 1, 0, 0);
	// Bind the BACK texture of the sky map to the BACK side of the cube
	glBindTexture(GL_TEXTURE_2D, textures[BACK_ID]);

	// Center the skybox
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[FRONT_ID]);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[BOTTOM_ID]);
	glBegin(GL_QUADS);		
	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[TOP_ID]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[LEFT_ID]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[RIGHT_ID]);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();

	glRotatef(-90, 1, 0, 0);
}

// se encarga del dibujo.
void display (void) {

  /* TODO: fog */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*if( anti_alias )
	glEnable(GL_POLYGON_SMOOTH);
  else*/
    glDisable(GL_POLYGON_SMOOTH);

	char texto[1024];
	sprintf(texto, "%g Km/h", m_pAvion.GetVelocidad());
	m_pVelocidad.SetText(texto);

	m_pAvion.Draw();

	if(m_bUsarAmbiente) {
		glEnable(GL_TEXTURE_2D);
		int tamano= TAMANO_PISTA/10;
		CreateSkyBox(0, tamano/2-970, strcmp(m_pAvion.GetCamara(), "Camera_ISO")==0 ? 0 : -m_pAvion.GetDistanciaRecorrida(), tamano, tamano, tamano*2);
		glDisable(GL_TEXTURE_2D);
	}

	// pista
	glEnable ( GL_COLOR_MATERIAL );
	glMaterialf(GL_FRONT, GL_SHININESS, 32.0f); // evitamos que la pista refleje la luz
	
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-5000, -5000, -960);
	glVertex3f(5000, -5000, -960);
	glVertex3f(5000, TAMANO_PISTA, -960);
	glVertex3f(-5000, TAMANO_PISTA, -960);
	glEnd();

	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, amarillo);

	// linea izquierda
	glColor3fv(amarillo);
	glBegin(GL_QUADS);
	glVertex3f(-5000, -5000, -950);
	glVertex3f(-4800, -5000, -950);
	glVertex3f(-4800, TAMANO_PISTA, -950);
	glVertex3f(-5000, TAMANO_PISTA, -950);
	glEnd();

	// linea derecha
	glBegin(GL_QUADS);
	glVertex3f(5000, -5000, -940);
	glVertex3f(4800, -5000, -940);
	glVertex3f(4800, TAMANO_PISTA, -940);
	glVertex3f(5000, TAMANO_PISTA, -940);
	glEnd();

	glMaterialfv(GL_FRONT, GL_SPECULAR, blanco);
	// linea punteada
	glColor3fv(blanco);
	for(float i=-5000; i<TAMANO_PISTA; i+=10000) {

		glBegin(GL_QUADS);
		glVertex3f(100, i, -940);
		glVertex3f(-100, i, -940);
		glVertex3f(-100, i+5000, -940);
		glVertex3f(100, i+5000, -940);
		glEnd();
	}

	glTranslatef(-4900,-5000, -890);

	glColor3fv(rojo);
	glColorMaterial( GL_FRONT_AND_BACK, GL_EMISSION );
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, rojo) ;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rojo);
	
	DWORD temp= GetTickCount()-g_dLucesPista;
	if(temp>200) { // cambio
		g_nLuces+=(int)temp/200;
		if(g_nLuces>14)
			g_nLuces=0;
		temp= g_dLucesPista;
		g_dLucesPista= GetTickCount();
		temp= g_dLucesPista-temp;
	}
	
	if(g_nLuces!=11 || g_nLuces!=13) {
		int secuencia=0;
		for(float i=-5000; i<TAMANO_PISTA; i+=10000,secuencia++) {

			if(g_nLuces==12 || g_nLuces==14 || secuencia%10==g_nLuces || secuencia%10==g_nLuces-1 || secuencia%10==g_nLuces-2) {
				glutSolidCube(120);
				glTranslatef(9800,0, 0);
				glutSolidCube(120);
				glTranslatef(-9800,0, 0);
			}
			glTranslatef(0,5000, 0);
		}
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);

	my_framerate.anotherFrameExecuted();
	my_framerate.displayFramerate();
	m_pVelocidad.Display();
	m_pNombre.Display();

	glutSwapBuffers();
}

// funcion de refresco
void funcionIdle() {

	m_pAvion.OnIdle();
}

/*!
* Respond to mouse buttons.  Action depends on current operating mode.
*/
static	void
mouse_cb(int button, int state, int x, int y)
{
  mx = x; my = y;
  if(button==GLUT_LEFT_BUTTON) {
	  rotating = state == GLUT_DOWN;
  }
}


/*!
* Respond to mouse motions; left button rotates the image or performs
* other action according to current operating mode.
*/
static void
drag_cb(int x, int y)
{
  if( rotating ) {
    m_pAvion.view_rotz += 0.1 * (x - mx);
    m_pAvion.view_rotx += 0.1 * (y - my);
    mx = x;
    my = y;
    glutPostRedisplay();
  }
}

void keyboard2GL(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			m_pAvion.DesAcelerar(true);
			m_pAvion.x--;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			m_pAvion.Acelerar();
			m_pAvion.x++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			m_pAvion.view_camera_zoom++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			m_pAvion.view_camera_zoom--;
			glutPostRedisplay();
			break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
	case 27:
	  exit(0);
	  break;
	case 'x':
		m_pAvion.SetCamara("Camera_X");
		break;
	case 'y':
		m_pAvion.SetCamara("Camera_Y");
		break;
	case 'z':
		m_pAvion.SetCamara("Camera_Z");
		break;
	case 'i':
		m_pAvion.SetCamara("Camera_ISO");
		break;
	case 'r':
		m_pAvion.Reset();
		break;
	case 'f':
		m_bUsarAmbiente=!m_bUsarAmbiente;
		break;
	case 13:
		if(m_bFullScreen) 
			glutReshapeWindow(800, 600);
		else glutFullScreen();
		m_bFullScreen=!m_bFullScreen;
		break;
	  break;
  }
}

void main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Taller 6");

	if(!m_pAvion.Load("avion.3ds")) {
		MessageBox(NULL, "No se puede cargar modelo 3d", NULL, MB_ICONERROR);
		return;
	}

	if(	!LoadImage("Top.bmp", textures, 3) ||
		!LoadImage("Bottom.bmp", textures, 2) ||
		!LoadImage("Left.bmp", textures, 4) ||
		!LoadImage("Right.bmp", textures, 5) ||
		!LoadImage("Front.bmp", textures, 1) ||
		!LoadImage("Back.bmp", textures, 0)){
		return ;
	}

#ifndef _DEBUG
	if(MessageBox(NULL, "¿Ejecutar en pantalla completa?", NULL, MB_YESNO)==IDYES) {
		m_bFullScreen=true;
		glutFullScreen();
	}
#endif

	g_dLucesPista= GetTickCount();

	my_framerate.init(0.05,GLUT_BITMAP_HELVETICA_18,1.0,0.0,0.0,0.05,0.95);
	m_pVelocidad.init(GLUT_BITMAP_HELVETICA_18,1.0,0.0,0.0,0.05,0.92);
	m_pNombre.init(GLUT_BITMAP_HELVETICA_18,0.0,0.0,1.0,0.65,0.95);
	m_pNombre.SetText("Fredy G. Quiroz Rivera");

	glClearColor(0.9, 0.9, 0.9, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	m_pAvion.SetCamara("Camera_X");
	
	glutReshapeFunc(funcionDeReescalado);
	glutIdleFunc(funcionIdle);
	glutDisplayFunc(display);
	glutMouseFunc(mouse_cb);
	glutMotionFunc(drag_cb);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2GL);

	glutMainLoop();
} 
