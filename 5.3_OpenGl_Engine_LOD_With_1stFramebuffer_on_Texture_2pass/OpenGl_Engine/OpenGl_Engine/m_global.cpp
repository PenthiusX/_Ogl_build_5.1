#include "m_global.h"


unsigned int screen_Width;
unsigned int screen_Height;

//---For fbx Loader
FBXLoader* m_FbxObj;
//-------------

glm::mat4 g_matProjection = glm::mat4(1.0f);	// 1.0f gives identity matrix
glm::mat4 g_matView = glm::mat4(1.0f);
glm::mat4 g_matWorld = glm::mat4(1.0f);		// aka "Model" matrix

GLuint g_ProjectionMatrixUniformLocation = 0;
GLuint g_ViewMatrixUniformLocation = 0;
GLuint g_ModelMatrixUniformLocation = 0;
GLuint g_ObjectColourUniformLocation = 0;

GLuint g_slot_LightPosition = 0;	//uniform vec4 LightPosition;	
GLuint g_slot_LightColour = 0;	//uniform vec4 LightColour;		
GLuint g_slot_attenuation = 0;	//uniform float attenuation;	

GLuint g_ShaderProgram_ID = 0;
GLuint g_VertexShader_ID = 0;
GLuint g_FragmentShader_ID = 0;

int g_screenWidth = 1400;
int g_screenHeight = 900;
int g_windowHandle = 0;
unsigned int g_FrameCount = 0;
clock_t g_LastTime = 0;

//Game Objects to render
std::vector<m_GameObject*> p_g_DebugObject_Vector;
m_Camera* p_g_CameraObj = 0;
m_ModelLoadManager*  p_g_ModelLoader = 0;	// set pointers to zero just in case
Mediator* p_g_MediatorObj = 0;

unsigned int g_Player_ID = 0;				// Used to locate the player

ShaderUniformVariables g_ShaderUniformVariables;//For the shader uniforms ie lights etc;

#include <iostream>	// for LoadShader, which we'll be replacing soon
void ExitOnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		//fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
		std::cout << gluErrorString(ErrorValue) << std::endl;
		//exit(EXIT_FAILURE);
	}
}

//LOD
std::vector<m_GameObject> g_GameOBject_Vector_fr_LOD;
