#include "m_Vec3d.h"
#include <math.h>

m_Vec3d::m_Vec3d(float x, float y, float z)
{
	this->x = x; this->y = y; this->z = z;
	return;
}

m_Vec3d::m_Vec3d(const m_Vec3d &v)
{
	this->x = v.x; this->y = v.y; this->z = v.z;
	return;
}

m_Vec3d& m_Vec3d::operator=(const m_Vec3d &v)
{
	// Check for self-assignment *IMPORTANT*
	if (this == &v)	return *this;
	// Else...copy values
	this->x = v.x; this->y = v.y; this->z = v.z;
	return *this;
}

m_Vec3d& m_Vec3d::operator+(const m_Vec3d &v)
{
	return m_Vec3d(*this) += v;
}

m_Vec3d& m_Vec3d::operator-(const m_Vec3d &v)
{
	return m_Vec3d(*this) -= v;
}

m_Vec3d& m_Vec3d::operator+=(const m_Vec3d &v)
{
	this->x += v.x; this->y += v.y; this->z += v.z;
	return *this;
}

m_Vec3d& m_Vec3d::operator-=(const m_Vec3d &v)
{
	this->x -= v.x; this->y -= v.y; this->z -= v.z;
	return *this;
}

// Times a scalar
m_Vec3d& m_Vec3d::operator*=(float scalar)
{
	this->x *= scalar; this->y *= scalar; this->z *= scalar;
	return *this;
}

// Divided by a scalar
m_Vec3d& m_Vec3d::operator/=(float scalar)
{
	this->x /= scalar; this->y /= scalar; this->z /= scalar;
	return *this;
}

m_Vec3d m_Vec3d::operator-()	// Negative on all directions
{
	return m_Vec3d(-x, -y, -z);
}

/*static*/ // Subtracts one vector from another and returns in Sub (by reference)
void m_Vec3d::Subtract(const float &x0, const float &y0, const float &z0,
	const float &x1, const float &y1, const float &z1,
	float &xSub, float &ySub, float &zSub)
{
	xSub = x0 - x1; ySub = y0 - y1; zSub = z0 - z1;
	return;
}

/*static*/ // Subtracts one vector from another and returns in Sub (by reference)
void m_Vec3d::Subtract(const m_Vec3d &v1, const m_Vec3d &v2, m_Vec3d &vSub)
{
	m_Vec3d::Subtract(v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z,
		vSub.x, vSub.y, vSub.z);
	return;
}

/*static*/ // Subtracts returns one vector from another 
m_Vec3d m_Vec3d::Subtract(const m_Vec3d &v1, const m_Vec3d &v2)
{
	m_Vec3d vTemp;
	m_Vec3d::Subtract(v1, v2, vTemp);
	return vTemp;
}

/*static*/
m_Vec3d m_Vec3d::ScalarMultiply(const m_Vec3d &v, float s)
{
	m_Vec3d vTemp(v);
	vTemp *= s;
	return vTemp;
}

/*static*/
m_Vec3d m_Vec3d::ScalarDivide(const m_Vec3d &v, float s)
{
	m_Vec3d vTemp(v);
	vTemp /= s;
	return vTemp;
}

/*static*/
// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns cross (xCross, yCross, zCross)
void m_Vec3d::CrossProduct(const float &x0, const float &y0, const float &z0,
	const float &x1, const float &y1, const float &z1,
	float &xCross, float &yCross, float &zCross)
{
	/* find cross-product between these vectors */
	xCross = y0 * z1 - z0 * y1;
	yCross = z0 * x1 - x0 * z1;
	zCross = x0 * y1 - y0 * x1;
	return;
}

/*static*/
void m_Vec3d::CrossProduct(const m_Vec3d v1, const m_Vec3d v2, m_Vec3d &vSub)
{
	m_Vec3d::CrossProduct(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vSub.x, vSub.y, vSub.z);
	return;
}

/*static*/
float m_Vec3d::DotProduct(const float &x1, const float &y1, const float &z1,
	const float &x2, const float &y2, const float &z2)
{
	float dot = x1 * x2 + y1 * y2 + z1 * z2;
	return dot;
}

/*static*/
float m_Vec3d::DotProduct(const m_Vec3d &v1, const m_Vec3d &v2)
{
	return DotProduct(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
}

/*static*/
// Set vector to unit size (length of 1.0)
void m_Vec3d::Normalize(float &x, float &y, float &z)
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
	return;
}

/*static*/
void m_Vec3d::Normalize(m_Vec3d &v)
{
	m_Vec3d::Normalize(v.x, v.y, v.z);
	return;
}

void m_Vec3d::Normalize(void)
{
	m_Vec3d::Normalize(*this);
	return;
}

/*static*/
float m_Vec3d::Magnitude(const m_Vec3d &v)
{
	return static_cast<float>(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
}

float m_Vec3d::Magnitude(void)
{
	return m_Vec3d::Magnitude(*this);
}
/*static*/
// Same as magnatude, but without the square root...
float m_Vec3d::MagnitudeSquared(const m_Vec3d &v)
{
	return static_cast<float>(v.x*v.x + v.y*v.y + v.z*v.z);

}

float m_Vec3d::MagnitudeSquared(void)
{
	return m_Vec3d::Magnitude(*this);
}

/*static*/
float m_Vec3d::Distance(const m_Vec3d &v1, const m_Vec3d &v2)
{
	double deltaX = static_cast<double>(v1.x - v2.x);
	double deltaY = static_cast<double>(v1.y - v2.y);
	double deltaZ = static_cast<double>(v1.z - v2.z);
	return static_cast<float>(sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ));
}

/*static*/
// Same as distance, but without the square root...
float m_Vec3d::DistanceSquared(const m_Vec3d &v1, const m_Vec3d &v2)
{
	double deltaX = static_cast<double>(v1.x - v2.x);
	double deltaY = static_cast<double>(v1.y - v2.y);
	double deltaZ = static_cast<double>(v1.z - v2.z);
	return static_cast<float>(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
}

void m_Vec3d::Reverse(void)
{
	this->x = -(this->x);
	this->y = -(this->y);
	this->z = -(this->z);
	return;
}