#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D m_texture_wheel;

void main()
{
	fs_out_col = texture(m_texture_wheel, vec2(vs_out_tex[0] / 4 / 1.1 - 0.25, vs_out_tex[1] / 2 / 1.1 - 0.5));


}