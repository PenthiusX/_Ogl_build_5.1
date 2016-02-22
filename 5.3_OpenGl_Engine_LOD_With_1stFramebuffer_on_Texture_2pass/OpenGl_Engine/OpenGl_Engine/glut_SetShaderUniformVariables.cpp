#include "m_global.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

bool SetLightingUniforms(void);//prototype

//Read Shaders 
GLuint ReadnCreateShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	char* glsl_source;		// std::string shaderSourceText;

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);

		if (NULL != (glsl_source = (char*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(char), file_size, file))
			{
				glsl_source[file_size] = '\0';

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, &glsl_source, NULL);
					glCompileShader(shader_id);
					ExitOnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", file_size);

		fclose(file);
	}
	else
	{
		if (NULL != file)
			fclose(file);
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);
	}

	//GLcharARB* vShaderSource[1];
	//vShaderSource[0] = glsl_source;
	//
	////Error Checking
	//::glShaderSource(shader_id,        // Handle to shader
	//
	//	1,                       // Number of elements in string array (if using one big string, it's 1)
	//
	//	(const GLcharARB**)vShaderSource, // Pointer to shader source (char array)
	//
	//	NULL);                   // Length of array (0 or NULL if null terminated)
	//
	//// ...and compile.
	//
	//::glCompileShader(shader_id);
	//
	////
	//
	//// Did compile work?
	//
	//GLint statusOK = 0;
	//GLint isCompiled = 0;
	//glGetShaderiv(shader_id, GL_COMPILE_STATUS, &statusOK);
	//// ::glGetObjectParameterivARB( shader_id , GL_OBJECT_COMPILE_STATUS_ARB, &statusOK );
	//
	//if (!statusOK)
	//
	//{
	//
	//	// shaderProgDescription.vShader.vecShaderErrors.push_back("Error: Could not compile vertex shader.");
	//
	//	// This gets the 'last' error message for that shader (if there was one)
	//
	//	GLcharARB infoLog[GL_INFO_LOG_LENGTH];   // defined in glext.h
	//
	//	glGetInfoLogARB(shader_id, GL_INFO_LOG_LENGTH, NULL, infoLog);
	//
	//	std::stringstream ss;
	//
	//	std::cout << infoLog << std::endl;
	//
	//	// shaderProgDescription.vShader.vecShaderErrors.push_back( ss.str() );
	//}
	//else
	//{
	//	std::cout << "Shader compiled " << std::endl;
	//}
	return shader_id;
}

//Load Shader
void AttachShader(void)
{
	g_ShaderUniformVariables.shaderProgram_ID = glCreateProgram();
	ExitOnGLError("ERROR: Could not create the shader program");

	g_ShaderUniformVariables.vertexShader_ID = ReadnCreateShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
	g_ShaderUniformVariables.fragmentShader_ID = ReadnCreateShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);

	// Attaching shaders to the OGL program
	glAttachShader(g_ShaderUniformVariables.shaderProgram_ID, g_ShaderUniformVariables.vertexShader_ID);
	glAttachShader(g_ShaderUniformVariables.shaderProgram_ID, g_ShaderUniformVariables.fragmentShader_ID);
	glLinkProgram(g_ShaderUniformVariables.shaderProgram_ID);
	ExitOnGLError("ERROR: Could not link the shader program");

	return;
}

bool SetShaderUniformVariables(void)
{
// From the vertex shader---------------------------------------------------------------------------------------------------------
	::g_ModelMatrixUniformLocation = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, "ModelMatrix");
	if (g_ModelMatrixUniformLocation == -1)
	{
		std::cout << "Warning: Can't find ModelMatrixUniformLocation in vertex shader." << std::endl;
	}
	::g_ViewMatrixUniformLocation = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, "ViewMatrix");
	if (g_ViewMatrixUniformLocation == -1)
	{
		std::cout << "Warning: Can't find ViewMatrixUniformLocation in vertex Shader." << std::endl;
	}
	::g_ProjectionMatrixUniformLocation = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, "ProjectionMatrix");
	if (g_ProjectionMatrixUniformLocation == -1)
	{
		std::cout << "Warning: Can't find ProjectionMatrixUniformLocation in vertex shader." << std::endl;
	}

