#include "m_global.h"
#include <iostream>		
#include <sstream>		

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <vector>
#include <fstream>
#include "m_VertexTypes.h"
#include "m_timerFunction.h"
#include <map>

static const double PI = 3.14159265358979323846;

ModelDataINFo tempModelDataINFo;
m_timerFunction g_simTimer;

//shader Unifrorm Variables
void AttachShader(void);

//From Misc Functions
void setup_Lights_globalFunct();
void setup_Camera_GlobalFunct();




int main(int argc, char* argv[])
{
	screen_Width = 1280;
	screen_Height = 720;
	::OpenGL_Initialize(argc, argv, screen_Width, screen_Height);
//-----------------------------------------------------------------------------------
	AttachShader();

	//------------------------------->
	SetShaderUniformVariables();
	//------------------------------->
//||||||||||||||||||||||||||||||||||||||||||FILE MODEL LOADING|||||||||||||||||||||||||||||||||||||||||||||||	
																								         //||										                                                 //||
	std::vector<std::string> ModelsToLoad_vector;
	//ModelsToLoad_vector.push_back("ply/cubefrme.ply");
	//ModelsToLoad_vector.push_back("ply/Seafloor2_onlyXYZ.ply");
	//ModelsToLoad_vector.push_back("FBX/ViperMkVII_ASCII.fbx");
	//ModelsToLoad_vector.push_back("FBX/1.fbx");
	//ModelsToLoad_vector.push_back("FBX/banff.fbx");
	//ModelsToLoad_vector.push_back("FBX/plane.fbx");
	ModelsToLoad_vector.push_back("FBX/Lod/backplane.fbx");
	ModelsToLoad_vector.push_back("FBX/Lod/Kn1.fbx");
	ModelsToLoad_vector.push_back("FBX/Lod/Kn2.fbx");
	

	p_g_ModelLoader = new m_ModelLoadManager();
	//------------------------------------
	if (!p_g_ModelLoader->LoadModels(ModelsToLoad_vector))//Loadin models into buffers as per file type
		{
			std::cout << "Can't load one or more models. Sorry it didn't work out." << std::endl;
			return -1;
		}
	//-------------------------------------
	unsigned int ID;
	p_g_MediatorObj = new Mediator();
	for (int index = 0; index < ModelsToLoad_vector.size(); index++)
	{
		if (ModelsToLoad_vector[index].substr(ModelsToLoad_vector[index].length() - 3) == "ply")
		{
			ID = p_g_MediatorObj->CreateObjectByType("PLY", ModelsToLoad_vector[index]);
				p_g_MediatorObj->UpdateObjectPositionByID(ID, m_Vec3d(float(index), 1.0f, 0.0f));
		}
		else if (ModelsToLoad_vector[index].substr(ModelsToLoad_vector[index].length() - 3) == "fbx")
		{
			ID = p_g_MediatorObj->CreateObjectByType("FBX", ModelsToLoad_vector[index]);
		}
	}
																										  //||
																										  //||
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	p_g_MediatorObj->get_Objects_ToRender(p_g_DebugObject_Vector);
	//p_g_DebugObject_Vector[0]->position = m_Vec3d(-1.0, 0.0f, 0.0f);
	//// p_g_DebugObject_Vector[0]->rotation = m_Vec3d(982.348, 0.01 , 0.01);
	//p_g_DebugObject_Vector[0]->scale = 1.1;
	
	for (int index = 0; index < 30; index++)
	{
		g_GameOBject_Vector_fr_LOD.push_back(*p_g_DebugObject_Vector[0]);
	}
	for (int index = 2; index < 10; index++)
	{
		g_GameOBject_Vector_fr_LOD[index].position.z = g_GameOBject_Vector_fr_LOD[index - 1].position.z + 2;
	}
	for (int index = 11; index < 20; index++)
	{
		g_GameOBject_Vector_fr_LOD[index].position.x = 2.0f;
		g_GameOBject_Vector_fr_LOD[index].position.z = g_GameOBject_Vector_fr_LOD[index - 1].position.z + 2;
	}
	for (int index = 21; index < 30; index++)
	{
		g_GameOBject_Vector_fr_LOD[index].position.x = -2.0f;
		g_GameOBject_Vector_fr_LOD[index].position.z = g_GameOBject_Vector_fr_LOD[index - 1].position.z + 2;
	}

	g_GameOBject_Vector_fr_LOD[0].name = "FBX/backplane.fbx";
	g_GameOBject_Vector_fr_LOD[0].position = m_Vec3d(0.0,2.0,-3.0);
	//g_GameOBject_Vector_fr_LOD[0].scale = 1.0;
	g_GameOBject_Vector_fr_LOD[0].set_TextureNo(2);
	//g_GameOBject_Vector_fr_LOD[0].rotation = m_Vec3d(982.348, 0.01, 0.01);
//---------------------------------------------------------------------------
	setup_Lights_globalFunct(); //----gobalfunction ,needs to be reorganised
//---------------------------------------------------------------------------
	p_g_CameraObj = new m_Camera();
	p_g_CameraObj->SetMediator((IMediator*)p_g_MediatorObj);
	p_g_CameraObj->eye.x = 0.0f;		
	p_g_CameraObj->eye.y = 2.0f;		
	p_g_CameraObj->eye.z = 4.0f;		
	
	p_g_CameraObj->target = p_g_DebugObject_Vector[0]->position;
	p_g_CameraObj->up.x = 0.0f;
	p_g_CameraObj->up.y = 1.0f;			//The up direction	

	g_simTimer.Reset();
	g_simTimer.Start();	
//----------------------
	glutMainLoop();
//----------------------	
	exit(EXIT_SUCCESS);
}

