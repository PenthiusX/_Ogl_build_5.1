#include "m_global.h"

void setup_Lights_globalFunct()
{

	glm::vec3 gvecObj;
	glm::vec3 colorobj;

	gvecObj.x = 0.0, gvecObj.y = 2.0, gvecObj.z = 0.0;
	colorobj.x = 0.0, colorobj.y = 1.0, colorobj.z = 0.0;

	for (int lightCount = 0; lightCount < 3; lightCount++)
	{
		::g_ShaderUniformVariables.Lights[lightCount].isEnabled = GL_TRUE;
		::g_ShaderUniformVariables.Lights[lightCount].isLocal = GL_TRUE;		// Only directional lights AREN'T local
		::g_ShaderUniformVariables.Lights[lightCount].isSpot = GL_FALSE;		// Local = TRUE, Spot = FALSE --- Point light

		::g_ShaderUniformVariables.Lights[lightCount].color = glm::vec3(1.0f, 1.0f, 1.0f);		// White light
		::g_ShaderUniformVariables.Lights[lightCount].ambient = glm::vec3(0.2f, 0.0f, 0.0f);	// White light
		//

		::g_ShaderUniformVariables.Lights[lightCount].constantAttenuation = 0.87f;
		::g_ShaderUniformVariables.Lights[lightCount].linearAttenuation = 0.43f;
		::g_ShaderUniformVariables.Lights[lightCount].quadraticAttenuation = -0.02f;
	}

	//set positon
	::g_ShaderUniformVariables.Lights[0].position = glm::vec3(0.0f, -4.0f, 0.0f);	
	/*::g_ShaderUniformVariables.Lights[1].position = glm::vec3(0.0f, 2.0f, 5.0f);
	::g_ShaderUniformVariables.Lights[2].position = glm::vec3(0.0f, 2.0f, 5.0f);*/
	
}

void setup_Camera_GFunct()
{

}

void setGameObjectProperties()
{
	p_g_MediatorObj->get_Objects_ToRender(p_g_DebugObject_Vector);

	p_g_DebugObject_Vector[0]->position = m_Vec3d(0.0, 0.0f, 0.0f);
	p_g_DebugObject_Vector[0]->scale = 0.08;
	p_g_DebugObject_Vector[1]->position = m_Vec3d(-3.0, 3.0f, 0.0f);

}