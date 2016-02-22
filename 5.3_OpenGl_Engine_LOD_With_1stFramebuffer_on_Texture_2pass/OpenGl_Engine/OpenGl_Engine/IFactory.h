#ifndef _IFactory_HG_
#define _IFactory_HG_

#include "m_GameObject.h"	// (also has string)
#include <vector>	

class IFactory		// Things that can create object see this
{
public:
	virtual ~IFactory() {};		// implement a desructor for this
	// Returns a "handle" (aka an ID) of the object (NOT a pointer, though)
	virtual unsigned int CreateObjectByType(std::string type) = 0;
	virtual unsigned int CreateObjectByType(std::string type, m_Vec3d initPosition) = 0;
	virtual unsigned int CreateObjectByType(std::string type, m_Vec3d initPosition, float initScale) = 0;
	// Added
	//virtual unsigned int CreateObject(CMessage theCreationParams) = 0;
	//
	virtual bool UpdateObjectPositionByID(unsigned int ID, m_Vec3d newPosition) = 0;
	virtual bool UpdateObjectScaleByID(unsigned int ID, float newScale) = 0;
	virtual bool UpdateObjectVelocityByID(unsigned int ID, m_Vec3d newVelocity) = 0;
	virtual bool UpdateObjectRotationByID(unsigned int ID, m_Vec3d newRotation) = 0;
	virtual bool UpdatePlyFileByID(unsigned int ID, std::string newPlyFileName) = 0;
	// ADDED on September 29, 2014
	virtual bool UpdateColourByID(unsigned int ID, m_Vec3d newColour) = 0;

	// Called by the renderer
	virtual void get_Objects_ToRender(std::vector< m_Vec3d* > &vecObjectsToRender) = 0;
	// This is called by the physics updater
	virtual void getPhysicsObjects(std::vector< m_Vec3d* > &vecObjectsToUpdate) = 0;
};

#endif
