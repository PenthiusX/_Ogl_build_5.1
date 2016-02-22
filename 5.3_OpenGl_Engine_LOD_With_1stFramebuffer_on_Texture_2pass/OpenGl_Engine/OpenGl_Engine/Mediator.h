#ifndef _Mediator_
#define _Mediator_

#include "m_GameObject.h"
#include "m_Vec3d.h"
#include <vector>



class Mediator
{
public:

	Mediator();
	virtual ~Mediator();

	void setupGameObjects();



	unsigned int CreateObjectByType(std::string type, std::string filename);
	unsigned int CreateObjectByType(std::string type, m_Vec3d initPosition, std::string filename);
	unsigned int CreateObjectByType(std::string type, m_Vec3d initPosition, float initScale, std::string filename);
	
	 void get_Objects_ToRender(std::vector< m_GameObject* > &vecObjectsToRender);

	 bool UpdateObjectPositionByID(unsigned int ID, m_Vec3d newPosition);
     bool UpdateObjectScaleByID(unsigned int ID, float newScale);
	 bool UpdateObjectVelocityByID(unsigned int ID, m_Vec3d newVelocity);
	 bool UpdateObjectRotationByID(unsigned int ID, m_Vec3d newRotation);
	 bool UpdateColourByID(unsigned int ID, m_Vec3d newColour);
	 
	 bool UpdatePlyFileByID(unsigned int ID, std::string newPlyFileName);
	
	 std::vector<unsigned int> GetIDOfNearestObjects(m_Vec3d &location, float radius);
	 m_Vec3d GetPositionByID(unsigned int ID, m_Vec3d &position);

	//new
	unsigned int get_IDfromFileName(std::string filename);





private:

	std::vector<m_GameObject*> m_p_GameObjects;

	m_GameObject* Mediator::m_CreateObjectByType(std::string type, m_Vec3d initPosition, float initScale, std::string filename);
	m_GameObject* m_findObjectByID(int ID);
};

#endif
