#ifndef _m_Vector4f_HG_
#define _m_Vector4f_HG_

class m_Vector4f
{
public:
	m_Vector4f() : x(0.0f), y(0.0f), z(0.0f), a(1.0f) {};
	m_Vector4f(float x, float y, float z, float a) : x(x), y(y), z(z), a(a) {};
	~m_Vector4f() {};
	float x, y, z, a;
};

#endif
