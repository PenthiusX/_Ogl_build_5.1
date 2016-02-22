#pragma once
#ifndef _verttype_
#define _verttype_


struct CVertex_fXYZ_fRGB
{
	CVertex_fXYZ_fRGB();	// Yes, structs can have constructors. It's true.
	float Position[4];
	float Color[4];

	//For normals
	float x, y, z, w;
};

struct CVertex_fXYZ_fRGB_fNxNyNz  //With  normals ?
{
	CVertex_fXYZ_fRGB_fNxNyNz();
	float Position[4];
	float Color[4];
	float Normal[4];
};

struct CVertex_fXYZW
{
	CVertex_fXYZW() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};
	void SetParams(float cx, float xy, float cz);
	float x, y, z, w;
};

#endif
