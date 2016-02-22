#include "m_GameObject.h"

m_GameObject::m_GameObject()
{
	this->m_Init();
	this->m_pMediator = 0;
	return;
}
m_GameObject::~m_GameObject()
{
	return;
}

static const unsigned int FIRSTGAMEOBJECTID = 1000;
unsigned int m_GameObject::m_nextID = FIRSTGAMEOBJECTID;

// Called by the constructors
void m_GameObject::m_Init(void)
{
	// Assign ID here
	this->m_ID = this->m_nextID;
	this->m_nextID++;	// Increment

	this->scale = 1.0f;

	this->name = "no name";
	this->FileName = "";

	this->Wireframe = false;

	this->specularShininess = 1.0f;		// "Meh" shiny
	this->specularStrength = 1.0f;		// Not shiny

	return;
}

void m_GameObject::setMediator(IMediator* pMediator)
{
	// The mediator should only be set at construction, so if something
	//	tries to set it again, we prevent it
	if (this->m_pMediator == 0)
	{	// Hasn't been set, so assume creator is calling it
		this->m_pMediator = pMediator;
	}
	return;
}

void m_GameObject::Update(float deltaTime)
{
	// Insert behaviour code in here
	int sexyBreakpoint = 0;

	return;
}

unsigned int m_GameObject::getID(void)
{
	return this->m_ID;
}

//Textures 
void m_GameObject::set_TextureNo(unsigned int textureNO)
{
	this->textureNO = textureNO;
}
unsigned int m_GameObject::get_TextureNo()
{
	return this->textureNO;
}