#include "m_global.h"


void glutClose_callback()
{
	// We get rid of all the shaders. 
	//	Note that we do this in (sort of) the reverse order that we created them.
	// (We don't *really* have to do that, but)
	glDetachShader(g_ShaderUniformVariables.shaderProgram_ID, g_ShaderUniformVariables.vertexShader_ID);		// glDetachShader(g_ShaderIds[0], g_ShaderIds[1]);
	glDetachShader(g_ShaderUniformVariables.shaderProgram_ID, g_ShaderUniformVariables.fragmentShader_ID);	// glDetachShader(g_ShaderIds[0], g_ShaderIds[2]);

	glDeleteShader(g_ShaderUniformVariables.vertexShader_ID);	// glDeleteShader(g_ShaderIds[1]);
	glDeleteShader(g_ShaderUniformVariables.fragmentShader_ID);	// glDeleteShader(g_ShaderIds[2]);
	glDeleteProgram(g_ShaderUniformVariables.shaderProgram_ID);	// glDeleteProgram(g_ShaderIds[0]);
	ExitOnGLError("ERROR: Could not destroy the shaders");

	//p_g_ModelLoader->ShutDown();

	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}