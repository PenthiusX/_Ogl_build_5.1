#ifndef _globlaSpace_
#define _globalSpace_

#include <time.h>	
#include <iostream>

#include "glut_SetShaderUniformVariables.h"
#define GLM_FORCE_CXX98    
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> 

#include "m_GameObject.h"				
#include "m_triangle.h"		
#include "Mediator.h"	
#include "m_ModelLoadManager.h"


#include "m_Camera.h"
#include "FBXLoader.h"


extern unsigned int screen_Width;
extern unsigned int screen_Height;

//---For fbx Loader---------
extern FBXLoader* m_FbxObj;
//--------------------------

extern ShaderUniformVariables g_ShaderUniformVariables;
extern m_Camera* p_g_CameraObj;
extern m_ModelLoadManager*  p_g_ModelLoader;	// "p" for pointer
extern Mediator* p_g_MediatorObj;

extern unsigned int g_Player_ID;	// = 0;		// Used to locate the player
//---------------------------------------------------
extern std::vector< m_GameObject* > p_g_DebugObject_Vector;
//----------------------------------------------------

extern glm::mat4 g_matProjection;
extern glm::mat4 g_matView;
extern glm::mat4 g_matWorld;		// aka "Model" matrix

extern GLuint g_ProjectionMatrixUniformLocation; 
extern GLuint g_ViewMatrixUniformLocation;
extern GLuint g_ModelMatrixUniformLocation;
extern GLuint g_ObjectColourUniformLocation;

extern GLuint g_slot_LightPosition;	//uniform vec4 LightPosition;	
extern GLuint g_slot_LightColour;	//uniform vec4 LightColour;		
extern GLuint g_slot_attenuation;	//uniform float attenuation;	

//extern GLuint g_ShaderProgram_ID;
//extern GLuint g_VertexShader_ID;
//extern GLuint g_FragmentShader_ID;//Now moved to the Cshaderuniform Struct..

extern int g_screenWidth;	// = 1400;
extern int g_screenHeight;	// = 900;
extern int g_windowHandle;	// = 0;
extern unsigned int g_FrameCount;	// = 0;
extern clock_t g_LastTime;		// = 0;		

// Since this is static (i.e. there's only one of these), so we don't need "extern"
//static const std::string g_WindowTitleDefault = "Aditya's OpengL Template!";

bool SetShaderUniformVariables(void);

bool LoadPlyFileIntoGLBuffers(std::string plyFile,
									GLuint &vertexBufferID,
									GLuint &vertexColourBufferID,
									GLuint &indexBufferID,
									int &numberOfVerticesLoaded,
									int &numberOfTrianglesLoaded,
									float &maxExtent);

bool OpenGL_Initialize(int argc, char* argv[], int screenWidth, int screenHeight);

// Loads the "whats_this.txt" file and displays it in the console
void printTheWhatsThisProgramAboutBlurb(void);

// OpenGL callbacks-----------------
void glutReshape_callback(int Width, int Height);		// void ResizeFunction(int, int);
void glutDisplay_callback(void);						// void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
//void glutClose_callback(void);							// void DestroyCube(void);//Not being used

//----------------------------------
void glutKeyboard_callback(unsigned char key, int x, int y);	// void keyboardFunctionCallback(unsigned char key, int x, int y);
void glutSpecialKey_callback(int key, int x, int y);			// void specialKeyCallback( int key, int x, int y );

void ExitOnGLError(const char* error_message);						// from Util.cpp
GLuint LoadShader(const char* filename, GLenum shader_type);		// from Util.cpp


///LOD

extern std::vector<m_GameObject>g_GameOBject_Vector_fr_LOD;

#endif