//-----------------------------------------------------------------------------------------------------------------------------------
	// From the Fragment shader
	::g_ShaderUniformVariables.Shininess_LocationID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, "Shininess");
	if (::g_ShaderUniformVariables.Shininess_LocationID == -1)
	{
		std::cout << "Warning: Can't find Shininess uniform variable in shader program." << std::endl;
	}

	::g_ShaderUniformVariables.Strength_LocationID = glGetUniformLocation(g_ShaderUniformVariables.shaderProgram_ID, "Strength"); //Strength
	if (::g_ShaderUniformVariables.Strength_LocationID == -1)
	{
		std::cout << "Warning: Can't find Strength uniform variable in shader program." << std::endl;
	}

	::g_ShaderUniformVariables.EyeDirection_LocationID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, "EyeDirection");
	if (::g_ShaderUniformVariables.EyeDirection_LocationID == -1)
	{
		std::cout << "Warning: Can't find EyeDirection uniform variable in shader program." << std::endl;
	}

	if (!SetLightingUniforms())
	{
		std::cout << "Warning: Can't find (one or more) Lights uniform variable in shader program." << std::endl;
	}
	ExitOnGLError("ERROR: Could not get shader uniform locations");

	return true;
}

bool SetLightingUniforms(void)
{
	bool bNoErrors = true;

	std::string isEnabled;
	std::string isLocal;
	std::string isSpot;
	std::string ambient;
	std::string color;
	std::string position;
	std::string halfVector;
	std::string coneDirection;
	std::string spotCosCutoff;
	std::string spotExponent;
	std::string constantAttenuation;
	std::string linearAttenuation;
	std::string quadraticAttenuation;

  for (int i = 0; i < ::g_ShaderUniformVariables.MaxLights; i++)
	{
		isEnabled = "Lights[" + std::to_string(i) + "].isEnabled";
		isLocal = "Lights[" + std::to_string(i) + "].isLocal";
		isSpot = "Lights[" + std::to_string(i) + "].isSpot";
		ambient = "Lights[" + std::to_string(i) + "].ambient";
		color = "Lights[" + std::to_string(i) + "].color";
		position = "Lights[" + std::to_string(i) + "].position";
		halfVector = "Lights[" + std::to_string(i) + "].halfVector";
		coneDirection = "Lights[" + std::to_string(i) + "].coneDirection";
		spotCosCutoff = "Lights[" + std::to_string(i) + "].spotCosCutoff";
		spotExponent = "Lights[" + std::to_string(i) + "].spotExponent";
		constantAttenuation = "Lights[" + std::to_string(i) + "].constantAttenuation";
		linearAttenuation = "Lights[" + std::to_string(i) + "].linearAttenuation";
		quadraticAttenuation = "Lights[" + std::to_string(i) + "].quadraticAttenuation";

		const GLchar*  gl_enabled = isEnabled.c_str();
		const GLchar*  gl_Local = isLocal.c_str();
		const GLchar*  gl_Spot = isSpot.c_str();
		const GLchar*  gl_ambient = ambient.c_str();
		const GLchar*  gl_color = color.c_str();
		const GLchar*  gl_position = position.c_str();
		const GLchar*  gl_halfVector = halfVector.c_str();
		const GLchar*  gl_coneDirection = coneDirection.c_str();
		const GLchar*  gl_spotCosCutoff = spotCosCutoff.c_str();
		const GLchar*  gl_spotExponent = spotExponent.c_str();
		const GLchar*  gl_constantAttenuation = constantAttenuation.c_str();
		const GLchar*  gl_linearAttenuation = linearAttenuation.c_str();
		const GLchar*  gl_quadraticAttenuation = quadraticAttenuation.c_str();

		::g_ShaderUniformVariables.Lights[i].isEnabled_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_enabled);
		::g_ShaderUniformVariables.Lights[i].isLocal_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_Local);
		::g_ShaderUniformVariables.Lights[i].isSpot_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_Spot);
		::g_ShaderUniformVariables.Lights[i].ambient_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_ambient);
		::g_ShaderUniformVariables.Lights[i].color_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_color);
		::g_ShaderUniformVariables.Lights[i].position_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_position);
		::g_ShaderUniformVariables.Lights[i].halfVector_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_halfVector);
		::g_ShaderUniformVariables.Lights[i].coneDirection_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_coneDirection);
		::g_ShaderUniformVariables.Lights[i].spotCosCutoff_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_spotCosCutoff);
		::g_ShaderUniformVariables.Lights[i].spotExponent_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_spotExponent);
		::g_ShaderUniformVariables.Lights[i].constantAttenuation_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_constantAttenuation);
		::g_ShaderUniformVariables.Lights[i].linearAttenuation_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_linearAttenuation);
		::g_ShaderUniformVariables.Lights[i].quadraticAttenuation_LocID = glGetUniformLocation(::g_ShaderUniformVariables.shaderProgram_ID, gl_quadraticAttenuation);
	}
	
  return bNoErrors;
}