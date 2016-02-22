#include "m_plyOps.h"

#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>


const float PI = 3.141592653589793238462643383279502884197f;	// Yes, yes, I *know* it's too big... whatever.


m_plyOps::m_plyOps()
{
	this->m_minX = this->m_maxX = this->m_deltaX = 0.0f;
	this->m_minY = this->m_maxY = this->m_deltaY = 0.0f;
	this->m_minZ = this->m_maxZ = this->m_deltaZ = 0.0f;
	this->m_centreX = this->m_centreY = this->m_centreZ = 0.0f;
	//
	this->m_ID = statNextID++;
	//
	this->m_numberOfVertices = 0;
	this->m_numberOfElements = 0;

	// Added March 14, 2012
	this->m_bHadNormalsInFile = false;
	this->m_bHadTextureCoordinatesInFile = false;
	this->m_totalProperties = INT_MAX;
	this->m_x_propertyIndex = INT_MAX;
	this->m_y_propertyIndex = INT_MAX;
	this->m_z_propertyIndex = INT_MAX;
	this->m_normx_propertyIndex = INT_MAX;
	this->m_normy_propertyIndex = INT_MAX;
	this->m_normz_propertyIndex = INT_MAX;
	this->m_red_propertyIndex = INT_MAX;
	this->m_green_propertyIndex = INT_MAX;
	this->m_blue_propertyIndex = INT_MAX;
	this->m_tex0u_propertyIndex = INT_MAX;
	this->m_tex0v_propertyIndex = INT_MAX;
	this->m_tex1u_propertyIndex = INT_MAX;
	this->m_tex1v_propertyIndex = INT_MAX;
	// End of Added

	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_UNKNOWN;
}

//static 
unsigned int m_plyOps::statNextID = 1;

m_plyOps::~m_plyOps()
{
	return;
}

m_plyOps::m_plyOps(const m_plyOps &rhs)	// Copy constructor
{
	this->m_verticies = rhs.m_verticies;
	this->m_elements = rhs.m_elements;

	this->m_minX = rhs.m_minX;
	this->m_minY = rhs.m_minY;
	this->m_minZ = rhs.m_minZ;

	this->m_maxX = rhs.m_maxX;
	this->m_maxY = rhs.m_maxY;
	this->m_maxZ = rhs.m_maxZ;

	this->m_deltaX = rhs.m_deltaX;
	this->m_deltaY = rhs.m_deltaY;
	this->m_deltaZ = rhs.m_deltaZ;

	this->m_centreX = rhs.m_centreX;
	this->m_centreY = rhs.m_centreY;
	this->m_centreZ = rhs.m_centreZ;

	this->m_maxExtent = 0.0f;

	this->m_totalProperties = rhs.m_totalProperties;
	this->m_x_propertyIndex = rhs.m_x_propertyIndex;
	this->m_y_propertyIndex = rhs.m_y_propertyIndex;
	this->m_z_propertyIndex = rhs.m_z_propertyIndex;
	this->m_normx_propertyIndex = rhs.m_normx_propertyIndex;
	this->m_normy_propertyIndex = rhs.m_normy_propertyIndex;
	this->m_normz_propertyIndex = rhs.m_normz_propertyIndex;
	this->m_red_propertyIndex = rhs.m_red_propertyIndex;
	this->m_green_propertyIndex = rhs.m_green_propertyIndex;
	this->m_blue_propertyIndex = rhs.m_blue_propertyIndex;

	this->m_bHadTextureCoordinatesInFile = rhs.m_bHadTextureCoordinatesInFile;
	this->m_bHadNormalsInFile = rhs.m_bHadNormalsInFile;
	this->m_tex0u_propertyIndex = rhs.m_tex0u_propertyIndex;
	this->m_tex0v_propertyIndex = rhs.m_tex0v_propertyIndex;
	this->m_tex1u_propertyIndex = rhs.m_tex1u_propertyIndex;
	this->m_tex1v_propertyIndex = rhs.m_tex1v_propertyIndex;
}

