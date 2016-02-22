#include "m_global.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

void glutReshape_callback(int Width, int Height)
{
	::g_screenWidth = Width;	// CurrentWidth = Width;
	::g_screenHeight = Height;	// CurrentHeight = Height;

	//glViewport(0, 0, CurrentWidth, CurrentHeight);
	glViewport(0, 0, ::g_screenWidth, ::g_screenHeight);

	g_matProjection = glm::mat4(1.0f);
	
	g_matProjection = glm::perspective(glm::radians(45.0f), (float)g_screenWidth/g_screenHeight, 0.1f, 100.f);

	glUseProgram(g_ShaderUniformVariables.shaderProgram_ID);

	glUniformMatrix4fv(g_ProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(g_matProjection));
	glUseProgram(0);

	return;
}
