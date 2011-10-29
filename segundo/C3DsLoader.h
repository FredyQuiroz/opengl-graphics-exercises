#pragma once

#pragma comment(lib, "../lib/lib3ds-1_3.lib")

# include <windows.h>
#include <lib3ds/file.h>
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>

#pragma comment(lib, "../lib/glut32.lib")
#include <../include/glut.h>

# define ACELERACION	10 // m/s2
# define VELOCIDAD_DESPEGUE 83.34 // 300*10/36	// 300Km/h   83.34 m/s
# define TAMANO_PISTA	2500000	// 2500 metros

typedef struct _player_texture
{
  int valid; // was the loading attempt successful ? 
  void *bitmap;
  GLuint tex_id; //OpenGL texture ID
  float scale_x, scale_y; // scale the texcoords, as OpenGL thinks in TEX_XSIZE and TEX_YSIZE
} Player_texture;

class C3DsLoader
{
protected:
	Lib3dsFile *file;
	Lib3dsVector bmin, bmax;
	float	sx, sy, sz, size;	/* bounding box dimensions */
	float	cx, cy, cz;		/* bounding box center */
	const char* camera;
	Player_texture *pt;
	char datapath[256];
	char filename[256];
	int tex_mode;
	int gl_width;
	int gl_height;

	DWORD inicio;

	void render_node(Lib3dsNode *node);
	void decompose_datapath(const char *fn);
	void light_update(Lib3dsLight *l);

	DWORD m_dwInicioAceleracion;
	bool  m_bAcelerando;
	float m_fVelocidad;
	float m_fRotacionHelice;
	float m_fRotacionLlantas;
	float m_fDistacia;
	float m_fVuelo;
public:
	float view_rotx;
	float view_roty;
	float view_rotz;
	float view_camera_zoom;

	float x;
	float z;

	C3DsLoader(const char *filepath);
	~C3DsLoader(void);
	void Draw();
	void SetDimensions(int h, int w);
	void OnIdle();
	void SetCamara(const char *camara);
	void Acelerar();
	void DesAcelerar(bool freno=false);
	float GetVelocidad() const;
	float GetDistanciaRecorrida() const;
	void Reset();
};
