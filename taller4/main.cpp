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

void main(int argc, char** argv)
{
	auxInitDisplayMode (AUX_SINGLE | AUX_RGBA);
	auxInitPosition (0, 0, 640, 480);
	auxInitWindow (argv[0]);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// glColor3f(R, G, B);

	GLfloat fX=-1,fY=0.9;

	glBegin(GL_LINES);
	for(int i=0;i<2; i++) {

		fX+=0.633333333333333;
		glVertex3f(fX, -1, 0.0f);
		glVertex3f(fX, 1, 0.0f);

		glVertex3f(-1, fX, 0.0f);
		glVertex3f(1, fX, 0.0f);
	}
	glEnd();

	fX=-0.9;
	glBegin(GL_POINTS);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.5, fY-0.6, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glVertex3f(fX, fY-0.6, 0.0f);
	glEnd();

	fX+=0.64;
	glBegin(GL_LINE_STRIP);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.5, fY-0.6, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glVertex3f(fX, fY-0.6, 0.0f);
	glEnd();

	fX+=0.64;
	glBegin(GL_LINE_LOOP);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.5, fY-0.6, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glVertex3f(fX, fY-0.6, 0.0f);
	glEnd();

	fY-=0.68;
	fX=-0.9;
	glBegin(GL_POLYGON);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glVertex3f(fX+0.5, fY-0.3, 0.0f);
	glVertex3f(fX+0.3, fY-0.5, 0.0f);
	glVertex3f(fX, fY-0.3, 0.0f);
	glEnd();

	fX+=0.64;
	glBegin(GL_QUADS);
	glVertex3f(fX, fY, 0.0f);
	glVertex3f(fX+0.2, fY, 0.0f);
	glVertex3f(fX+0.2, fY-0.5, 0.0f);
	glVertex3f(fX, fY-0.5, 0.0f);

	glColor3f(0xFF, 0, 0);
	glVertex3f(fX+0.3, fY, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glVertex3f(fX+0.5, fY-0.5, 0.0f);
	glVertex3f(fX+0.3, fY-0.5, 0.0f);
	glColor3f(0xFF, 0xFF, 0xFF);
	glEnd();

	fX+=0.64;
	glBegin(GL_QUAD_STRIP);
	glVertex3f(fX, fY-0.5, 0.0f);
	glVertex3f(fX, fY, 0.0f);	
	glVertex3f(fX+0.2, fY-0.5, 0.0f);
	glVertex3f(fX+0.2, fY, 0.0f);
	
	glColor3f(0xFF, 0, 0);
	glVertex3f(fX+0.5, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	/*glVertex3f(fX+0.3, fY, 0.0f);
	glVertex3f(fX+0.3, fY-0.5, 0.0f);*/
	glColor3f(0xFF, 0xFF, 0xFF);
	glEnd();

	fY-=0.68;
	fX=-0.9;
	glBegin(GL_TRIANGLES);
	glVertex3f(fX, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glEnd();

	fX+=0.64;
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(fX, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glColor3f(0xFF, 0, 0);
	glVertex3f(fX, fY, 0.0f);
	glColor3f(0xFF, 0xFF, 0xFF);
	glEnd();

	fX+=0.64;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(fX, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY-0.5, 0.0f);
	glVertex3f(fX+0.5, fY, 0.0f);
	glColor3f(0xFF, 0, 0);
	glVertex3f(fX, fY, 0.0f);
	glEnd();

	glFlush();
	// getchar();
	getch();
} 
