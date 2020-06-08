#version 430 core

layout ( location = 0) in vec3 positions;
layout ( location = 1) in vec2 textureCoords;

out vec2 texture_coordinates;

uniform mat4 transformationMatrix;

void main()
{
  gl_Position =  vec4(positions,1.0f);
  texture_coordinates = textureCoords;
}