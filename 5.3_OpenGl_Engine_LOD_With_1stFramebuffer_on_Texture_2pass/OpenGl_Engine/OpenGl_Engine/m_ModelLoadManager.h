#pragma once
#ifndef _m_modelloadmanager_
#define _m_modelloadmanager_

#include <map>

#include "ModelDataINFo.h"
#include "m_triangle.h"		
#include "m_plyOps.h"
#include "m_VertexTypes.h"

#include <vector>

#include "FBXLoader.h"



class m_ModelLoadManager
{
public:
	m_ModelLoadManager();
	~m_ModelLoadManager();

	//bool LoadModels(std::map <std::string, std::vector<std::string>>Filetoload, std::string type);
	bool LoadModels(std::vector< std::string > vecFileNames);

	bool GetRenderingInfoByModelFileName(std::string modelName,
											ModelDataINFo &theInfo);
	// Deletes any buffers
	//bool ShutDown(void);

	std::wstring ASCII_to_Unicode(std::string ASCIIstring);
	std::string Unicode_to_ASCII(std::wstring UnicodeString);

private:
	
	std::vector< ModelDataINFo > ModelDataINFo_Vector;
	//std::vector<ModelDataINFo>FBX_ModelDataINFo_Vector;
	
	//-----Plyloading-----
	bool LoadPlyFile(std::string plyfile_name, m_plyOps* &pPlyLoader);
	//--------------------
	//
	//-----FBX Loading----
	//FBXLoader m_FbxObj;
	void LoadFBXFile(const char* fbxFile_name, FBXLoader* FBXobj);
	//-------------------
	
	// To keep the original ply information around
	std::map< std::string, m_plyOps* > m_mapNameToCPlyFile;

	int numberOfIndices;
	GLuint* p_indexArray;

	


	bool m_LoadPlyFileIntoGLBuffers(std::string filetype ,FBXLoader* fbxloaderObj, m_plyOps* pPlyLoader ,//Define File format is being loaded in
									std::string plyFile,						/*CVertex_fXYZW Objcolor,*/
									unsigned int &vertexBufferID,			/* GLuint &vertexBufferID, */
									unsigned int &vertexColourBufferID,		/* GLuint &vertexColourBufferID, */
									unsigned int &indexBufferID,			/* GLuint &indexBufferID, */
									int &numberOfVerticesLoaded,
									int &numberOfTrianglesLoaded,
									float &maxExtent);

	int n;
};
#endif
