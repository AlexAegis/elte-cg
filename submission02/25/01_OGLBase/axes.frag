#version 330 core

in vec3 vs_out_pos;

vec3 color = vec3( 0.0, 0.0, 0.0 );

out vec4 fs_out_col;

void main()
{
	
	if (vs_out_pos.x > 0.0)
		color.r = 1.0;
	if (vs_out_pos.y > 0.0)
		color.g = 1.0;
	if (vs_out_pos.z > 0.0)
		color.b = 1.0;

	fs_out_col = vec4(color, 1);
}