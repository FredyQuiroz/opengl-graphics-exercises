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

// funciones globales de animacion.
GLfloat g_fTranslate=0;
GLfloat g_fRotate=0;
GLfloat g_fScale=1;
GLfloat g_fWait=0;

// funcion para la redimencion de la ventana
void CALLBACK funcionDeReescalado(GLsizei w, GLsizei h)
{
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

// funcion que dibuja el triangulo y el borde
void DrawTriangle(float r, float g, float b, bool dotted=false) {

	glColor3f (r, g, b); // azul

	glBegin(GL_TRIANGLES);
	glVertex3f(-1.5, -1.5, 0.0f);
	glVertex3f(-0.5, -1.5, 0.0f);
	glVertex3f(-1.0, -0.5, 0.0f);
	glEnd();


	glColor3f (1.0f, 1.0f, 1.0f); // blanco
	if(dotted) {
		glLineStipple (2,0x00FF);
	} else {
		glLineStipple (2,0xFFFF);
	}

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.5, -1.5, 0.0f);
	glVertex3f(-0.5, -1.5, 0.0f);
	glVertex3f(-1.0, -0.5, 0.0f);
	glEnd();
}

// se encarga del dibujo.
void CALLBACK display (void) {

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -2.0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable (GL_LINE_STIPPLE);
	glLineWidth(2);

	DrawTriangle(0,0,1);

	glTranslatef(0.0f, g_fTranslate, 0.0f);
	DrawTriangle(0,1,0, true);

	if(g_fTranslate>=2) {

		glRotatef(g_fRotate, 0.0f, 0.0f, 1.0f);
		glTranslatef(g_fRotate*-2.5/45, g_fRotate*-1/45, 0.0f);
		DrawTriangle(1,0,0, true);

		if(g_fRotate<=-45) {

			glScalef(g_fScale,g_fScale,1);
			glTranslatef(g_fScale*0.9/2, g_fScale*-0.2/2, 0.0f);
			DrawTriangle(1,1,0, true);
		}
	}

	glFlush();
	auxSwapBuffers();
}

// funcion de refresco
void CALLBACK funcionIdle() {

	if(g_fTranslate<2)
		g_fTranslate+=0.01f;
	else if(g_fRotate>-45)
		g_fRotate-=0.1;
	else if(g_fScale<2) 
		g_fScale+=0.001f;
	else if(g_fWait<2) 
		g_fWait+=0.002f;
	else {
		g_fWait=0;
		g_fTranslate=0;
		g_fRotate=0;
		g_fScale=1;
	}
	display();
}

void main(int argc, char** argv)
{
	auxInitDisplayMode (AUX_DOUBLE | AUX_RGBA);
	auxInitPosition (0, 0, 640, 640);
	auxInitWindow ("Taller 6");
	auxReshapeFunc(funcionDeReescalado);
	auxIdleFunc(funcionIdle);
	auxMainLoop(display);
} 
