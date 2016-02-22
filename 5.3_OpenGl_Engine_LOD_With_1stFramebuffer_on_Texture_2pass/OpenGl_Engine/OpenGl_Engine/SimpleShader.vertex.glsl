// This shader is from the OpenGL Programming Guide, 8th edition, pg 377-378


//VERTEX SHADER with normal for lights

#version 400

// in and out mean vertex buffer
layout(location=0) in vec4 in_Position;   //Vertex in positions
layout(location=1) in vec4 in_Color;	
layout(location=2) in vec4 in_Normal;	// We'll only use vec3 of the normal
layout(location=3) in vec4 in_VertexUVCoords;//AddingTextures



uniform mat4 ModelMatrix;    		// aka WorldMAtrix
uniform mat4 ViewMatrix;             //Camera matrix
uniform mat4 ProjectionMatrix;       //POV

// From the fragment shader
uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;
		
out vec4 ex_Position;    //Vertex  out positions
out vec4 ex_Color;       //Vertex Color
out vec3 ex_Normal;			// Note Normal is a vec3
out vec4 ex_UVCoords;      //Adding Textures

//For lod
uniform vec2 BillboardSize;
uniform mat4 CameraRight_worldspace;
uniform mat4 CameraUp_worldspace;


void billboardFunct(void);
void main(void)
{	
	mat4 MVMatrix = ViewMatrix * ModelMatrix;
	mat4 MVPMatrix = ProjectionMatrix * MVMatrix;

	// gl_Position is a built-in variable
	gl_Position = MVPMatrix * in_Position;
	ex_Position = ModelMatrix * in_Position;	
			
	ex_Normal = vec3(in_Normal);
	ex_Color = in_Color;
	
	//For textures
	ex_UVCoords = in_VertexUVCoords;
}

 void billboardFunct(void)
  {							 
 
     // ex_Position =  in_Position.xyz + CameraRight_worldspace * in_Position.x * BillboardSize.x + CameraUp_worldspace * in_Position.y * BillboardSize.y;
		//http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/				 
 }