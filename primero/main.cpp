#include <windows.h> // libreria estandar de windows
#include <stdio.h> // libreria estandar de C++
#include "../include/gl.h" // libreria de gl
#include "../include/glu.h" // utilidaes de opengl
#include "../include/glaux.h" // libreria auxiliar de opengl
#include <math.h>

// linquea con las librerias o definicion de simbolos de los dlls
#pragma comment(lib, "../lib/opengl32.lib")
#pragma comment(lib, "../lib/GlAux.Lib")
#pragma comment(lib, "../lib/glu32.lib")

// punto de entrada a la funcion principal
void main(int argc, char** argv)
{
	auxInitDisplayMode (AUX_SINGLE | AUX_RGBA); // inicia la ventana en modo RGB y un solo buffer. para el manejo de imagenes
	auxInitPosition (0, 0, 640, 480); // esta es la posicion de la ventana.
	auxInitWindow (argv[0]); // pasa los parametros de la ventana

	glClearColor (0.0, 0.0, 0.0, 0.0); // define el color con el cual se va a limpiar la ventana negro.
	glClear(GL_COLOR_BUFFER_BIT); // limpia la ventana con el color definido anteriormente

	GLfloat angulo;
	int i;

	glBegin(GL_LINES); // define la figura que vamos a dibujar, lineas
	for(i=0; i<360; i+=3){
		angulo = (GLfloat)i*3.14159f/180.0f;
		glVertex3f(0.0f, 0.0f, 0.0f); // punto de inicio de la linea
		glVertex3f(cos(angulo), sin(angulo), 0.0f); // punto final de la linea
	} 
	glEnd(); // termina la definicion de la figura.


	glBegin(GL_LINE_STRIP); // define la figura que vamos a dibujar
		glVertex3f(-0.5f, 0.0f, 0.0f); // punto inicial de la lineea
		glVertex3f(0.5f, 0.0f, 0.0f); // siguiente punto
		glVertex3f(0.5f, 0.5f, 0.0f); // siguiente punto
		glVertex3f(-0.5f, 0.0f, 0.0f); // linea final del triangulo.
	glEnd(); // termnia la figura.

	glFlush(); // libera el buffer de dibujo.
	getchar(); // espera un enter.
} 
