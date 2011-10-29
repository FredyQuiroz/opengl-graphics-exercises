#pragma once

#include <windows.h>
#include <stdio.h>


class ImageBMP{
private:
	/*
		We need some memory to store the image data. Storing the data for a BMP is fairly simple since it is a
			non compressed format. We store the width and height in mWidth and mHeight. Then we need to know how 
			man channels exist inside the image. If it is a simple RGB image (24 bit) then it will only have 3 
			channels. If it had a alpha channel too for transparency, then it becomes 4 channels RGBA (32 bit).
			We store that info inside 'mType'. We need a handle that will load in our image. WE store it inside
			mBitmap and finally we have to save the image data itself. We use imgData for this. Oh yea, we also 
			need to record the amount of 'stride' in a bitmap file. This is basically the number of true bytes in
			a line of pixels in memory.
	*/
	int mWidth, mHeight;
	int mType;
	int mStride;

	HBITMAP mBitmap;

	unsigned char *imgData;

	/*
		FreeBMP is under 'private' because the user should NEVER have to call it. The class is designed to load
			the bitmap into OpenGL and then erase the image data since OpenGL creates it's own copy of it.
	*/
	void FreeBMP();

	// This function is here for the same reason. It just sizes the image for writing to
	bool SizeImg(int w, int h, int channels);
public:
	// We will initialize all our data in this function to ensure we can properly write to a bitmap
	ImageBMP();

	// Delete the data.
	~ImageBMP();


	/*
		We will need methods to access the data inside the bitmap we load. Since we want this process
			to be as quick as possible, we make it a inline function.
	*/
	inline int GetWidth() { return mWidth; };
	inline int GetHeight(){ return mHeight;};
	inline int GetType()  { return mType;  };
	inline int GetStride(){ return mStride;};

	unsigned char* GetImageData(int i);

	/* 
		This is our actual load function. As a side note, it may be wise to use a string instead of
			char* if you are using C++ (if your using classes, your using C++). I just use
			char* to help simplify things witohut needing to have namespaces and extra headers
			cloud the file a little. The actual work required to fix this is almost
			nonexistant.
	*/
	bool LoadBMP(const char* filename);
};