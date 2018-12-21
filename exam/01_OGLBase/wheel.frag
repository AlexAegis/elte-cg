#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
out vec4 fs_out_col;

uniform sampler2D m_texture_wheel;

void main()
{
	//fs_out_col = vec4(vs_out_pos, 1);
		fs_out_col =  texture(m_texture_wheel, vec2(vs_out_pos.x / 1 + 0.5, vs_out_pos.z));
}