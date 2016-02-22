#include "TextureLoader.h"
#include <iostream>
#include <fstream>
#include "Mediator.h"

//#include <glew.h>//For GL_BGR

TextureLoader::TextureLoader()
{
	textureID = 0;
}
TextureLoader::~TextureLoader()
{}


GLuint TextureLoader::getTextureId()
{
	this->textureID = textureID;
	return textureID;
}

bool TextureLoader::openBitmapFile(const char* imagepath, unsigned int& width, unsigned int& height, unsigned char*& data)
{
	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{
		{ printf("Image could not be opened\n"); return 0; }
	}

	if (fread(header, 1, 54, file) != 54)
	{ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0) imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0) dataPos = 54; // The BMP header is done that way
	

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
	return true;
	//Everything is in memory now, the file can be closed
	fclose(file);	

	
}

unsigned int TextureLoader::set_TextureData(unsigned int width, unsigned int height, unsigned char* data)
{
			glGenTextures(1, &textureID);//-->Empty GLuint TexturID gets asigned                   
			glBindTexture(GL_TEXTURE_2D, textureID); //"Bind" the newly created texture : all future texture functions will modify this texture
			ExitOnGLError("ERROR: Could not generate the VAO");
			//Give the image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
			ExitOnGLError("ERROR: Could not generate the VAO");

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			ExitOnGLError("ERROR: Could not generate the VAO");

			//return texid
			return textureID;
}

void TextureLoader::create_EmptyTexture_ID()
{
	// Create the texture to write the "colour" stuff to 
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1, &g_ShaderUniformVariables.color_BufferID);
	glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.color_BufferID);
	// Should really be a square, and a power of 2
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, screen_Width, screen_Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ExitOnGLError("ERROR: Could not generate the VAO");

	glBindTexture(GL_TEXTURE_2D, 0);
	// Create the depth texture we are going to write to (we'll need that as well)
	glGenTextures(1, &g_ShaderUniformVariables.depth_BufferID);
	glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.depth_BufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screen_Width, screen_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ExitOnGLError("ERROR: Could not generate the VAO");

}


unsigned int TextureLoader::loadTextures(const char* imagepath)
{
	openBitmapFile(imagepath, width, height, data);
	return set_TextureData(width, height, data);
}

void TextureLoader::textureLoader()
{
	//Mediator temp_med_obj;
	//std::vector <m_GameObject*>temp_GObj;
	//temp_med_obj.get_Objects_ToRender(temp_GObj);
//
	//for (int index = 0; temp_GObj.size(); index++)
	//{ // Create one OpenGL texture;
	//	if (temp_GObj[index]->get_TextureNo() == 0)
	//	{
	//		loadTextures("stuff / WoodCabinDif.bmp");
	//	}
	//	else if (temp_GObj[index]->get_TextureNo() == 1)
	//	{
	//		loadTextures("stuff / VMaskColtexture.bmp");
	//	}
	//}
	
	g_ShaderUniformVariables.g_textureID1 = loadTextures("stuff/WoodCabinDif.bmp");
	g_ShaderUniformVariables.g_textureID2 = loadTextures("stuff/VMaskColtexture.bmp");
}