m_plyOps& m_plyOps::operator=(const m_plyOps &rhs)
{
	// Check for self-assignment *IMPORTANT*
	if (this == &rhs)	return *this;
	// Else...copy values
	this->m_verticies = rhs.m_verticies;
	this->m_elements = rhs.m_elements;

	this->m_minX = rhs.m_minX;
	this->m_minY = rhs.m_minY;
	this->m_minZ = rhs.m_minZ;

	this->m_maxX = rhs.m_maxX;
	this->m_maxY = rhs.m_maxY;
	this->m_maxZ = rhs.m_maxZ;

	this->m_deltaX = rhs.m_deltaX;
	this->m_deltaY = rhs.m_deltaY;
	this->m_deltaZ = rhs.m_deltaZ;

	this->m_centreX = rhs.m_centreX;
	this->m_centreY = rhs.m_centreY;
	this->m_centreZ = rhs.m_centreZ;

	this->m_totalProperties = this->m_totalProperties;
	this->m_x_propertyIndex = this->m_x_propertyIndex;
	this->m_y_propertyIndex = this->m_y_propertyIndex;
	this->m_z_propertyIndex = this->m_z_propertyIndex;
	this->m_normx_propertyIndex = this->m_normx_propertyIndex;
	this->m_normy_propertyIndex = this->m_normy_propertyIndex;
	this->m_normz_propertyIndex = this->m_normz_propertyIndex;
	this->m_red_propertyIndex = this->m_red_propertyIndex;
	this->m_green_propertyIndex = this->m_green_propertyIndex;
	this->m_blue_propertyIndex = this->m_blue_propertyIndex;

	return *this;
}

unsigned int m_plyOps::GetID(void)
{
	return this->m_ID;
}

bool m_plyOps::ReadPLYFileHeader(std::wstring fileName, std::wstring &error)
{
	error = L"OK";

	std::wifstream thePlyFile(fileName.c_str());
	if (!thePlyFile.is_open())
		{
			error = L"Can't open the file. Sorry it didn't work out.";
			return false;
		}

	std::wstring tempString;
	this->m_numberOfVertices = 0;
	this->m_numberOfElements = 0;

	try
	{
		bool bItsABunnyFile = true;
		// ply
		thePlyFile >> tempString;	if (tempString != L"ply")		{ bItsABunnyFile = false; error = L"No a ply file."; throw; }
		// format ascii 1.0
		thePlyFile >> tempString;	if (tempString != L"format")	{ bItsABunnyFile = false; throw; }
		thePlyFile >> tempString;	if (tempString != L"ascii")	{ bItsABunnyFile = false; error = L"Must be an ASCII ply file."; throw; }
		thePlyFile >> tempString;	if (tempString != L"1.0")		{ bItsABunnyFile = false; throw; }

		// Find the number of verticies...
		// Item after "vertex" is number of verticies
		while (thePlyFile >> tempString)
		{
			if (tempString == L"vertex")	break;
		}
		thePlyFile >> this->m_numberOfVertices;	// 453

		// Read the properties and note the index locations of them...
		int currentIndex = 0;
		while (true)
		{
			thePlyFile >> tempString;
			if (tempString == L"element") { break; /* on to next part of file */ }
			// property float x
			if (tempString == L"property")
			{
				thePlyFile >> tempString;
				if ((tempString == L"float") | (tempString == L"uchar"))
				{	// Figure out which index to set
					thePlyFile >> tempString;
					this->m_setIndexBasedOnPropertyName(currentIndex, tempString);
				}
				else
				{
					error = L"expected property type missing"; throw;
				} // if ( tempString == L"float")	
			}
			else
			{
				error = L"expected property"; throw;
			} // if ( tempString == L"property") 
			currentIndex++;
		}
		this->m_totalProperties = currentIndex;
		// 
		thePlyFile >> tempString;	if (tempString != L"face")	{ bItsABunnyFile = false; error = L"expected element face."; throw; }
		thePlyFile >> this->m_numberOfElements;

		while (thePlyFile >> tempString)
		{
			if (tempString == L"end_header")	break;
		}
	}
	catch (...)
	{
		error = L"Something went wrong while reading the file.";
		return false;
	}

	return true;
}

