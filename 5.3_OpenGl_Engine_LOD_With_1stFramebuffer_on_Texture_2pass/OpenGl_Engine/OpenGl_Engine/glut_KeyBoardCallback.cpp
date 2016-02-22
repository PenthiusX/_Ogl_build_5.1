#include "m_global.h"


void glutKeyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':		// Space
		/*CMessage theMessage;
		theMessage.vecNVPairs.push_back(CNameValuePair("StopMoving"));
		::g_pFactoryMediator->SendMessageToObject(g_Player_ID, 0, theMessage);*/
	break;

	case 'b': case 'B':
		// View from above "birds eye view"
		::p_g_CameraObj->target = m_Vec3d(0.0f, 0.0f, 0.0f);// Origin
		::p_g_CameraObj->eye = m_Vec3d(0.0f, 10.0f, 0.1f); // NOOOOOOOO!

	case 'p': case 'P':
		std::cout << "ConstAtten=" << ::g_ShaderUniformVariables.Lights[0].constantAttenuation << std::endl;
		std::cout << "linerAtten=" << ::g_ShaderUniformVariables.Lights[0].linearAttenuation << std::endl;
		std::cout << "QuadraticAtten" << ::g_ShaderUniformVariables.Lights[0].quadraticAttenuation << std::endl;

		break;

    case 'K':
		::g_ShaderUniformVariables.Lights[0].constantAttenuation = ::g_ShaderUniformVariables.Lights[0].constantAttenuation + 0.01f;
		break;

	case 'k':
		::g_ShaderUniformVariables.Lights[0].constantAttenuation = ::g_ShaderUniformVariables.Lights[0].constantAttenuation - 0.01f;

		break;

	case 'L':
		::g_ShaderUniformVariables.Lights[0].linearAttenuation = ::g_ShaderUniformVariables.Lights[0].linearAttenuation + 0.01f;
		break;

	case 'l':
		::g_ShaderUniformVariables.Lights[0].linearAttenuation = ::g_ShaderUniformVariables.Lights[0].linearAttenuation - 0.01f;
		break;

	case 'J':
		::g_ShaderUniformVariables.Lights[0].quadraticAttenuation = ::g_ShaderUniformVariables.Lights[0].quadraticAttenuation + 0.01f;
		break;

	case 'j':
		::g_ShaderUniformVariables.Lights[0].quadraticAttenuation = ::g_ShaderUniformVariables.Lights[0].quadraticAttenuation - 0.01f;
		break;


	case 'T': case 't':
		::g_ShaderUniformVariables.Lights[0].position.z = ::g_ShaderUniformVariables.Lights[0].position.z - 0.11f;
		break;

	case 'G': case 'g':
		::g_ShaderUniformVariables.Lights[0].position.z = ::g_ShaderUniformVariables.Lights[0].position.z + 0.11f;
		break;

	case 'F': case 'f':
		::g_ShaderUniformVariables.Lights[0].position.x = ::g_ShaderUniformVariables.Lights[0].position.x - 0.11f;
		break;
	case 'H': case 'h':
		::g_ShaderUniformVariables.Lights[0].position.x = ::g_ShaderUniformVariables.Lights[0].position.x + 0.11f;
		break;



	case 'w':
		::p_g_CameraObj->eye.x -= 0.1f;		// Move the camera "left" (-ve x)
		//::g_ourObjects[2].y += 0.1f;	// Up
		//g_ourObjects[2].velY += 0.01f;
		//g_ourObjects[2].m_State = "GOING_UP";
		break;

	case 'W':
		::p_g_CameraObj->eye.y -= 0.1f;
		break;

	case 'S':
		::p_g_CameraObj->eye.y += 0.1f;
		break;

	case 's':
		::p_g_CameraObj->eye.x += 0.1f;		// Right (+ve x)
		//::g_ourObjects[2].y -= 0.1f;	// Down
		//::g_ourObjects[2].velY -= 0.01f;	// Down
		//g_ourObjects[2].m_State = "GOING_DOWN";
		break;
	case 'a': case 'A':
		::p_g_CameraObj->eye.z -= 0.1f;		// Forward (-ve z)
		//::g_ourObjects[2].x -= 0.1f;	// Left
		//::g_ourObjects[2].velX -= 0.01f;	// Left
		//g_ourObjects[2].m_State = "GOING_LEFT";
		break;
	case 'd': case 'D':
		::p_g_CameraObj->eye.z += 0.1f;		// Back (-ve z)
		//::g_ourObjects[2].x += 0.1f;	// Right
		//::g_ourObjects[2].velX += 0.01f;	// Right
		//g_ourObjects[2].m_State = "GOING_RIGHT";
		break;
	};

	return;
}

void glutSpecialKey_callback(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		//::g_ourObjects[2].z += 0.1f;		// Away (along z axis)
		// Sending a bunny a command
	{
		//CMessage theMessage;
		//theMessage.vecNVPairs.push_back(CNameValuePair("ChangeDirection", "Z"));
		//theMessage.vecNVPairs.push_back(CNameValuePair("NewZVel", 0.25f));
		//::p_g_MediatorObj->SendMessageToObject(g_Player_ID, 0, theMessage);
	}
	break;
	case GLUT_KEY_DOWN:
		//::g_ourObjects[2].z -= 0.1f;		// Towards (along z axis)
	{
	/*	CMessage theMessage;
		theMessage.vecNVPairs.push_back(CNameValuePair("ChangeDirection", "Z"));
		theMessage.vecNVPairs.push_back(CNameValuePair("NewZVel", -0.25f));
		::g_pFactoryMediator->SendMessageToObject(g_Player_ID, 0, theMessage);*/
	}
	break;
	case GLUT_KEY_LEFT:
	{
	/*	CMessage theMessage;
		theMessage.vecNVPairs.push_back(CNameValuePair("ChangeDirection", "X"));
		theMessage.vecNVPairs.push_back(CNameValuePair("NewXVel", -0.25f));
		::g_pFactoryMediator->SendMessageToObject(g_Player_ID, 0, theMessage);*/
	}
	break;
	case GLUT_KEY_RIGHT:
	{
		/*CMessage theMessage;
		theMessage.vecNVPairs.push_back(CNameValuePair("ChangeDirection", "X"));
		theMessage.vecNVPairs.push_back(CNameValuePair("NewXVel", 0.25f));
		::g_pFactoryMediator->SendMessageToObject(g_Player_ID, 0, theMessage);*/
	}
	break;
	};
	return;
}