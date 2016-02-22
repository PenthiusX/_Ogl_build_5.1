#pragma once
#include <fbxsdk.h>
#include <vector>
#include "m_VertexTypes.h"
#include "m_Vec3d.h"



class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void PrintNode(FbxNode* pNode, FbxGeometryConverter converter);
	int LoadFBX(const char* filename);

	void FBXLoader::ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, CVertex_fXYZW &Normalobj);
	void ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, CVertex_fXYZW &pUV);
	void ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, CVertex_fXYZW &pColor);
	
		void PrintTabs();

	unsigned int get_FBXvertexCount();
	unsigned int get_FBXindexCount();
	unsigned int get_FBXPoligonCount();

	std::vector<int>getIndexDataVector();
	std::vector<CVertex_fXYZW>get_FbxVertex_Vector();
	std::vector<CVertex_fXYZW>get_FBXNormals_Vector();
	std::vector<CVertex_fXYZW>get_FBXUV_Vector();
	std::vector<CVertex_fXYZW>get_FBXColor_Vector();

	m_Vec3d get_FbxPosition();
	m_Vec3d get_FbxRotation();
	m_Vec3d get_FbxScaling();
	
	std::string FbxModelName;

private:

	//Translation rotation and scaling 
	FbxDouble3 Fbxtranslation;
	FbxDouble3 Fbxrotation;
	FbxDouble3 Fbxscaling;

	//Vertex, index and uv count
	unsigned int vertex_Count;
	unsigned int index_Count;
	unsigned int elementCount_UV;
	unsigned int poligon_Count;

	//Vertex Array
	std::vector<CVertex_fXYZW>FBX_Vertex_Vector;
	int numTabs;
	
	// INdex data
	std::vector<int>FBX_index_Vector;

	//Normal data
	CVertex_fXYZW* normalVec4Obj;
	std::vector<CVertex_fXYZW> FBX_Vector_Fr_normals;
	//UVData
	std::vector<CVertex_fXYZW>FBX_Vector_Fr_UV;

	//Color
	std::vector<CVertex_fXYZW> FBX_Vector_Fr_Color;



};

