#version 330 core

in vec3 vs_in_pos;

out vec3 vs_out_pos;
out vec3 vs_out_normal;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = vs_in_pos;
	vs_out_normal = vs_in_pos;
}
