#ifndef _ModelDataINFo_HG_
#define _ModelDataINFo_HG_

#include <string>
#include <GL\glew.h>			// Because it complains when we don't include it
#include <GL\freeglut.h>		// Because of the GLuint

class ModelDataINFo
{
public:
	ModelDataINFo() : numberOfVertices(0), numberOfElements(0),			// "initializer" list
		vertexBufferStartIndex(0), indexBufferStartIndex(0), extent(0.0f),
		vertexPositionBufferID(0), vertexColourBufferID(0),
		indexBufferID(0) {};
	~ModelDataINFo() {};				// empty d'stor
	
	std::string model_filetype;
	int numberOfVertices;
	int numberOfElements;		// aka "Triangles"
	int vertexBufferStartIndex;
	int indexBufferStartIndex;
	float extent;				// aka "maxExtent"
	std::wstring plyFileName;

	std::string plyFileASCII;		// ADDED 

	GLuint vertexPositionBufferID;
	GLuint vertexColourBufferID;
	GLuint indexBufferID;
};



#endif