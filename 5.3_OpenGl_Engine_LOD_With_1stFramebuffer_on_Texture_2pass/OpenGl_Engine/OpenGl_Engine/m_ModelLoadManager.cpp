#include "m_ModelLoadManager.h"

#include <fstream>			// For loading the files ifstream
#include <sstream>			// for the Unicode to ASCII conversion methods
#include "m_Triangle.h"		// also includes CVertex

#include "TextureLoader.h"
#include "m_global.h"

m_ModelLoadManager::m_ModelLoadManager()
{
	m_FbxObj = new FBXLoader();
	n = 0;
}
m_ModelLoadManager::~m_ModelLoadManager()
{}

bool m_ModelLoadManager::LoadModels(std::vector< std::string > vecFileNames)
{
//-------------------------------LOAD PLY MODELS-------------------------------------------	
	for (int index = 0; index != vecFileNames.size(); index++)
	{
		if (vecFileNames[index].substr(vecFileNames[index].length() - 3) == "ply")
		{
			std::string theFileName = vecFileNames[index];
			ModelDataINFo tempModelInfo;
			//tempModelInfo.model_filetype = "PLY";

			m_plyOps* temp_pPlyLoader = new m_plyOps();

			std::string filetype = "PLY";
			
			LoadPlyFile(vecFileNames[index], temp_pPlyLoader);//------Load Aply file if extension is .ply

			if (!this->m_LoadPlyFileIntoGLBuffers(filetype, m_FbxObj, temp_pPlyLoader, theFileName,
				tempModelInfo.vertexPositionBufferID,
				tempModelInfo.vertexColourBufferID,
				tempModelInfo.indexBufferID,
				tempModelInfo.numberOfVertices,
				tempModelInfo.numberOfElements,
				tempModelInfo.extent))
			{
				return false;
			}
			tempModelInfo.plyFileASCII = theFileName;
			this->ModelDataINFo_Vector.push_back(tempModelInfo);
		}

//-------------------------------LOAD FBX MODELS-------------------------------------------------
		else if (vecFileNames[index].substr(vecFileNames[index].length() - 3) == "fbx")
		{
			std::string theFileName = vecFileNames[index];
			ModelDataINFo tempModelInfo;
			//tempModelInfo.model_filetype = "FBX";

			m_plyOps* temp_p_plyLoader = new m_plyOps();//tmp fix
			
			std::string filetype = "FBX";
					
					m_FbxObj;//-------->a FBX object
			
			LoadFBXFile(vecFileNames[index].c_str(), m_FbxObj);//---Loading in an fbx model.
			
			if (!this->m_LoadPlyFileIntoGLBuffers(filetype, m_FbxObj, temp_p_plyLoader, theFileName,
				tempModelInfo.vertexPositionBufferID,
				tempModelInfo.vertexColourBufferID,
				tempModelInfo.indexBufferID,
				tempModelInfo.numberOfVertices,
				tempModelInfo.numberOfElements,
				tempModelInfo.extent))
			{
				return false;
			}

			tempModelInfo.plyFileASCII = theFileName;
			this->ModelDataINFo_Vector.push_back(tempModelInfo);
		}
//---------------------------------------------------------------------------------------------------		
	}
	return true;
}

