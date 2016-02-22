#pragma once
#ifndef _Vec3d_Ogl_
#define _Vec3d_Ogl_


class m_Vec3d
{

public:
	m_Vec3d() : x(0.0f), y(0.0f), z(0.0f) {};
	m_Vec3d(float x, float y, float z);
	m_Vec3d(const m_Vec3d &v);	// Copy constructor
	~m_Vec3d() {};
	float x; float y; float z;

	// These are used on vector types
	m_Vec3d& operator=(const m_Vec3d &v);
	m_Vec3d& operator+(const m_Vec3d &v);
	m_Vec3d& operator-(const m_Vec3d &v);
	m_Vec3d& operator+=(const m_Vec3d &v);
	m_Vec3d& operator-=(const m_Vec3d &v);
	m_Vec3d& operator*=(float scalar);	// Times a scalar
	m_Vec3d& operator/=(float scalar);	// Divided by a scalar
	m_Vec3d operator-();	// Negative on all directions

	// Helpful functions
	void Normalize(void);
	float Magnitude(void);
	float MagnitudeSquared(void);
	void Reverse(void);

	// Helpful static functions...
	static void Normalize(float &x, float &y, float &z);
	static void Normalize(m_Vec3d &v);
	static float Magnitude(const m_Vec3d &v);
	static float MagnitudeSquared(const m_Vec3d &v);
	static float Distance(const m_Vec3d &v1, const m_Vec3d &v2);
	static float DistanceSquared(const m_Vec3d &v1, const m_Vec3d &v2);

	static void Subtract(const float &x0, const float &y0, const float &z0,
		const float &x1, const float &y1, const float &z1,
		float &xSub, float &ySub, float &zSub);
	static void Subtract(const m_Vec3d &v1, const m_Vec3d &v2, m_Vec3d &vSub);
	static m_Vec3d Subtract(const m_Vec3d &v1, const m_Vec3d &v2);
	static m_Vec3d ScalarMultiply(const m_Vec3d &v, float s);
	static m_Vec3d ScalarDivide(const m_Vec3d &v, float s);

	static void CrossProduct(const float &x0, const float &y0, const float &z0,
		const float &x1, const float &y1, const float &z1,
		float &xCross, float &yCross, float &zCross);
	static void CrossProduct(const m_Vec3d v1, const m_Vec3d v2, m_Vec3d &vSub);

	static float DotProduct(const float &x1, const float &y1, const float &z1,
		const float &x2, const float &y2, const float &z2);
	static float DotProduct(const m_Vec3d &v1, const m_Vec3d &v2);
};



#endif