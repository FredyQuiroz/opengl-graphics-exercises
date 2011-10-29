#include <windows.h>
#include <stdio.h>
#include "../include/gl.h"
#include "../include/glu.h"
#include "../include/glaux.h"
# include "FramerateDisplayer.h"
# include "C3DsLoader.h"
#include <math.h>
# include "debug.h"

#ifndef WIN32
#include <unistd.h>
#else
#define random rand
#define srandom srand
#endif

#pragma comment(lib, "../lib/opengl32.lib")
#pragma comment(lib, "../lib/GlAux.Lib")
#pragma comment(lib, "../lib/glu32.lib")
#pragma comment(lib, "../lib/glut32.lib")

#include "../include/glut.h"

FramerateDisplayer my_framerate;
CText			m_pVelocidad;
// funciones globales de animacion.
GLfloat g_fTranslate=0;
GLfloat g_fRotate=0;
GLfloat g_fScale=1;
GLfloat g_fWait=0;
DWORD inicio=0;
int rotating = 0;
int	mx, my;
C3DsLoader *m_pAvion;

// funcion para la redimencion de la ventana
void funcionDeReescalado(int w, int h)
{
	m_pAvion->SetDimensions(h, w);
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

// se encarga del dibujo.
void display (void) {

	char texto[1024];
	sprintf(texto, "%g Km/h", m_pAvion->GetVelocidad());
	m_pVelocidad.SetText(texto);

	m_pAvion->Draw();

	// pista
	glEnable ( GL_COLOR_MATERIAL );
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-5000, -5000, -960);
	glVertex3f(5000, -5000, -960);
	glVertex3f(5000, TAMANO_PISTA, -960);
	glVertex3f(-5000, TAMANO_PISTA, -960);
	glEnd();

	// linea izquierda
	glColor3f(1.0, 1.0, 0);
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
	glDisable(GL_COLOR_MATERIAL);

	// linea punteada
	glColor3f(1.0, 1.0, 1.0);
	for(float i=-5000; i<TAMANO_PISTA; i+=10000) {

		glBegin(GL_QUADS);
		glVertex3f(100, i, -940);
		glVertex3f(-100, i, -940);
		glVertex3f(-100, i+5000, -940);
		glVertex3f(100, i+5000, -940);
		glEnd();
	}

	my_framerate.anotherFrameExecuted();
	my_framerate.displayFramerate();
	m_pVelocidad.Display();

	glFlush();
	glutSwapBuffers();
}

// funcion de refresco
void funcionIdle() {

	m_pAvion->OnIdle();
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
    m_pAvion->view_rotz += 0.1 * (x - mx);
    m_pAvion->view_rotx += 0.1 * (y - my);
    mx = x;
    my = y;
    glutPostRedisplay();
  }
}

void keyboard2GL(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			m_pAvion->DesAcelerar(true);
			m_pAvion->x--;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			m_pAvion->Acelerar();
			m_pAvion->x++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			m_pAvion->view_camera_zoom++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			m_pAvion->view_camera_zoom--;
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
		m_pAvion->SetCamara("Camera_X");
		break;
	case 'y':
		m_pAvion->SetCamara("Camera_Y");
		break;
	case 'z':
		m_pAvion->SetCamara("Camera_Z");
		break;
	case 'i':
		m_pAvion->SetCamara("Camera_ISO");
		break;
	  break;
  }
}

void main(int argc, char** argv)
{
	m_pAvion= new C3DsLoader("debug\\avion.3ds");

	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Taller 6");

	my_framerate.init(0.05,GLUT_BITMAP_HELVETICA_18,1.0,0.0,0.0,0.05,0.95);
	m_pVelocidad.init(GLUT_BITMAP_HELVETICA_18,1.0,0.0,0.0,0.05,0.90);

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	inicio= GetTickCount();
	m_pAvion->SetCamara("Camera_X");
	
	glutReshapeFunc(funcionDeReescalado);
	glutIdleFunc(funcionIdle);
	glutDisplayFunc(display);
	glutMouseFunc(mouse_cb);
	glutMotionFunc(drag_cb);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2GL);

	glutMainLoop();

	delete m_pAvion;
} 
