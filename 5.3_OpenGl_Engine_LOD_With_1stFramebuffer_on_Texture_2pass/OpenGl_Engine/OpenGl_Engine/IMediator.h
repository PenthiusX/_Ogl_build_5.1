#ifndef _IMediator_HG_
#define _IMediator_HG_


#include "m_Vec3d.h"
#include <vector>

//
class IMediator
{
public:
	virtual ~IMediator() {};		// need a destructor on interfaces

	virtual std::vector<unsigned int> GetIDOfNearestObjects(m_Vec3d &location, float radius) = 0;
	virtual bool GetPositionByID(unsigned int ID, m_Vec3d &position) = 0;
};

#endif