static const float g_MAXIMUM_TIME_STEP = 0.1f;

float CalcDistanceBetweenPoints(m_Vec3d point1, m_Vec3d point2)//Calcualate distance between two 3d Points
{
	float deltaXsquared = (point2.x - point1.x) * (point2.x - point1.x);
	float deltaYsquared = (point2.y - point1.y) * (point2.y - point1.y);
	float deltaZsquared = (point2.z - point1.z) * (point2.z - point1.z);

	float distance = sqrt(deltaXsquared + deltaYsquared + deltaZsquared);

	return distance;
}

void IdleFunction(void)
{
	g_simTimer.Stop();
	float deltaTime = ::g_simTimer.GetElapsedSeconds();
	if (deltaTime > g_MAXIMUM_TIME_STEP)
	{deltaTime = g_MAXIMUM_TIME_STEP;}
	g_simTimer.Start();

	std::vector< m_GameObject* > loc_p_gameObj_vector;
	p_g_MediatorObj->get_Objects_ToRender(loc_p_gameObj_vector);


//----------------FOR LOD--------------------------------------
	g_GameOBject_Vector_fr_LOD[0].rotation = m_Vec3d(5.348, 0.01, 0.01);
	for (int index = 1; index < g_GameOBject_Vector_fr_LOD.size(); index++)
	{
		float distance = CalcDistanceBetweenPoints(p_g_CameraObj->eye, g_GameOBject_Vector_fr_LOD[index].position);
		if (distance < 10)
		{
			g_GameOBject_Vector_fr_LOD[index].FileName = "FBX/Lod/Kn1.fbx";
			//g_GameOBject_Vector_fr_LOD[index].FileName = "FBX/1.fbx";
			g_GameOBject_Vector_fr_LOD[index].scale = 1.1f;	
			g_GameOBject_Vector_fr_LOD[index].set_TextureNo(2);
		}
		if (distance > 10)
		{
			g_GameOBject_Vector_fr_LOD[index].FileName = "FBX/Lod/Kn2.fbx";
			//g_GameOBject_Vector_fr_LOD[index].FileName = "ply/cubefrme.ply";
			g_GameOBject_Vector_fr_LOD[index].scale = 1.1f;
			g_GameOBject_Vector_fr_LOD[index].set_TextureNo(2);
		}
	}
//------------------------------------------------------------



		for (int index = 0; index != static_cast<int>(loc_p_gameObj_vector.size()); index++)
				{
					loc_p_gameObj_vector[index]->position.x += (loc_p_gameObj_vector[index]->velocity.x * deltaTime);
					loc_p_gameObj_vector[index]->position.y += (loc_p_gameObj_vector[index]->velocity.y * deltaTime);
					loc_p_gameObj_vector[index]->position.z += (loc_p_gameObj_vector[index]->velocity.z * deltaTime);
				}
		for (int index = 0; index != static_cast<int>(loc_p_gameObj_vector.size()); index++)
				{
					loc_p_gameObj_vector[index]->Update(deltaTime);
				}

		p_g_CameraObj->Update(deltaTime);
		glutPostRedisplay();
		return;
}



