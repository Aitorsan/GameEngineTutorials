#version 430 core

in vec2 texture_coordinates;

out vec4 pixel_color;

uniform sampler2D texture_sample;
void main()
{
   pixel_color =  texture(texture_sample,texture_coordinates);
}