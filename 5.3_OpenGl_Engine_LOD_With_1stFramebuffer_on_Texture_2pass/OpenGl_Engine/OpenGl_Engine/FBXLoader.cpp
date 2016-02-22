#include "FBXLoader.h"
#include <iostream>
#include <stdio.h>

FBXLoader::FBXLoader()
{
	numTabs = 0;
}
FBXLoader::~FBXLoader()
{}

void FBXLoader::PrintTabs()
{
	for (int i = 0; i < numTabs; i++)
		printf("\t");
}

FbxString FBXLoader::GetAttributeTypeName(FbxNodeAttribute::EType type) 
{
	switch (type)
	{
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
	}
}

void FBXLoader::PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void FBXLoader::PrintNode(FbxNode* pNode, FbxGeometryConverter converter)
{
	PrintTabs();
	const char* nodeName = pNode->GetName();
	Fbxtranslation = pNode->LclTranslation.Get();
	Fbxrotation = pNode->LclRotation.Get();
	Fbxscaling = pNode->LclScaling.Get();

	// Print the contents of the node.
	printf("<node name='%s' position='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
		nodeName,
		Fbxtranslation[0], Fbxtranslation[1], Fbxtranslation[2],
		Fbxrotation[0], Fbxrotation[1], Fbxrotation[2],
		Fbxscaling[0], Fbxscaling[1], Fbxscaling[2]
		);

	FBX_Vertex_Vector.clear();
	FBX_index_Vector.clear();
	FBX_Vector_Fr_normals.clear();
	FBX_Vector_Fr_UV.clear();
	FBX_Vector_Fr_Color.clear();

	numTabs++;

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));
	}

	//-.-.-.-.-.-.-.-.-..-.-.-.-.-.-.-.-.-.-.-..--.-.-.-.-.-..-.-.-.-.-.-.-.-.--.-.-.-.-.-.-.
	// Print the node's attributes.
	std::cout << "This node has " << pNode->GetNodeAttributeCount() << " attribute(s):" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

		FbxNodeAttribute::EType lAttributeType = pNode->GetNodeAttribute()->GetAttributeType();

		if (lAttributeType == FbxNodeAttribute::eMesh)
		{
			//std::cout << " Model name: >" << pNode->GetName() << "<" << std::endl;

			FbxModelName = pNode->GetName();

			FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();

			// This is an array of all the vertices...
			FbxVector4* lControlPoints = pMesh->GetControlPoints();

			vertex_Count = pMesh->GetControlPointsCount();//pMesh->GetPolygonVertexCount(); 
			std::cout << "has" << vertex_Count << "vertieces" << std::endl;
			index_Count = pMesh->GetPolygonVertexCount();
			std::cout << "has" << index_Count << "indices" << std::endl;
			poligon_Count = pMesh->GetPolygonCount();
			std::cout << "has" << poligon_Count << "polygons" << std::endl;
			elementCount_UV = pMesh->GetElementUVCount();
			std::cout << "has" << elementCount_UV << "UV elements" << std::endl;
			

			if (elementCount_UV == 0)
			{
				std::cout << "(Model doesn't have any texture coordinates)" << std::endl;
			}
			//-------------------------------Storeing Vertex Data---------------------------->
			for (int vertIndex = 0; vertIndex != vertex_Count; vertIndex++)
			{
				CVertex_fXYZW FBX_Vertex_array;

				//Geting index data 2nd way-->

				FbxVector4 controlPoint = lControlPoints[vertIndex];

				FBX_Vertex_array.x = (float)controlPoint.mData[0];
				FBX_Vertex_array.y = (float)controlPoint.mData[1];
				FBX_Vertex_array.z = (float)controlPoint.mData[2];

				FBX_Vertex_Vector.push_back(FBX_Vertex_array);
				//----------------------------------------------------------------------------->
				
			}
			std::cout << "(Model Vertices have been Loaded)" << std::endl;
			for (int vertex_index = 0; vertex_index < index_Count; vertex_index++)
			{
				//---------------Getting INdex Data-------------------------------------------->
				FBX_index_Vector.push_back(pMesh->GetPolygonVertices()[vertex_index]);

				//---------------Getting normal data?------------------------------------------>
				CVertex_fXYZW normalVec4Obj;
				ReadNormal(pMesh, vertex_index, vertex_index, normalVec4Obj);
				FBX_Vector_Fr_normals.push_back(normalVec4Obj);

				//---------------Geting UV data------------------------------------------------>
				CVertex_fXYZW pUV;
				for (int uvlayer = 0; uvlayer < 2; uvlayer++)
				{ReadUV(pMesh, vertex_index, vertex_index, uvlayer, pUV);}
				FBX_Vector_Fr_UV.push_back(pUV);
				//----------------------------------------------------------------------------->
				CVertex_fXYZW Color;
				ReadColor(pMesh, vertex_index, vertex_index, Color);
				FBX_Vector_Fr_Color.push_back(Color);
			}
			std::cout << "(Model Incdics,Normals & UV have been Loaded)" << std::endl;

		}
	}

	std::cout << "----------------------------------------" << std::endl;
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		PrintNode(pNode->GetChild(j), converter);

		numTabs--;
		PrintTabs();
		printf("</node>\n");
	}
}
	