bool m_plyOps::OpenPLYFile(std::wstring fileName, std::wstring &error)
{
	error = L"OK";

	std::wifstream thePlyFile(fileName.c_str());
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}

	std::wstring tempString;
	this->m_numberOfVertices = 0;
	this->m_numberOfElements = 0;

	try
	{
		bool bItsABunnyFile = true;
		// ply
		thePlyFile >> tempString;	if (tempString != L"ply")		{ bItsABunnyFile = false; error = L"No a ply file."; throw; }
		// format ascii 1.0
		thePlyFile >> tempString;	if (tempString != L"format")	{ bItsABunnyFile = false; throw; }
		thePlyFile >> tempString;	if (tempString != L"ascii")	{ bItsABunnyFile = false; error = L"Must be an ASCII ply file."; throw; }
		thePlyFile >> tempString;	if (tempString != L"1.0")		{ bItsABunnyFile = false; throw; }

		// Find the number of verticies...
		// Item after "vertex" is number of verticies
		while (thePlyFile >> tempString)
		{
			if (tempString == L"vertex")	break;
		}
		thePlyFile >> this->m_numberOfVertices;	// 453

		// Read the properties and note the index locations of them...
		int currentIndex = 0;
		while (true)
		{
			thePlyFile >> tempString;
			if (tempString == L"element") { break; /* on to next part of file */ }
			// property float x
			if (tempString == L"property")
			{
				thePlyFile >> tempString;
				if ((tempString == L"float") | (tempString == L"uchar"))
				{	// Figure out which index to set
					thePlyFile >> tempString;
					this->m_setIndexBasedOnPropertyName(currentIndex, tempString);
				}
				else
				{
					error = L"expected property type missing"; throw;
				} // if ( tempString == L"float")	
			}
			else
			{
				error = L"expected property"; throw;
			} // if ( tempString == L"property") 
			currentIndex++;
		}
		this->m_totalProperties = currentIndex;
		// 
		thePlyFile >> tempString;	if (tempString != L"face")	{ bItsABunnyFile = false; error = L"expected element face."; throw; }
		thePlyFile >> this->m_numberOfElements;

		while (thePlyFile >> tempString)
		{
			if (tempString == L"end_header")	break;
		}

		// Now the real work begins...
		//... a bunch of vertices...
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 
		for (int vertexCount = 0; vertexCount != this->m_numberOfVertices; vertexCount++)
		{
			PLYVERTEX tempVertex;
			// Place the data into the array...
			// Read through the properties and place them into the appropriate part of the vertex...
			for (int curPropIndex = 0; curPropIndex != this->m_totalProperties; curPropIndex++)
			{
				std::wstring tempString;
				thePlyFile >> tempString;
				// Load this item based on index
				this->m_placeItemInAppropriateVertexLocation(tempString, curPropIndex, tempVertex);
			}

			this->m_verticies.push_back(tempVertex);
		}

		this->calcualteExtents();

		for (int elementCount = 0; elementCount != this->m_numberOfElements; elementCount++)
		{
			PLYELEMENT tempElement;
			thePlyFile >> tempString;	// 3 
			thePlyFile >> tempElement.vertex_index_1;
			thePlyFile >> tempElement.vertex_index_2;
			thePlyFile >> tempElement.vertex_index_3;
			this->m_elements.push_back(tempElement);
		}
	}
	catch (...)
	{
		error = L"Something went wrong while reading the file.";
		return false;
	}

	this->m_fileInformation.fileName = fileName;
	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_PLY;

	return true;
}

void m_plyOps::scaleVertices(float scaleFactor)
{
	std::vector<PLYVERTEX>::iterator itVertex = this->m_verticies.begin();
	for (; itVertex != this->m_verticies.end(); itVertex++)
	{
		itVertex->xyz.x *= scaleFactor;
		itVertex->xyz.y *= scaleFactor;
		itVertex->xyz.z *= scaleFactor;
	}
	this->calcualteExtents();
	return;
}