bool m_ModelLoadManager::m_LoadPlyFileIntoGLBuffers(std::string filetype, FBXLoader* fbxloaderObj, m_plyOps* plyloaderObj,
													std::string plyFile,							/* CVertex_fXYZW Objcolor,*/
													unsigned int &vertexBufferID,					/* GLuint &vertexBufferID, */
													unsigned int &vertexColourBufferID,				/* GLuint &vertexColourBufferID, */
													unsigned int &indexBufferID,						/* GLuint &indexBufferID, */
													int &numberOfVerticesLoaded,
													int &numberOfTrianglesLoaded,
													float &maxExtent)
{
	//--------------Read and Set Textures in buffer-------------
	TextureLoader locTexObj;
	locTexObj.textureLoader();
	ExitOnGLError("ERROR: Could not generate the VAO");
	//----------------------------------------------------------
	if (n == 0)
	{
		//----------------------------Generating the FrameBuffer-----------------------------
		glGenFramebuffers(1, &g_ShaderUniformVariables.frame_BufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, g_ShaderUniformVariables.frame_BufferID);
		glBindTexture(GL_TEXTURE_2D, 0);
		//----------------------------------------------------------------------------------- 
		//----------------------Genearting and setting the Epty textures for FrameBUffers------------------------------------->
		locTexObj.create_EmptyTexture_ID();//---------Creates Empty Texture IDs for Offscreen Rendring------------------------>
		// Attach the colour and depth textures to the frame buffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_ShaderUniformVariables.color_BufferID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_ShaderUniformVariables.depth_BufferID, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Problem with frame buffer" << std::endl;
		//------------------------------------------------------------------------------------------------------------------->
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		n++;
	}
	
	//----------------------------------------------------------------------------------------------------------------------->

	if (filetype == "PLY")
	{
		maxExtent = plyloaderObj->getMaxExtent();
		numberOfVerticesLoaded = plyloaderObj->GetNumberOfVerticies();

		CVertex_fXYZW* p_ObjectArrayVerts = new CVertex_fXYZW[numberOfVerticesLoaded];
		CVertex_fXYZW* p_ObjectArrayColor = new CVertex_fXYZW[numberOfVerticesLoaded];
		CVertex_fXYZW* p_ObjectArrayNormal = new CVertex_fXYZW[numberOfVerticesLoaded];
		CVertex_fXYZW* p_ObjectUVArray = new CVertex_fXYZW[numberOfVerticesLoaded];//For UVCooord

		for (int vertIndex = 0; vertIndex != numberOfVerticesLoaded; vertIndex++)
		{
			PLYVERTEX tempVertex = plyloaderObj->getVertex_at(vertIndex);

			p_ObjectArrayVerts[vertIndex].x = tempVertex.xyz.x;
			p_ObjectArrayVerts[vertIndex].y = tempVertex.xyz.y;
			p_ObjectArrayVerts[vertIndex].z = tempVertex.xyz.z;
			p_ObjectArrayVerts[vertIndex].w = 1.0f;

			p_ObjectArrayColor[vertIndex].x = 0.5;
			p_ObjectArrayColor[vertIndex].y = 0.5;
			p_ObjectArrayColor[vertIndex].z = 0.5;
			p_ObjectArrayColor[vertIndex].w = 1.0f;	//Color

			p_ObjectArrayNormal[vertIndex].x = tempVertex.nx;
			p_ObjectArrayNormal[vertIndex].y = tempVertex.ny;
			p_ObjectArrayNormal[vertIndex].z = tempVertex.nz;
			p_ObjectArrayNormal[vertIndex].w = 1.0f;//Normals

			p_ObjectUVArray[vertIndex].x = tempVertex.tex0u;
			p_ObjectUVArray[vertIndex].y = tempVertex.tex0v;
			p_ObjectUVArray[vertIndex].z = 0.0f;
			p_ObjectUVArray[vertIndex].w = 0.0f;  //For The UVCoords
		}

	
		// Now set up the OpenGL buffers and copy the information over
		glGenVertexArrays(1, &vertexBufferID);
		glBindVertexArray(vertexBufferID);//<----Vertex Buffer Object 
		//glGetError();
		ExitOnGLError("ERROR: Could not generate the VAO");

		// "Bind" them, which means they are ready to be loaded with data
		glGenBuffers(1, &vertexColourBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexColourBufferID);//<-----Vertex AttributeID
		//glGetError();
		ExitOnGLError("ERROR: Could not bind the VAO");

		//---------------------------------------------------------------
		int totalBufferSize = sizeof(CVertex_fXYZW) * numberOfVerticesLoaded /* Position */
			+ sizeof(CVertex_fXYZW) * numberOfVerticesLoaded /* Colour */
			+ sizeof(CVertex_fXYZW) * numberOfVerticesLoaded /* Normal */
			+ sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;/* Texture */
		//---------------------------------------------------------------
		glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_DYNAMIC_DRAW);
		//---------------------------------------------------------------
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		// Now set the data n the Opengl Structure
		unsigned int offsetToPositions = 0;
		unsigned int offsetToNormals = offsetToPositions + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;
		unsigned int offsetToUvs = offsetToNormals + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;
		unsigned int offsetToColours = offsetToUvs + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;

		glBufferSubData(GL_ARRAY_BUFFER, offsetToPositions, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_ObjectArrayVerts);
		glBufferSubData(GL_ARRAY_BUFFER, offsetToColours, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_ObjectArrayColor);
		glBufferSubData(GL_ARRAY_BUFFER, offsetToNormals, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_ObjectArrayNormal);
		glBufferSubData(GL_ARRAY_BUFFER, offsetToUvs, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_ObjectUVArray);//For uvdata
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToPositions));//ObjectVertices
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToColours));//Colors
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToNormals));//Normals 
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToUvs));//For UVcoords
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glEnableVertexAttribArray(0);			// Position
		glEnableVertexAttribArray(1);			// color
		glEnableVertexAttribArray(2);           //normals
		glEnableVertexAttribArray(3);           //UV
		ExitOnGLError("ERROR: Could not enable vertex attributes");

		//------------------------------------------------------------------------
		//SETTING and BINDING THE INDEX DATA BELOW-----------------------------------------
		//------------------------------------------------------------------------

		numberOfTrianglesLoaded = plyloaderObj->GetNumberOfElements();
		int numberOfIndices = numberOfTrianglesLoaded * 3;
		 p_indexArray = new GLuint[numberOfIndices];

		for (int triIndex = 0; triIndex < numberOfTrianglesLoaded; triIndex++)	// Step by three
		{
			//CTriangle tempTri = vecTriangles[triIndex];
			PLYELEMENT tempTri = plyloaderObj->getElement_at(triIndex);

			int index1 = (triIndex * 3);
			int index2 = (triIndex * 3) + 1;
			int index3 = (triIndex * 3) + 2;

			p_indexArray[index1] = (GLuint)tempTri.vertex_index_1;
			p_indexArray[index2] = (GLuint)tempTri.vertex_index_2;
			p_indexArray[index3] = (GLuint)tempTri.vertex_index_3;
		}

		// Now do the same for the index buffer
		glGenBuffers(1, &indexBufferID);			// Get an ID (aka "name")
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		ExitOnGLError("ERROR: Could not set VAO attributes");

		// Figure out how many "system units" (i.e. "bytes") the index buffer is
		// Or, how many bytes is GLuint (on this system) X number of indices
		int bytesInIndexArray = numberOfIndices * sizeof(GLuint);
		// Copy the data from the CPU RAM to the Video card RAM
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, p_indexArray, GL_STATIC_DRAW);//For indices s
		ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

		glBindVertexArray(0);
		ExitOnGLError("ERROR: Could not set VAO attributes");

		delete[] p_ObjectArrayVerts;
		delete[] p_ObjectArrayNormal;
		delete[] p_ObjectArrayColor;
		delete[] p_indexArray;
	}

  else if (filetype == "FBX")
	{
		numberOfVerticesLoaded = fbxloaderObj->get_FBXvertexCount();

		// Now set up the OpenGL buffers and copy the information over
		glGenVertexArrays(1, &vertexBufferID);
		glBindVertexArray(vertexBufferID);//<----Vertex Buffer Object 
		//glGetError();
		ExitOnGLError("ERROR: Could not generate the VAO");

		// "Bind" them, which means they are ready to be loaded with data
		glGenBuffers(1, &vertexColourBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexColourBufferID);//<-----Vertex AttributeID
		ExitOnGLError("ERROR: Could not bind the VAO");

		//---------------------------------------------------------------
		int totalBufferSize = sizeof(CVertex_fXYZW) * numberOfVerticesLoaded /* Position */
			+ sizeof(CVertex_fXYZW) * numberOfVerticesLoaded /* Colour */
			+ sizeof(CVertex_fXYZW) * numberOfVerticesLoaded /* Normal */
			+ sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;/* Texture */
		//---------------------------------------------------------------
		glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_DYNAMIC_DRAW);
		//---------------------------------------------------------------
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		// Now set the data n the Opengl Structure   (Directly loading the Vectors into the Buffers) 
		unsigned int offsetToPositions = 0;
		unsigned int offsetToColours = offsetToPositions + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;
		unsigned int offsetToNormals = offsetToColours + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;
		unsigned int offsetToUvs = offsetToNormals + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;

		glBufferSubData(GL_ARRAY_BUFFER, offsetToPositions, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, &fbxloaderObj->get_FbxVertex_Vector()[0]);
		glBufferSubData(GL_ARRAY_BUFFER, offsetToColours, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, &fbxloaderObj->get_FBXColor_Vector()[0]);
		glBufferSubData(GL_ARRAY_BUFFER, offsetToNormals, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, &fbxloaderObj->get_FBXNormals_Vector()[0]);
		glBufferSubData(GL_ARRAY_BUFFER, offsetToUvs, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, &fbxloaderObj->get_FBXUV_Vector()[0]);
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToPositions));//ObjectVertices
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToColours));//Colors
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToNormals));//Normals 
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(offsetToUvs));//For UVcoords
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glEnableVertexAttribArray(0);			// Position
		glEnableVertexAttribArray(1);			// color
		glEnableVertexAttribArray(2);           //normals
		glEnableVertexAttribArray(3);           //UV
		ExitOnGLError("ERROR: Could not enable vertex attributes");

		//------------------------------------------------------------------------
		//SETTING and BINDING THE INDEX DATA BELOW--------------------------------
		//------------------------------------------------------------------------

		numberOfTrianglesLoaded = fbxloaderObj->get_FBXPoligonCount();
		int numberOfIndices = numberOfTrianglesLoaded * 3;
		//GLuint* indexArray = new GLuint[numberOfIndices];
		//numberOfIndices = 6;
		//for (int i = 0; i < numberOfIndices; i++)
		// {
		//	 indexArray[i] = fbxloaderObj->getIndexDataVector()[i];//through this
		//	// std::cout << indexArray[i] << ";";//not getting correct values here
		// }

		// Now do the same for the index buffer
		glGenBuffers(1, &indexBufferID);			// Get an ID (aka "name")
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		ExitOnGLError("ERROR: Could not set VAO attributes");

		// Figure out how many "system units" (i.e. "bytes") the index buffer is
		// Or, how many bytes is GLuint (on this system) X number of indices
		int bytesInIndexArray = numberOfIndices * sizeof(GLuint);
		// Copy the data from the CPU RAM to the Video card RAM
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &fbxloaderObj->getIndexDataVector()[0], GL_STATIC_DRAW);//For indices s
		ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

		glBindVertexArray(0);
		ExitOnGLError("ERROR: Could not set VAO attributes");
	}
	return true;
}

