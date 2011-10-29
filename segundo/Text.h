#pragma once

class CText
{
protected:
	char d_text[1024];	// table containing fps information
	float d_red, d_green, d_blue, d_x, d_y; // color of fps information
	void* d_fonts_p;	// glut fonts
	// and its location within the window
public:
	CText();
	~CText();
	void init(void* fonts, float red, float green, float blue, float x, float y);
	virtual void Display();
	void SetText(const char *texto);
};
