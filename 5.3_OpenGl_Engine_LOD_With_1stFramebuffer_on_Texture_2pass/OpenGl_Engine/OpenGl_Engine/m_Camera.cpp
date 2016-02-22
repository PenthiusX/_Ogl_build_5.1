#include "m_Camera.h"
#include <math.h>		// For sqrt()


m_Camera::m_Camera()
{
	this->bIsFollowing = false;
	this->m_pTheMediator = 0;		// If mediator == 0, don't use it
	return;
}
m_Camera::~m_Camera()
{
	return;
}


void m_Camera::SetMediator(IMediator* pTheMediator)
{
	this->m_pTheMediator = pTheMediator;
	return;
}

void m_Camera::Follow(unsigned int ID_to_follow, float rhs_followSpeed,
	float rhs_minDistance, float rhs_minHeight)
{
	this->object_to_follow_ID = ID_to_follow;	// Follow this game object
	this->followSpeed = rhs_followSpeed;
	this->minFollowDistance = rhs_minDistance;
	this->bIsFollowing = true;
	this->minHeight = rhs_minHeight;
	return;
}

// distance = sqrt( deltaX^2 + deltaY^2 + dealtZ^2 )
float calcDistance(m_Vec3d me, m_Vec3d them)
{
	float deltaX = them.x - me.x;	float deltaY = them.y - me.y;	float deltaZ = them.z - me.z;

	float deltaXSquared = deltaX * deltaX;	float deltaYSquared = deltaY * deltaY;
	float deltaZSquared = deltaZ * deltaZ;

	// sqrt() is in the "math.h" file (a standard file in C)
	float distance = sqrt(deltaXSquared + deltaYSquared + deltaZSquared);
	return distance;
}

//#include <iostream>

void m_Camera::Update(float deltaSeconds)
{
	if (!this->bIsFollowing)
	{
		return;
	}

	//std::cout << "Camera XYZ: " << this->eye.x << " " << this->eye.y << " " << this->eye.z << std::endl;

	// Find out where are target is...
	m_Vec3d targetLocation;
	if (!this->m_pTheMediator->GetPositionByID(this->object_to_follow_ID, targetLocation))
	{	// No target with that ID, yo!
		this->bIsFollowing = false;
		return;
	}

	// Point at target
	this->target = targetLocation;

	// See how far away from the object we are.
	float distanceToTarget = calcDistance(this->eye, this->target);

	if (distanceToTarget > this->minFollowDistance)
	{
		// Move towards the object at speed
		float changeThisStep = deltaSeconds * this->followSpeed;

		if (abs(distanceToTarget - this->minFollowDistance) < 1.0f)
		{
			// Scale based on this value
			changeThisStep = changeThisStep * abs(distanceToTarget - this->minFollowDistance);
		}


		if (this->eye.x < this->target.x)	{ this->eye.x += changeThisStep; }
		else if (this->eye.x > this->target.x)	{ this->eye.x -= changeThisStep; }

		// Calculate min height ABOVE the target
		float targetHeight = this->target.y + this->minHeight;

		if (this->eye.y < targetHeight)	{ this->eye.y += changeThisStep; }
		else if (this->eye.y > targetHeight)	{ this->eye.y -= changeThisStep; }


		if (this->eye.z < this->target.z)	{ this->eye.z += changeThisStep; }
		else if (this->eye.z > this->target.z)	{ this->eye.z -= changeThisStep; }
	}
	return;
}

