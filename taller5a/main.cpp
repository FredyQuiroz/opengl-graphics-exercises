#include <windows.h>
#include <stdio.h>
#include "../include/gl.h"
#include "../include/glu.h"
#include "../include/glaux.h"
#include <math.h>
# include <conio.h>

#pragma comment(lib, "../lib/opengl32.lib")
#pragma comment(lib, "../lib/GlAux.Lib")
#pragma comment(lib, "../lib/glu32.lib")

void CALLBACK funcionDeReescalado(GLsizei w, GLsizei h)
{
	glMatrixMode (GL_PROJECTION); //activar la matriz deproyeccion
		glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 2.0, 10.0);
	//Definicion del volumen de
	// visualizacion
	glViewport (0, 0, w, h); // definir el viewport
	glMatrixMode (GL_MODELVIEW);
	/* restaurar la matriz de modelo-vista como activa*/
}

void CALLBACK display (void) {

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -2.0);

	glClearColor(0.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat fX=-0.4,fY=0.5;

	glBegin(GL_QUADS);
	glColor3f (1.0f, 0.0f, 1.0f);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.8, fY, 0.0f);
	glVertex3f(fX+0.8, fY-1.4, 0.0f);
	glVertex3f(fX, fY-1.4, 0.0f);
	glEnd();

	fX=-0.8;
	fY=-0.7;

	glColor3f (1.0f, 1.0f, 1.0f);
	glEnable (GL_LINE_STIPPLE);
	glLineWidth (8);
	glLineStipple (2,0xFF);

	glBegin(GL_LINE_LOOP);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+1.6, fY, 0.0f);
	glVertex3f(fX+0.8, fY+1.6, 0.0f);
	glEnd();

	fX=-0.7;
	fY=-0.6;

	glEnable (GL_LINE);
	glLineStipple (1,0xFFFF);

	glBegin(GL_TRIANGLES);
	glColor3f (0.0f, 0.0f, 1.0f); // azul
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.7, fY, 0.0f);
	glVertex3f(fX+0.35, fY+0.7, 0.0f);

	glColor3f (0.0f, 1.0f, 0.0f); // verde
	glVertex3f(fX+0.7, fY, 0.0f);
	glVertex3f(fX+1.4, fY, 0.0f);
	glVertex3f(fX+1.05, fY+0.7, 0.0f);

	glColor3f (1.0f, 0.0f, 0.0f); // rojo
	glVertex3f(fX+0.35, fY+0.7, 0.0f);
	glVertex3f(fX+1.05, fY+0.7, 0.0f);
	glVertex3f(fX+0.7, fY+1.4, 0.0f);

	// multicolor
	glColor3f (0.0f, 1.0f, 0.0f); // verde
	glVertex3f(fX+0.35, fY+0.7, 0.0f);
	glColor3f (0.0f, 0.0f, 1.0f); // azul
	glVertex3f(fX+1.05, fY+0.7, 0.0f);
	glColor3f (1.0f, 0.0f, 0.0f); // rojo
	glVertex3f(fX+0.7, fY, 0.0f);

	glEnd();

	glPointSize(10.0f);
	glColor3f (1.0f, 1.0f, 1.0f);

	glBegin(GL_POINTS);
	glVertex3f(fX+0.7, fY+0.35, 0.0f);
	glEnd();

	glPointSize(6);
	glBegin(GL_POINTS);
	glVertex3f(fX+0.35, fY+0.35, 0.0f);
	glVertex3f(fX+1.05, fY+0.35, 0.0f);
	glVertex3f(fX+0.7, fY+1.05, 0.0f);
	glEnd();

	glFlush();
}

void main(int argc, char** argv)
{
	auxInitDisplayMode (AUX_SINGLE | AUX_RGBA);
	auxInitPosition (0, 0, 640, 480);
	auxInitWindow ("Taller 5");
	auxReshapeFunc(funcionDeReescalado);
	auxMainLoop(display);
} 
