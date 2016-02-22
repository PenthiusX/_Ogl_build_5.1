#pragma once
#ifndef _m_GameObject_
#define _m_GameObject_

#include <string>

#include "m_Vec3d.h"
#include "m_Vector4f.h"
#include "IMediator.h"

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> // glm::mat4

class m_GameObject
{
public:
	m_GameObject();
	virtual ~m_GameObject();

	m_Vec3d position;
	m_Vec3d rotation;
	m_Vec3d velocity;
	m_Vec3d acceleration;
	float scale;

	bool Wireframe;
	bool WIsVisible;
	m_Vector4f debugColour;

	std::string name;			
	std::string FileName;	
	m_Vec3d colour;

	//Specularity
	float specularShininess;
	float specularStrength;

	// Only called by the IFactory object
	void setMediator(IMediator* pMediator);

	virtual void Update(float deltaTime);

	unsigned int getID(void);

	//For textures
	void set_TextureNo(unsigned int textureNO);
	unsigned int get_TextureNo();


protected:

	void m_Init(void);	
	IMediator* m_pMediator;
	// Used to figure out what "state" the object is in
	std::string m_State;

	unsigned int m_ID;	
	static unsigned int m_nextID;
	
	
	
	unsigned int textureNO;//-----This states what Texture to apply on the GameoBject;



};


#endif
