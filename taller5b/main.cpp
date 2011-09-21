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

	GLfloat angulo;
	int i;

	glColor3f (1.0f, 1.0f, 1.0f);
	glEnable (GL_LINE_STIPPLE);
	glLineWidth (5);
	glLineStipple (2,0xFF);

	glBegin(GL_LINES);
	glVertex3f(-1, 0, 0.0f); // eje y
	glVertex3f(1, 0, 0.0f);

	glVertex3f(0, -1, 0.0f); // eje x
	glVertex3f(0, 1, 0.0f);
	glEnd();

	glLineStipple (1,0xFFFF);
	glLineWidth (1);

	glBegin(GL_LINES); // define la figura que vamos a dibujar, lineas
	int color=0; // negro

	for(i=360; i>0; i-=3){
		
		glColor3f ((float) GetRValue(color)/255, (float) GetGValue(color)/255, (float) GetBValue(color)/255);
		color+=1000;
		angulo = (GLfloat)i*3.14159f/180.0f;
		glVertex3f(0.2f, 0.2f, 0.0f); // punto de inicio de la linea
		glVertex3f(cos(angulo), sin(angulo), 0.0f); // punto final de la linea
	} 
	glEnd(); // termina la definicion de la figura.

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
