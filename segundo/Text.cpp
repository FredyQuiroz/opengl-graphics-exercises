#include "Text.h"
#pragma comment(lib, "../lib/glut32.lib")
#include "../include/glut.h"
#include <string>

CText::CText(void)
{
	d_text[0]='\0';
}

CText::~CText(void)
{
}

void CText::SetText(const char *texto) {

	strcpy(d_text, texto);
}

void CText::init(void* fonts, float red, float green, float blue, float x, float y) {

	d_fonts_p = fonts;
	d_red = red;
	d_green = green;
	d_blue = blue;
	d_x = x;
	d_y = y;
}

void CText::Display() {

	if(!d_text[0])
		return;

	GLint matrixMode;
	GLboolean lightingOn;

	lightingOn= glIsEnabled(GL_LIGHTING);
	if (lightingOn) glDisable(GL_LIGHTING);

	glGetIntegerv(GL_MATRIX_MODE, &matrixMode); 

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColor3f(d_red, d_green, d_blue);
	glRasterPos3f(d_x, d_y, 0.0);
	for(char *ch = d_text; *ch; ch++) {
		glutBitmapCharacter(d_fonts_p, (int)*ch);
	}
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(matrixMode);
	if (lightingOn) glEnable(GL_LIGHTING);
}