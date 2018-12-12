#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in float _dampened_iteration_count;

out vec4 fs_out_col;

void main()
{
	//fs_out_col = vec4(vs_out_pos, 1);

	float mul = 0- (_dampened_iteration_count * 10.15);

	fs_out_col = vec4(vs_out_pos * -mul, 1) ;
	
}