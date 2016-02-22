#pragma once

#include "m_global.h"
//#include <freeglut.h>		// Because of the GLuint


class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();


	
	void textureLoader();
	GLuint getTextureId();

	//For FrameBuffers
	void create_EmptyTexture_ID();

private:

	//bitmaploader
	GLuint textureID; //moved here
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos; // Position in the file where the actual data begins
	unsigned int width;
	unsigned int height;
	unsigned int imageSize; // = width*height*3
	// Actual RGB data
	unsigned char * data;



	bool openBitmapFile(const char* imagepath, unsigned int& width, unsigned int& height, unsigned char*& data);
	unsigned int set_TextureData(unsigned int width, unsigned int height, unsigned char* data);
	unsigned int loadTextures(const char* imagepath);
	



};