void m_plyOps::calcualteExtents(void)
{
	this->m_minX = this->m_verticies.begin()->xyz.x;
	this->m_maxX = this->m_verticies.begin()->xyz.z;
	this->m_minY = this->m_verticies.begin()->xyz.y;
	this->m_maxY = this->m_verticies.begin()->xyz.y;
	this->m_minZ = this->m_verticies.begin()->xyz.z;
	this->m_maxZ = this->m_verticies.begin()->xyz.z;

	this->m_deltaX = this->m_maxX - this->m_minX;
	this->m_deltaY = this->m_maxY - this->m_minY;
	this->m_deltaZ = this->m_maxZ - this->m_minZ;

	this->m_maxExtent = this->m_deltaX;
	if (this->m_deltaY > this->m_maxExtent)	this->m_maxExtent = this->m_deltaY;
	if (this->m_deltaZ > this->m_maxExtent)	this->m_maxExtent = this->m_deltaZ;

	std::vector<PLYVERTEX>::iterator itVertex = this->m_verticies.begin();
	for (; itVertex != this->m_verticies.end(); itVertex++)
	{
		// Check bounding box limits...
		if (itVertex->xyz.x < this->m_minX)	this->m_minX = itVertex->xyz.x;
		if (itVertex->xyz.x > this->m_maxX)	this->m_maxX = itVertex->xyz.x;
		if (itVertex->xyz.y < this->m_minY)	this->m_minY = itVertex->xyz.y;
		if (itVertex->xyz.y > this->m_maxY)	this->m_maxY = itVertex->xyz.y;
		if (itVertex->xyz.z < this->m_minZ)	this->m_minZ = itVertex->xyz.z;
		if (itVertex->xyz.z > this->m_maxZ)	this->m_maxZ = itVertex->xyz.z;
		this->m_deltaX = this->m_maxX - this->m_minX;
		this->m_deltaY = this->m_maxY - this->m_minY;
		this->m_deltaZ = this->m_maxZ - this->m_minZ;
		// 
		if (this->m_deltaX > this->m_maxExtent)	this->m_maxExtent = this->m_deltaX;
		if (this->m_deltaY > this->m_maxExtent)	this->m_maxExtent = this->m_deltaY;
		if (this->m_deltaZ > this->m_maxExtent)	this->m_maxExtent = this->m_deltaZ;
	}
	return;
}

void NormalizeVector(float &x, float &y, float &z)
{
	// Convert to double so there is no loss...
	double x2 = static_cast<double>(x)* static_cast<double>(x);
	double y2 = static_cast<double>(y)* static_cast<double>(y);
	double z2 = static_cast<double>(z)* static_cast<double>(z);
	double length = x2 + y2 + z2;
	if (length == 0)
	{
		x = y = z = 0.0f;
	}
	else
	{	// Multiply by reciprical as multiplies are faster than divides (usually)
		double reciprical = 1 / sqrt(length);
		x = static_cast<float>(static_cast<double>(x)* reciprical);
		y = static_cast<float>(static_cast<double>(y)* reciprical);
		z = static_cast<float>(static_cast<double>(z)* reciprical);
	}
}

// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns difference (xSub, ySub, zSub)
void VectorSubtract(float &x0, float &y0, float &z0,
	float &x1, float &y1, float &z1,
	float &xSub, float &ySub, float &zSub)
{
	xSub = x0 - x1;
	ySub = y0 - y1;
	zSub = z0 - z1;
}

// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns cross (xCross, yCross, zCross)
void CrossProduct(float &x0, float &y0, float &z0,
	float &x1, float &y1, float &z1,
	float &xCross, float &yCross, float &zCross)
{
	/* find cross-product between these vectors */
	xCross = y0 * z1 - z0 * y1;
	yCross = z0 * x1 - x0 * z1;
	zCross = x0 * y1 - y0 * x1;
}

