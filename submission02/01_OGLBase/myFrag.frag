#version 330 core



uniform sampler2D tex_a;
uniform sampler2D tex_b;
uniform float transition;

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in float _dampened_iteration_count;

out vec4 fs_out_col;

void main()
{
	float mul = 0- (_dampened_iteration_count * 10.15);



	vec4 tca = texture( tex_a, vec2(vs_out_pos[0], vs_out_pos[1])) * transition;
	
	vec4 tcb = texture( tex_b, vec2(vs_out_pos[0], vs_out_pos[1])) * (2 - transition);
	
	fs_out_col =  vec4(vs_out_pos * -mul, 1) * (tcb + tca);
}