bool m_ModelLoadManager::GetRenderingInfoByModelFileName(std::string modelName,ModelDataINFo &theInfo)
{
	for (int index = 0; index != (int)this->ModelDataINFo_Vector.size(); index++)
	{	
		if (this->ModelDataINFo_Vector[index].plyFileASCII == modelName)
		{	
			theInfo = this->ModelDataINFo_Vector[index];
			return true;
		}
	}
	return true;
}

bool m_ModelLoadManager::LoadPlyFile(std::string plyFile, m_plyOps* &pPlyLoader)
{
	
	std::wstring error;	// Returns error info in this string
	if (!pPlyLoader->OpenPLYFile(this->ASCII_to_Unicode(plyFile), error))
	{
		return false;
	}
	if (!pPlyLoader->bHadNormalsInFile())
	{
		pPlyLoader->normalizeTheModelBaby(); // For normals
	}
	if (!pPlyLoader->bHadTextureCoordinatesInFile())
	{
		pPlyLoader->GenTextureCoordsSpherical(m_plyOps::POSITIVE_X, m_plyOps::POSITIVE_Y, true, 1.0, false);
	}

}

void m_ModelLoadManager::LoadFBXFile(const char* fbxFile_name, FBXLoader* FBXobj)
{
	//-------Loading Objects From FBX----------
	FBXobj->LoadFBX(fbxFile_name);
	//-----------------------------------------
}


// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
// But this is "good enough" for now
std::wstring m_ModelLoadManager::ASCII_to_Unicode(std::string ASCIIstring)
{
	std::wstringstream ssReturnUNICODE;
	for (std::string::iterator itChar = ASCIIstring.begin(); itChar != ASCIIstring.end(); itChar++)
	{
		wchar_t theChar = static_cast<wchar_t>(*itChar);
		ssReturnUNICODE << theChar;
	}
	return ssReturnUNICODE.str();
}


std::string m_ModelLoadManager::Unicode_to_ASCII(std::wstring UnicodeString)
{
	std::stringstream ssReturnASCII;
	for (std::wstring::iterator itChar = UnicodeString.begin(); itChar != UnicodeString.end(); itChar++)
	{
		char theChar = static_cast<char>(*itChar);
		ssReturnASCII << theChar;
	}
	return ssReturnASCII.str();
}


//bool m_ModelLoadManager::ShutDown(void)
//{
//	for (std::vector< ModelDataINFo >::iterator itModel = this->ply_ModelDataINFo_Vector.begin();
//		itModel != this->ply_ModelDataINFo_Vector.end(); ++itModel)
//	{
//		ModelDataINFo tempInfo = *itModel;
//
//		glDeleteBuffers(1, &(tempInfo.indexBufferID));
//		glDeleteBuffers(1, &(tempInfo.vertexColourBufferID));
//		glDeleteBuffers(1, &(tempInfo.vertexPositionBufferID));
//
//		// Set to zero, just in case
//		itModel->vertexPositionBufferID = 0;
//		itModel->vertexColourBufferID = 0;
//		itModel->indexBufferID = 0;
//	}
//
//	// Clear the vector as all the buffers are invalid
//	this->ply_ModelDataINFo_Vector.clear();
//
//	return true;
//}