#version 430 core

layout ( location = 0) in vec3 positions;
layout ( location = 1) in vec2 textureCoords;


out vec2 texture_coordinates;
uniform mat4 transformationMatrix;
uniform bool button_clicked;
void main()
{
     
	 
    
    if( button_clicked)
	{
		gl_Position = transformationMatrix* vec4(positions*1.02f,1.0f);
	}
	else
	{
			gl_Position = transformationMatrix* vec4(positions,1.0f);
	}
   
  texture_coordinates = textureCoords;
}