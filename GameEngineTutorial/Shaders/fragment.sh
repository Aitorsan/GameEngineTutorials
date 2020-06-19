#version 430 core

in vec2 texture_coordinates;

out vec4 pixel_color;

uniform bool is_inside;
uniform bool button_clicked;
uniform sampler2D texture_sample; //texture(texture_sample,texture_coordinates) *
void main()
{
   if(is_inside && !button_clicked )
      pixel_color =  vec4(0.3,0.5f,0.6f,1.f);
   else if (is_inside && button_clicked)
	  pixel_color =  vec4(0.3,0.6f,0.7f,1.f);
   else
      pixel_color =  vec4(0.3,0.4,0.5,1.f);
   
}