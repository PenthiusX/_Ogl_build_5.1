#pragma once

#include "IFactory.h"
#include "IMediator.h" 

#include <vector>

class m_Camera
{
public:
	m_Camera();
	~m_Camera();

	void SetMediator(IMediator* pTheMediator);

	m_Vec3d eye;
	m_Vec3d target;
	m_Vec3d up;

	// Follow at speed up to a certain distance
	void Follow(unsigned int ID_to_follow, float followSpeed,
		float minDistance, float minHeight);
	// Called in the "idle" or "simulation step"
	void Update(float deltaSeconds);

	bool bIsFollowing;
	unsigned int object_to_follow_ID;
	float minFollowDistance;
	float followSpeed;
	float minHeight;

	// Maybe you can fill these out if you can't sleep tonight...
	void MoveX(float delta);
	void MoveY(float delta);
	void MoveZ(float delta);


private:
	IMediator* m_pTheMediator;
};

