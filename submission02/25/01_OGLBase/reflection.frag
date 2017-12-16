#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_normal;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;
uniform vec3 camera_pos;

void main()
{
	fs_out_col = vec4(vs_out_pos, 1);
}