void FBXLoader::ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, CVertex_fXYZW &pNormal)
{
	if (pMesh->GetElementNormalCount() < 1)
	{
		return;
	}

	FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
	switch (leNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (leNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pNormal.x = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];
			pNormal.y = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];
			pNormal.z = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id    = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
			pNormal.x = leNormal->GetDirectArray().GetAt(id)[0];
			pNormal.y = leNormal->GetDirectArray().GetAt(id)[1];
			pNormal.z = leNormal->GetDirectArray().GetAt(id)[2];
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (leNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pNormal.x = leNormal->GetDirectArray().GetAt(vertexCounter)[0];
			pNormal.y = leNormal->GetDirectArray().GetAt(vertexCounter)[1];
			pNormal.z = leNormal->GetDirectArray().GetAt(vertexCounter)[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = leNormal->GetIndexArray().GetAt(vertexCounter);
			pNormal.x = leNormal->GetDirectArray().GetAt(id)[0];
			pNormal.y = leNormal->GetDirectArray().GetAt(id)[1];
			pNormal.z = leNormal->GetDirectArray().GetAt(id)[2];
		}
		break;

		default:
			break;
		}
	}
	break;
	}
	
}

void FBXLoader::ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, CVertex_fXYZW &pUV)
{
	if (uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)
	{
		return;
	}

	FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

	switch (pVertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pUV.x = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[0];
			pUV.y = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[1];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);
			pUV.x = pVertexUV->GetDirectArray().GetAt(id)[0];
			pUV.y = pVertexUV->GetDirectArray().GetAt(id)[1];
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			pUV.x = pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
			pUV.y = pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
		}
		break;

		default:
			break;
		}
	}
	break;
	}
}

void FBXLoader::ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, CVertex_fXYZW &pColor)//Texture Data
{
	if (pMesh->GetElementVertexColorCount() < 1)
	{
		return;
	}

	FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);
	switch (pVertexColor->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (pVertexColor->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pColor.x = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
			pColor.y = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
			pColor.z = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
			pColor.w = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
			pColor.x = pVertexColor->GetDirectArray().GetAt(id).mRed;
			pColor.y = pVertexColor->GetDirectArray().GetAt(id).mGreen;
			pColor.z = pVertexColor->GetDirectArray().GetAt(id).mBlue;
			pColor.w = pVertexColor->GetDirectArray().GetAt(id).mAlpha;
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (pVertexColor->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pColor.x = pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;
			pColor.y = pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;
			pColor.z = pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;
			pColor.w = pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
			pColor.x = pVertexColor->GetDirectArray().GetAt(id).mRed;
			pColor.y = pVertexColor->GetDirectArray().GetAt(id).mGreen;
			pColor.z = pVertexColor->GetDirectArray().GetAt(id).mBlue;
			pColor.w = pVertexColor->GetDirectArray().GetAt(id).mAlpha;
		}
		break;
		default:
			break;
		}
	}
	break;
	}
}

int FBXLoader::LoadFBX(const char* filename)
{
	// Change the following filename to a suitable filename value.
	const char* lFilename = filename;

	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);
	std::cout << "FBX SceneImported" << std::endl;

	// The file is imported; so get rid of the importer.
	lImporter->Destroy();

	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should
	// not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) 
	{
		FbxGeometryConverter converter(lSdkManager);

		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			PrintNode(lRootNode->GetChild(i),converter);
		}
		
	}
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
	return 0;
}

m_Vec3d FBXLoader::get_FbxPosition()
{
	m_Vec3d tempObj;

	tempObj.x = Fbxtranslation[0];
	tempObj.y = Fbxtranslation[1];
	tempObj.z = Fbxtranslation[2];

	return tempObj;
}

m_Vec3d FBXLoader::get_FbxRotation()
{
	m_Vec3d tempObj;

	tempObj.x = Fbxrotation[0];
	tempObj.y = Fbxrotation[1];
	tempObj.z = Fbxrotation[2];

	return tempObj;
}

m_Vec3d FBXLoader::get_FbxScaling()
{
	m_Vec3d tempObj;

	tempObj.x = Fbxscaling[0];
	tempObj.y = Fbxscaling[1];
	tempObj.z = Fbxscaling[2];

	return tempObj;
}

std::vector<int> FBXLoader::getIndexDataVector()
{
	return FBX_index_Vector;
}

std::vector<CVertex_fXYZW> FBXLoader::get_FbxVertex_Vector()
{
	return FBX_Vertex_Vector;
}

std::vector<CVertex_fXYZW> FBXLoader::get_FBXNormals_Vector()
{
	return FBX_Vector_Fr_normals;
}

std::vector<CVertex_fXYZW> FBXLoader::get_FBXUV_Vector()
{
	return FBX_Vector_Fr_UV;
}

std::vector<CVertex_fXYZW> FBXLoader::get_FBXColor_Vector()
{
	return FBX_Vector_Fr_Color;
}

unsigned int FBXLoader::get_FBXvertexCount()
{
	return vertex_Count;
}

unsigned int FBXLoader::get_FBXindexCount()
{
	return index_Count;
}

unsigned int FBXLoader::get_FBXPoligonCount()
{
	return poligon_Count;
}


