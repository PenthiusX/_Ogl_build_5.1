#include "m_global.h"
#include <iostream>
#include <sstream>		// Added

#define WINDOW_TITLE_PREFIX "OGL Template"

static const std::string g_WindowTitleDefault = "Aditya's OpengL Template!";
bool InitWindow(int argc, char* argv[], int screenWidth, int screenHeight)
{
	glutInit(&argc, argv);

	::g_screenWidth = screenWidth;
	::g_screenHeight = screenHeight;

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(::g_screenWidth, ::g_screenHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);
	::g_windowHandle = glutCreateWindow(::g_WindowTitleDefault.c_str());

	if (::g_windowHandle < 1)	//  if(WindowHandle < 1) 
	{
		std::cout << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Tells GLUT what functions to call...            ("callback" functions)
	glutReshapeFunc(glutReshape_callback);			// glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(glutDisplay_callback);			// glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(glutKeyboard_callback);		// glutKeyboardFunc( keyboardFunctionCallback );
	glutSpecialFunc(glutSpecialKey_callback);		// glutSpecialFunc( specialKeyCallback );
	
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);

	//glutCloseFunc(glutClose_callback);				// glutCloseFunc(DestroyCube);//Not being used
	
	return true;
}

bool OpenGL_Initialize(int argc, char* argv[], int screenWidth, int screenHeight)
{
	GLenum GlewInitResult;

	std::cout << "Aditya OGL Template" << std::endl;

	if (!InitWindow(argc, argv, screenWidth, screenHeight))
	{
		return false;	// Error
	}

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		std::cout << "ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glGetError();
	glClearColor(0.5f, 0.0f, 0.5f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

	return true;
}

void TimerFunction(int Value)//Meant to show the FrameRates
{
	if (0 != Value)
	{
		std::stringstream ss;
		ss << WINDOW_TITLE_PREFIX << "  Fps:"
			<< ::g_FrameCount * 4 << "  Res:"
			<< ::g_screenWidth << " x"				// << CurrentWidth << " x "
			<< ::g_screenHeight;					// << CurrentHeight;

		glutSetWindowTitle(ss.str().c_str());
		//glutSetWindowTitle(TempString);
		//free(TempString);
	}

	::g_FrameCount = 0;	// FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);

	return;
}