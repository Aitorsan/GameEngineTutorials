#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec3 mouse_pos;
out float cursor_inside;
out vec2 texture_coordinates;

in VS_OUT {
    vec2 texture_coordinates;
} gs_in[];

void main()
{     cursor_inside = 0.0f;
      vec4 v0vec3 = gl_in[1].gl_Position - gl_in[0].gl_Position; 
      vec4 v1vec3 = gl_in[2].gl_Position - gl_in[0].gl_Position; 
      vec4 v2vec3 = vec4(mouse_pos,1.f)  - gl_in[0].gl_Position; 

	vec2 v0 = vec2(v2vec3.xy);
	vec2 v1 = vec2(v2vec3.xy);
	vec2 v2= vec2(v2vec3.xy);
	// Compute dot products
	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	float uv = u+v;

    if (u >= 0.000f && v >= 0.00f && uv < 1.000f)
    {
        cursor_inside = 0.5f;
    }
	else
	{
	   cursor_inside = 0.00f;
	}
	

	gl_Position = gl_in[0].gl_Position;
	texture_coordinates = gs_in[0].texture_coordinates.xy+cursor_inside;
	EmitVertex();
		gl_Position = gl_in[1].gl_Position;
		texture_coordinates =texture_coordinates = gs_in[1].texture_coordinates.xy+cursor_inside;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	texture_coordinates =texture_coordinates = gs_in[2].texture_coordinates.xy +cursor_inside;
	EmitVertex();
	

	EndPrimitive();
}