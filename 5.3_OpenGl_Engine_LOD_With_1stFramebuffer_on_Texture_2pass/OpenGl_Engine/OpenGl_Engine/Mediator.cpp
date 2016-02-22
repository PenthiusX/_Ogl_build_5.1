#include "Mediator.h"
#include <math.h>		// for the sqrt()
#include <iostream>
#include "m_global.h"



Mediator::Mediator()
{
	return;
}

Mediator::~Mediator()
{
	// Delete all the objects in Distructor
	for (int index = 0; index != static_cast<int>(this->m_p_GameObjects.size()); index++)
	{
		delete this->m_p_GameObjects[index];
		this->m_p_GameObjects[index] = 0;		// Not really needed, but what yolo.
	}

	this->m_p_GameObjects.clear();	// Bye, bye
	return;
}

void Mediator::get_Objects_ToRender(std::vector<m_GameObject*> &vecObjectsToRender)// Called in the DisplayCallback
{
	vecObjectsToRender.clear();
	for (int index = 0; index != (int)this->m_p_GameObjects.size(); index++)
	{
		vecObjectsToRender.push_back(this->m_p_GameObjects[index]);
	}

	return;
}

m_GameObject* Mediator::m_CreateObjectByType(std::string type, m_Vec3d initPosition, float initScale, std::string filename)
{
	if (type == "PLY")
	{
		m_GameObject* p_TempObject = new m_GameObject();	
		p_TempObject->FileName = filename;
		p_TempObject->scale = initScale;
		p_TempObject->position = initPosition;
		p_TempObject->set_TextureNo(0);
		return p_TempObject;
	}
	else if (type == "FBX")
	{
		m_GameObject* p_TempObject = new m_GameObject();		
		p_TempObject->FileName = filename;//Needs to come from FBX file
		p_TempObject->scale = initScale/20;//--------^^
		p_TempObject->position = initPosition;//
		p_TempObject->rotation = m_Vec3d(982.348f,0.1f,0.1f);
		p_TempObject->set_TextureNo(1);
		return p_TempObject;
	}
	return 0;
}

unsigned int Mediator::CreateObjectByType(std::string type , std::string filename)
{
	return this->CreateObjectByType(type, m_Vec3d(0.0f, 0.0f, 0.0f), 1.0f, filename);//by default setting the Initposition and scale
}

// From IFactory: This is the "Factory Method" of the Abstract Factory
unsigned int Mediator::CreateObjectByType(std::string type, m_Vec3d initPosition, float initScale, std::string filename)
{
	m_GameObject* p_TempObject = this->m_CreateObjectByType(type, initPosition, initScale, filename);
	if (p_TempObject == 0)
	{
		return 0;
	}
	this->m_p_GameObjects.push_back(p_TempObject);
	return p_TempObject->getID();
}

unsigned int m_CreateObjectByType(std::string type, m_Vec3d initPosition);
unsigned int m_CreateObjectByType(std::string type, m_Vec3d initPosition, float initScale);


bool Mediator::UpdateObjectPositionByID(unsigned int ID, m_Vec3d newPosition)
{
	m_GameObject* pTheGO = this->m_findObjectByID(ID);
	if (pTheGO == 0)
	{
		return false;
	}
	pTheGO->position = newPosition;
	return true;
}

bool Mediator::UpdateObjectRotationByID(unsigned int ID, m_Vec3d newRotation)
{
	m_GameObject* pTheGO = this->m_findObjectByID(ID);
	if (pTheGO == 0)
	{
		std::cout << "No GameObjects Exist(Rot)";
		return false;
	}
	pTheGO->rotation = newRotation;
	return true;
}


bool Mediator::UpdateObjectScaleByID(unsigned int ID, float newScale)
{
	m_GameObject* pTheGO = this->m_findObjectByID(ID);
	if (pTheGO == 0)
	{
		std::cout << "No GameObjects Exist(Scale)";
		return false;
	}
	pTheGO->scale = newScale;
	return true;
}

bool Mediator::UpdateObjectVelocityByID(unsigned int ID, m_Vec3d newVelocity)
{
	m_GameObject* pTheGO = this->m_findObjectByID(ID);
	if (pTheGO == 0)
	{
		std::cout << "No GameObjects Exist(Vel)";
		return false;
	}
	pTheGO->velocity = newVelocity;
	return true;
}

bool Mediator::UpdatePlyFileByID(unsigned int ID, std::string newPlyFileName)
{
	m_GameObject* pTheGO = this->m_findObjectByID(ID);
	if (pTheGO == 0)
	{
		std::cout << "No GameObjects Exist(ID)";
		return false;
	}

	pTheGO->FileName = newPlyFileName;
	return true;
}

bool Mediator::UpdateColourByID(unsigned int ID, m_Vec3d newColour)
{
	m_GameObject* t_gameObj = this->m_findObjectByID(ID);
	if (t_gameObj == 0)
	{
	std::cout << "No GameObjects Exist (color)";
		return false;
	}
	t_gameObj->colour = newColour;
	return true;
}


m_Vec3d Mediator::GetPositionByID(unsigned int ID, m_Vec3d &position)
{
	m_GameObject* t_gameObj = this->m_findObjectByID(ID);
	if (t_gameObj == 0)
	{
		std::cout << "no GObj in vec";
	}
	position = t_gameObj->position;

	return position;
}

m_GameObject* Mediator::m_findObjectByID(int ID)
{
	for (int index = 0; index != static_cast<int>(this->m_p_GameObjects.size()); index++)
	{
		if (this->m_p_GameObjects[index]->getID() == ID)
		{
			return this->m_p_GameObjects[index];	
		}
	}
	return 0;
}

float getDistanceBetweenPoints(m_Vec3d point1, m_Vec3d point2)//Calcualate distance between two 3d Points
{
	float deltaXsquared = (point2.x - point1.x) * (point2.x - point1.x); 
	float deltaYsquared = (point2.y - point1.y) * (point2.y - point1.y);
	float deltaZsquared = (point2.z - point1.z) * (point2.z - point1.z);

	float distance = sqrt(deltaXsquared + deltaYsquared + deltaZsquared);

	return distance;
}

std::vector<unsigned int> Mediator::GetIDOfNearestObjects(m_Vec3d &location, float radius)
{
	std::vector<unsigned int> vecFoundObjects;

	for (int index = 0; index != (int)this->m_p_GameObjects.size(); index++)
	{
		float distance = ::getDistanceBetweenPoints(location, this->m_p_GameObjects[index]->position);
		if (distance <= radius)
		{
			vecFoundObjects.push_back(this->m_p_GameObjects[index]->getID());
		}
	}

	return vecFoundObjects;
}

unsigned int Mediator::get_IDfromFileName(std::string filename)
{
	for (int index = 0; index < m_p_GameObjects.size(); index++)
	{

		if (m_p_GameObjects[index]->FileName == filename)
		{
			return m_p_GameObjects[index]->getID();
		}
		else
		{
			std::cout << "No such obj in list";
		}
	}
	
}