void m_plyOps::normalizeTheModelBaby(void)
{

	std::vector<PLYELEMENT>::iterator itVecFace;
	for (itVecFace = this->m_elements.begin(); itVecFace != this->m_elements.end(); itVecFace++)
	{
		m_Vec3d vectorA, vectorB, vectorC;
		vectorA.x = this->m_verticies.at(itVecFace->vertex_index_1).xyz.x;
		vectorA.y = this->m_verticies.at(itVecFace->vertex_index_1).xyz.y;
		vectorA.z = this->m_verticies.at(itVecFace->vertex_index_1).xyz.z;
		vectorB.x = this->m_verticies.at(itVecFace->vertex_index_2).xyz.x;
		vectorB.y = this->m_verticies.at(itVecFace->vertex_index_2).xyz.y;
		vectorB.z = this->m_verticies.at(itVecFace->vertex_index_2).xyz.z;
		vectorC.x = this->m_verticies.at(itVecFace->vertex_index_3).xyz.x;
		vectorC.y = this->m_verticies.at(itVecFace->vertex_index_3).xyz.y;
		vectorC.z = this->m_verticies.at(itVecFace->vertex_index_3).xyz.z;
		// calculate the vectors for the cross...
		//SexyVector vecAB;// = vecB - vecA
		m_Vec3d vecAB;

		vecAB = vectorB - vectorA;

		//SexyVector vecBC;// = vecC - vecB
		m_Vec3d vecBC;

		vecBC = vectorC - vectorB;

		//SexyVector normal;
		m_Vec3d normal;

		normal.CrossProduct(vecAB, vecBC, normal);


		normal.Normalize();

		// Load the normals onto the verticies
		// NOTE: We are ADDING instead of OVERWRITING...
		this->m_verticies.at(itVecFace->vertex_index_1).nx += normal.x;  //****
		this->m_verticies.at(itVecFace->vertex_index_1).ny += normal.y;  //****
		this->m_verticies.at(itVecFace->vertex_index_1).nz += normal.z;  //****
		this->m_verticies.at(itVecFace->vertex_index_2).nx += normal.x;  //****
		this->m_verticies.at(itVecFace->vertex_index_2).ny += normal.y;  //****
		this->m_verticies.at(itVecFace->vertex_index_2).nz += normal.z;  //****
		this->m_verticies.at(itVecFace->vertex_index_3).nx += normal.x;  //****
		this->m_verticies.at(itVecFace->vertex_index_3).ny += normal.y;  //****
		this->m_verticies.at(itVecFace->vertex_index_3).nz += normal.z;  //****
	}
	// Now go through all the vertices and normalize (average) them...
	std::vector<PLYVERTEX>::iterator itVert;
	for (itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
	{
		::NormalizeVector(itVert->nx, itVert->ny, itVert->nz);
	}
}

void m_plyOps::normalizeTheModelBaby_Rock_n_Roll(void)
{
	// Go through all the faces, calculate the normal and 
	//	save (overwrite) the normals at that faces vertices.
	// LONG WAY version
	std::vector<PLYELEMENT>::iterator itVecFace;
	for (itVecFace = this->m_elements.begin(); itVecFace != this->m_elements.end(); itVecFace++)
	{
		m_Vec3d vectorA, vectorB, vectorC;
		vectorA.x = this->m_verticies.at(itVecFace->vertex_index_1).xyz.x;
		vectorA.y = this->m_verticies.at(itVecFace->vertex_index_1).xyz.y;
		vectorA.z = this->m_verticies.at(itVecFace->vertex_index_1).xyz.z;
		vectorB.x = this->m_verticies.at(itVecFace->vertex_index_2).xyz.x;
		vectorB.y = this->m_verticies.at(itVecFace->vertex_index_2).xyz.y;
		vectorB.z = this->m_verticies.at(itVecFace->vertex_index_2).xyz.z;
		vectorC.x = this->m_verticies.at(itVecFace->vertex_index_3).xyz.x;
		vectorC.y = this->m_verticies.at(itVecFace->vertex_index_3).xyz.y;
		vectorC.z = this->m_verticies.at(itVecFace->vertex_index_3).xyz.z;

		m_Vec3d vecAB;

		vecAB = vectorB - vectorA;

		m_Vec3d vecBC;

		vecBC = vectorC - vectorB;

		m_Vec3d normal;
		//::CrossProduct
		normal.CrossProduct(vecAB, vecBC, normal);

		//::NormalizeVector
		normal.Normalize();

		// Load the normals onto the verticies
		this->m_verticies.at(itVecFace->vertex_index_1).nx = normal.x;
		this->m_verticies.at(itVecFace->vertex_index_1).ny = normal.y;
		this->m_verticies.at(itVecFace->vertex_index_1).nz = normal.z;
		this->m_verticies.at(itVecFace->vertex_index_2).nx = normal.x;
		this->m_verticies.at(itVecFace->vertex_index_2).ny = normal.y;
		this->m_verticies.at(itVecFace->vertex_index_2).nz = normal.z;
		this->m_verticies.at(itVecFace->vertex_index_3).nx = normal.x;
		this->m_verticies.at(itVecFace->vertex_index_3).ny = normal.y;
		this->m_verticies.at(itVecFace->vertex_index_3).nz = normal.z;
	}
}

void m_plyOps::GenTextureCoordsSpherical(enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast)
{
	// determine the 'remaining' coordinate...
	bool xUsed = false;
	bool yUsed = false;
	bool zUsed = false;
	if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
	if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
	if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;

	std::vector<PLYVERTEX>::iterator itVert;
	for (itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
	{
		m_Vec3d xyz;
		if (basedOnNormals)
		{
			if (uBias == POSITIVE_X)		xyz.x = itVert->nx;
			else if (uBias == POSITIVE_Y)	xyz.x = itVert->ny;
			else if (uBias == POSITIVE_Z)	xyz.x = itVert->nz;

			if (vBias == POSITIVE_X)		xyz.y = itVert->nx;
			else if (vBias == POSITIVE_Y)	xyz.y = itVert->ny;
			else if (vBias == POSITIVE_Z)	xyz.y = itVert->nz;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = itVert->nx;
			if (!yUsed)	xyz.z = itVert->ny;
			if (!zUsed)	xyz.z = itVert->nz;
		}
		else
		{
			if (uBias == POSITIVE_X)		xyz.x = itVert->xyz.x;
			else if (uBias == POSITIVE_Y)	xyz.x = itVert->xyz.y;
			else if (uBias == POSITIVE_Z)	xyz.x = itVert->xyz.z;

			if (vBias == POSITIVE_X)		xyz.y = itVert->xyz.x;
			else if (vBias == POSITIVE_Y)	xyz.y = itVert->xyz.y;
			else if (vBias == POSITIVE_Z)	xyz.y = itVert->xyz.z;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = itVert->xyz.x;
			if (!yUsed)	xyz.z = itVert->xyz.y;
			if (!zUsed)	xyz.z = itVert->xyz.z;
		}

		xyz.Normalize();

		if (fast)
		{
			itVert->tex0u = ((xyz.x / 2.0f) + 0.5f) * scale;
			itVert->tex0v = ((xyz.y / 2.0f) + 0.5f) * scale;
		}
		else
		{
			itVert->tex0u = ((asin(xyz.x) / PI) + 0.5f) * scale;
			itVert->tex0v = ((asin(xyz.y) / PI) + 0.5f) * scale;
		}
		itVert->tex1u = itVert->tex0u;
		itVert->tex1v = itVert->tex0v;
	}
}

PLYVERTEX m_plyOps::getVertex_at(std::vector<PLYVERTEX>::size_type index)
{
	PLYVERTEX x = this->m_verticies.at(index);
	return x;
}

PLYELEMENT m_plyOps::getElement_at(std::vector<PLYELEMENT>::size_type index)
{
	PLYELEMENT x = this->m_elements.at(index);
	return x;
}

void m_plyOps::m_setIndexBasedOnPropertyName(int curIndex, std::wstring propName)
{
	if (propName == L"x")  { this->m_x_propertyIndex = curIndex; return; }
	if (propName == L"y")	{ this->m_y_propertyIndex = curIndex; return; }
	if (propName == L"z")	{ this->m_z_propertyIndex = curIndex; return; }

	// Added: March 14, 2012
	if (propName == L"nx") { this->m_normx_propertyIndex = curIndex; this->m_bHadNormalsInFile = true; return; }
	if (propName == L"ny") { this->m_normy_propertyIndex = curIndex; this->m_bHadNormalsInFile = true; return; }
	if (propName == L"nz") { this->m_normz_propertyIndex = curIndex; this->m_bHadNormalsInFile = true; return; }
	// End of Added

	if (propName == L"r")	{ this->m_red_propertyIndex = curIndex; return; }
	if (propName == L"red")	{ this->m_red_propertyIndex = curIndex; return; }
	if (propName == L"g")	{ this->m_green_propertyIndex = curIndex; return; }
	if (propName == L"green")	{ this->m_green_propertyIndex = curIndex; return; }
	if (propName == L"b")	{ this->m_blue_propertyIndex = curIndex; return; }
	if (propName == L"blue")	{ this->m_blue_propertyIndex = curIndex; return; }

	// Added: March 14, 2012
	// Textures... note: ply only defines one texture coord...
	if (propName == L"u")	{ this->m_tex0u_propertyIndex = curIndex; this->m_bHadTextureCoordinatesInFile = true; return; }
	if (propName == L"v") { this->m_tex0v_propertyIndex = curIndex; this->m_bHadTextureCoordinatesInFile = true; return; }
	// ...so these ones aren't in any 'normal' ply file
	if (propName == L"tex0u") { this->m_tex0u_propertyIndex = curIndex; this->m_bHadTextureCoordinatesInFile = true; return; }
	if (propName == L"tex0v") { this->m_tex0v_propertyIndex = curIndex; this->m_bHadTextureCoordinatesInFile = true; return; }
	if (propName == L"tex1u") { this->m_tex1u_propertyIndex = curIndex; this->m_bHadTextureCoordinatesInFile = true; return; }
	if (propName == L"tex1v") { this->m_tex1v_propertyIndex = curIndex; this->m_bHadTextureCoordinatesInFile = true; return; }
	// End of Added



}

bool m_plyOps::m_placeItemInAppropriateVertexLocation(std::wstring tempString, int propertyIndex, PLYVERTEX &tempVertex)
{
	// Convert val to float
	std::wstringstream ss;
	ss << tempString;
	float valAsFloat = 0.0f;
	ss >> valAsFloat;
	if (propertyIndex == this->m_x_propertyIndex) { tempVertex.xyz.x = valAsFloat; return true; }
	if (propertyIndex == this->m_y_propertyIndex) { tempVertex.xyz.y = valAsFloat; return true; }
	if (propertyIndex == this->m_z_propertyIndex) { tempVertex.xyz.z = valAsFloat; return true; }
	if (propertyIndex == this->m_red_propertyIndex) { tempVertex.red = valAsFloat; return true; }
	if (propertyIndex == this->m_green_propertyIndex) { tempVertex.green = valAsFloat; return true; }
	if (propertyIndex == this->m_blue_propertyIndex) { tempVertex.blue = valAsFloat; return true; }

	// Added March 14, 2012
	if (propertyIndex == this->m_normx_propertyIndex) { tempVertex.nx = valAsFloat; return true; }
	if (propertyIndex == this->m_normy_propertyIndex) { tempVertex.ny = valAsFloat; return true; }
	if (propertyIndex == this->m_normz_propertyIndex) { tempVertex.nz = valAsFloat; return true; }
	if (propertyIndex == this->m_tex0u_propertyIndex) { tempVertex.tex0u = valAsFloat; return true; }
	if (propertyIndex == this->m_tex0v_propertyIndex) { tempVertex.tex0v = valAsFloat; return true; }
	if (propertyIndex == this->m_tex1u_propertyIndex) { tempVertex.tex1u = valAsFloat; return true; }
	if (propertyIndex == this->m_tex1v_propertyIndex) { tempVertex.tex1v = valAsFloat; return true; }
	// End of Added

	// Didn't find a match
	return false;
}

// Added March 14, 2012
bool m_plyOps::bHadTextureCoordinatesInFile(void)
{
	return this->m_bHadTextureCoordinatesInFile;
}

bool m_plyOps::bHadNormalsInFile(void)
{
	return this->m_bHadNormalsInFile;
}

void m_plyOps::normlizeExistingNomrals(void)
{
	// Now go through all the vertices and normalize (average) them...
	std::vector<PLYVERTEX>::iterator itVert;
	for (itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
	{
		m_Vec3d normal(itVert->nx, itVert->ny, itVert->nz);
		normal.Normalize();
		itVert->nx = normal.x;
		itVert->ny = normal.y;
		itVert->nz = normal.z;
	}
}
// End of Added
int m_plyOps::GetNumberOfVerticies(void)
{
	//int x = static_cast<int>( this->m_verticies.size() );
	return this->m_numberOfVertices;
}

int m_plyOps::GetNumberOfElements(void)
{
	//int x = static_cast<int>( this->m_elements.size() );
	return this->m_numberOfElements;
}

float m_plyOps::getMaxX(void)
{
	return this->m_maxX;
}

float m_plyOps::getMinX(void)
{
	return this->m_minX;
}

float m_plyOps::getDeltaX(void)
{
	return this->m_deltaX;
}

float m_plyOps::getMaxY(void)
{
	return this->m_maxY;
}

float m_plyOps::getMinY(void)
{
	return this->m_minY;
}

float m_plyOps::getDeltaY(void)
{
	return this->m_deltaY;
}

float m_plyOps::getMaxZ(void)
{
	return this->m_maxZ;
}

float m_plyOps::getMinZ(void)
{
	return this->m_minZ;
}

float m_plyOps::getDeltaZ(void)
{
	return this->m_deltaZ;
}

float m_plyOps::getMaxExtent(void)
{
	return this->m_maxExtent;
}
