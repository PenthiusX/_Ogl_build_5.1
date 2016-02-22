
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>			// glm::value_ptr

#include "m_VertexTypes.h"				// For CVertex_fXYZ_fRGB, etc.
#include "m_global.h"
#include "TextureLoader.h"


void SetLightingUniforms(void);//prototype




void Render_SecondPass_Funct()
{
	::g_FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	clock_t Now = clock();
	if (::g_LastTime == 0)
	{
		::g_LastTime = Now;
	}
	//------------------------------------------------------------------------------------------------------------------->
	glm::mat4 matView(1.0f);
	matView = glm::lookAt(glm::vec3(p_g_CameraObj->eye.x, p_g_CameraObj->eye.y, p_g_CameraObj->eye.z),			// Camera (aka "Eye")
		glm::vec3(p_g_CameraObj->target.x, p_g_CameraObj->target.y, p_g_CameraObj->target.z),		// At (aka "target")
		glm::vec3(p_g_CameraObj->up.x, p_g_CameraObj->up.y, p_g_CameraObj->up.z));		// Up
	//-------------------------------------------------------------------------------------------------------------------->
	// Get the objects we are to render from the factory
	std::vector<m_GameObject*> p_gameObj_loc_vector;
	p_g_MediatorObj->get_Objects_ToRender(p_gameObj_loc_vector);//The local game OBjct gets populated with the Created GameObject

	//For LOD
	//GameOBject_Vector_fr_LOD;

	for (int index = 0; index != static_cast<unsigned int>(g_GameOBject_Vector_fr_LOD.size()); index++)
	{
		SetShaderUniformVariables();
		glm::mat4 matWorld = glm::mat4(1.0f);		// aka "World" matrix

		// (The last thing you do is the FIRST thing that's applied)
		matWorld = glm::translate(matWorld, glm::vec3(g_GameOBject_Vector_fr_LOD[index].position.x,
			g_GameOBject_Vector_fr_LOD[index].position.y,
			g_GameOBject_Vector_fr_LOD[index].position.z));

		matWorld = glm::rotate(matWorld, g_GameOBject_Vector_fr_LOD[index].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		matWorld = glm::rotate(matWorld, g_GameOBject_Vector_fr_LOD[index].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		matWorld = glm::rotate(matWorld, g_GameOBject_Vector_fr_LOD[index].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		matWorld = glm::scale(matWorld, glm::vec3(g_GameOBject_Vector_fr_LOD[index].scale,
			g_GameOBject_Vector_fr_LOD[index].scale,
			g_GameOBject_Vector_fr_LOD[index].scale));

		//------------- START of DRAW THE OBJECT FROM THE BUFFER --------------------------------
		//glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//---------------------------------------------------------------------------------------

		//-----------------APPLY TEXTURE PER OBJECT----------------------------------------------
		GLuint text1ID = glGetUniformLocation(g_ShaderUniformVariables.shaderProgram_ID, "Texture_Sample_1");
		if (g_GameOBject_Vector_fr_LOD[index].get_TextureNo() == 1)//All the texture loading and setting into buffers ..                                    
		{                                                     //is done in the TextureLoader Class
			glUniform1i(text1ID, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.g_textureID1);
			glGetError();
			//ExitOnGLError("ERROR: Could not use the shader program");
		}
		else if (g_GameOBject_Vector_fr_LOD[index].get_TextureNo() == 0)
		{
			glUniform1i(text1ID, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.g_textureID2);
			glGetError();
			//ExitOnGLError("ERROR: Could not use the shader program");
		}
		//-----------------------------------------------------------------------------------------

		//-----------USE THE SHADERS FROM THE GLUT_SHADRUNIFORMVARABLES----------------------------
		glUseProgram(g_ShaderUniformVariables.shaderProgram_ID);
		//-----------------------------------------------------------------------------------------

		// ----------------------------ADDED SIMPLE LIGHTING---------------------------------------
		::SetShaderUniformVariables();
		//-----------------------------------------------------------------------------------------
		glUniform3f(::g_ShaderUniformVariables.EyeDirection_LocationID, p_g_CameraObj->eye.x, p_g_CameraObj->eye.y, p_g_CameraObj->eye.z);// Set the camera
		glUniform1f(::g_ShaderUniformVariables.Shininess_LocationID, g_GameOBject_Vector_fr_LOD[index].specularShininess);// Shininess
		glUniform1f(::g_ShaderUniformVariables.Strength_LocationID, g_GameOBject_Vector_fr_LOD[index].specularStrength);// Strength...
		ExitOnGLError("ERROR: Could not use the shader program");

		SetLightingUniforms();
		//-----------------------------------------------------------------------------------------
		//--------------------SETTING THE WORLD AND VIEW MATRIX------------------------------------
		glUniformMatrix4fv(g_ModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matWorld));
		glUniformMatrix4fv(g_ViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matView));
		ExitOnGLError("ERROR: Could not set the shader uniforms");
		//-----------------------------------------------------------------------------------------

		//-----THIS PART SYNCS THE GAME OBJECT AND THE MODEL TO RENDER, BY COMPARING FILE NAMES----
		ModelDataINFo tempModelDataINFo;
		//std::string plyFileNameDEBUG = p_gameObj_loc_vector[index]->plyFileName;
		if (!::p_g_ModelLoader->GetRenderingInfoByModelFileName(g_GameOBject_Vector_fr_LOD[index].FileName, tempModelDataINFo))
		{
			continue;
		}
		//-----------------------------------------------------------------------------------------
		// At this point, we have found a valid model (that was loaded)
		glBindVertexArray(tempModelDataINFo.vertexPositionBufferID);				// "Connects" to a vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, tempModelDataINFo.vertexColourBufferID);	// Also buffer (with colours)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempModelDataINFo.indexBufferID);	// And Index buffer	
		//-----------------------------------------------------------------------------------------
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);			// color
		glEnableVertexAttribArray(2);			// normal
		glEnableVertexAttribArray(3);			//tex
		ExitOnGLError("ERROR: Could not enable vertex attributes");
		//-----------------Enable Before Draw Elements---------------------------------------------
		int numberOfIndicesToRender = 0;
		numberOfIndicesToRender = tempModelDataINFo.numberOfElements * 3;
		glDrawElements(GL_TRIANGLES, numberOfIndicesToRender, GL_UNSIGNED_INT, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not draw the cube");
		//--------------------Disable after--------------------------------------------
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		//------------------------------------------------------------------------------
	}
	// Detatch from the vertex buffer (0 is reserved in OpenGL, so setting to "0" means "set to nothing", sort of)
	glBindVertexArray(0);
	// Detatch from the current shader program
	glUseProgram(0);

	// Now that everything is drawn, show the back buffer
	// (i.e. switch the 'back' framebuffer with the 'front')
	glutSwapBuffers();
	return;
}
void Render_FirstPAss_Funct()
{
	::g_FrameCount++;

	//------------------Binding frame Buffer-------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, g_ShaderUniformVariables.frame_BufferID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	clock_t Now = clock();
	if (::g_LastTime == 0)
	{
		::g_LastTime = Now;
	}
	//------------------------------------------------------------------------------------------------------------------->
	glm::mat4 matView(1.0f);
	matView = glm::lookAt(glm::vec3(p_g_CameraObj->eye.x, p_g_CameraObj->eye.y, p_g_CameraObj->eye.z),			// Camera (aka "Eye")
		glm::vec3(p_g_CameraObj->target.x, p_g_CameraObj->target.y, p_g_CameraObj->target.z),		// At (aka "target")
		glm::vec3(p_g_CameraObj->up.x, p_g_CameraObj->up.y, p_g_CameraObj->up.z));		// Up
	//-------------------------------------------------------------------------------------------------------------------->
	// Get the objects we are to render from the factory
	std::vector<m_GameObject*> p_gameObj_loc_vector;
	p_g_MediatorObj->get_Objects_ToRender(p_gameObj_loc_vector);//The local game OBjct gets populated with the Created GameObject

	//For LOD
	//GameOBject_Vector_fr_LOD;

	for (int index = 1; index != static_cast<unsigned int>(g_GameOBject_Vector_fr_LOD.size()); index++)
	{
		SetShaderUniformVariables();
		glm::mat4 matWorld = glm::mat4(1.0f);		// aka "World" matrix

		// (The last thing you do is the FIRST thing that's applied)
		matWorld = glm::translate(matWorld, glm::vec3(g_GameOBject_Vector_fr_LOD[index].position.x,
			g_GameOBject_Vector_fr_LOD[index].position.y,
			g_GameOBject_Vector_fr_LOD[index].position.z));

		matWorld = glm::rotate(matWorld, g_GameOBject_Vector_fr_LOD[index].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		matWorld = glm::rotate(matWorld, g_GameOBject_Vector_fr_LOD[index].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		matWorld = glm::rotate(matWorld, g_GameOBject_Vector_fr_LOD[index].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		matWorld = glm::scale(matWorld, glm::vec3(g_GameOBject_Vector_fr_LOD[index].scale,
			g_GameOBject_Vector_fr_LOD[index].scale,
			g_GameOBject_Vector_fr_LOD[index].scale));

		//------------- START of DRAW THE OBJECT FROM THE BUFFER --------------------------------
		//glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//---------------------------------------------------------------------------------------

		//-----------------APPLY TEXTURE PER OBJECT----------------------------------------------
		GLuint text1ID = glGetUniformLocation(g_ShaderUniformVariables.shaderProgram_ID, "Texture_Sample_1");
		if (g_GameOBject_Vector_fr_LOD[index].get_TextureNo() == 1)//All the texture loading and setting into buffers ..                                    
		{                                                     //is done in the TextureLoader Class
			glUniform1i(text1ID, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.g_textureID1);
			glGetError();
			//ExitOnGLError("ERROR: Could not use the shader program");
		}
		else if (g_GameOBject_Vector_fr_LOD[index].get_TextureNo() == 0)
		{
			glUniform1i(text1ID, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.g_textureID2);
			glGetError();
			//ExitOnGLError("ERROR: Could not use the shader program");
		}
			else if (g_GameOBject_Vector_fr_LOD[index].get_TextureNo() == 2)
		{
			glUniform1i(text1ID, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, g_ShaderUniformVariables.color_BufferID);
			glGetError();
			//ExitOnGLError("ERROR: Could not use the shader program");
		}
		//-----------------------------------------------------------------------------------------

		//-----------USE THE SHADERS FROM THE GLUT_SHADRUNIFORMVARABLES----------------------------
		glUseProgram(g_ShaderUniformVariables.shaderProgram_ID);
		//-----------------------------------------------------------------------------------------

		// ----------------------------ADDED SIMPLE LIGHTING---------------------------------------
		::SetShaderUniformVariables();
		//-----------------------------------------------------------------------------------------
		glUniform3f(::g_ShaderUniformVariables.EyeDirection_LocationID, p_g_CameraObj->eye.x, p_g_CameraObj->eye.y, p_g_CameraObj->eye.z);// Set the camera
		glUniform1f(::g_ShaderUniformVariables.Shininess_LocationID, g_GameOBject_Vector_fr_LOD[index].specularShininess);// Shininess
		glUniform1f(::g_ShaderUniformVariables.Strength_LocationID, g_GameOBject_Vector_fr_LOD[index].specularStrength);// Strength...
		ExitOnGLError("ERROR: Could not use the shader program");

		SetLightingUniforms();
		//-----------------------------------------------------------------------------------------
		//--------------------SETTING THE WORLD AND VIEW MATRIX------------------------------------
		glUniformMatrix4fv(g_ModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matWorld));
		glUniformMatrix4fv(g_ViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matView));
		ExitOnGLError("ERROR: Could not set the shader uniforms");
		//-----------------------------------------------------------------------------------------

		//-----THIS PART SYNCS THE GAME OBJECT AND THE MODEL TO RENDER, BY COMPARING FILE NAMES----
		ModelDataINFo tempModelDataINFo;
		//std::string plyFileNameDEBUG = p_gameObj_loc_vector[index]->plyFileName;
		if (!::p_g_ModelLoader->GetRenderingInfoByModelFileName(g_GameOBject_Vector_fr_LOD[index].FileName, tempModelDataINFo))
		{
			continue;
		}
		//-----------------------------------------------------------------------------------------
		// At this point, we have found a valid model (that was loaded)
		glBindVertexArray(tempModelDataINFo.vertexPositionBufferID);				// "Connects" to a vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, tempModelDataINFo.vertexColourBufferID);	// Also buffer (with colours)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempModelDataINFo.indexBufferID);	// And Index buffer	
		//-----------------------------------------------------------------------------------------
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);			// color
		glEnableVertexAttribArray(2);			// normal
		glEnableVertexAttribArray(3);			//tex
		ExitOnGLError("ERROR: Could not enable vertex attributes");
		//-----------------Enable Before Draw Elements---------------------------------------------
		int numberOfIndicesToRender = 0;
		numberOfIndicesToRender = tempModelDataINFo.numberOfElements * 3;
		glDrawElements(GL_TRIANGLES, numberOfIndicesToRender, GL_UNSIGNED_INT, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not draw the cube");
		//--------------------Disable after--------------------------------------------
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		//------------------------------------------------------------------------------
	}
	
	// Detatch from the vertex buffer (0 is reserved in OpenGL, so setting to "0" means "set to nothing", sort of)
	glBindVertexArray(0);
	// Detatch from the current shader program
	glUseProgram(0);
	// Now that everything is drawn, show the back buffer
	// (i.e. switch the 'back' framebuffer with the 'front')

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Unbinding------------------------------------->

	//glutSwapBuffers();
	return;
}

void glutDisplay_callback(void)
{
	Render_FirstPAss_Funct();//---------getting the Frame Buffer Texture
	Render_SecondPass_Funct();//--------setting the Frame Buffer Texture;
}

//Seting Lighting uniforms
void SetLightingUniforms(void)
{
	for (int index = 0; index < ::g_ShaderUniformVariables.MaxLights; index++)
		{

			glUniform1i(::g_ShaderUniformVariables.Lights[index].isEnabled_LocID,
						::g_ShaderUniformVariables.Lights[index].isEnabled);

			glUniform1i(::g_ShaderUniformVariables.Lights[index].isLocal_LocID,
						::g_ShaderUniformVariables.Lights[index].isLocal);

			glUniform1i(::g_ShaderUniformVariables.Lights[index].isSpot_LocID, 
						::g_ShaderUniformVariables.Lights[index].isSpot);

			glUniform3f(::g_ShaderUniformVariables.Lights[index].ambient_LocID,
						::g_ShaderUniformVariables.Lights[index].ambient.r,
						::g_ShaderUniformVariables.Lights[index].ambient.g,
						::g_ShaderUniformVariables.Lights[index].ambient.b);

			glUniform3f(::g_ShaderUniformVariables.Lights[index].color_LocID, 
						::g_ShaderUniformVariables.Lights[index].color.r,
						::g_ShaderUniformVariables.Lights[index].color.g,
						::g_ShaderUniformVariables.Lights[index].color.b);

			glUniform3f(::g_ShaderUniformVariables.Lights[index].position_LocID, 
						::g_ShaderUniformVariables.Lights[index].position.r,
						::g_ShaderUniformVariables.Lights[index].position.g,
						::g_ShaderUniformVariables.Lights[index].position.b);

			glUniform3f(::g_ShaderUniformVariables.Lights[index].halfVector_LocID, 
						::g_ShaderUniformVariables.Lights[index].halfVector.r,
						::g_ShaderUniformVariables.Lights[index].halfVector.g,
						::g_ShaderUniformVariables.Lights[index].halfVector.b);

			glUniform3f(::g_ShaderUniformVariables.Lights[index].coneDirection_LocID, 
						::g_ShaderUniformVariables.Lights[index].coneDirection.r,
						::g_ShaderUniformVariables.Lights[index].coneDirection.g,
						::g_ShaderUniformVariables.Lights[index].coneDirection.b);

			glUniform1f(::g_ShaderUniformVariables.Lights[index].spotCosCutoff_LocID, 
						::g_ShaderUniformVariables.Lights[index].spotCosCutoff);

			glUniform1f(::g_ShaderUniformVariables.Lights[index].spotExponent_LocID, 
						::g_ShaderUniformVariables.Lights[index].spotExponent);

			glUniform1f(::g_ShaderUniformVariables.Lights[index].constantAttenuation_LocID, 
						::g_ShaderUniformVariables.Lights[index].constantAttenuation);

			glUniform1f(::g_ShaderUniformVariables.Lights[index].linearAttenuation_LocID, 
						::g_ShaderUniformVariables.Lights[index].linearAttenuation);

			glUniform1f(::g_ShaderUniformVariables.Lights[index].quadraticAttenuation_LocID, 
						::g_ShaderUniformVariables.Lights[index].quadraticAttenuation);

		}
	return;
}