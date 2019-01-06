#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külsõ paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 MVP;

void main()
{

	float pi = 3.1415f;
	float cylinder_u = vs_in_pos.x * 2 * pi;
	float cylinder_v = vs_in_pos.z;

	float r = 1;

	// hengerpalást - rajzolási mód GL_TRIANGLES a glDrawElements-nél
	vec3 cylinder = vec3(r * cos(cylinder_u), cylinder_v, r * -sin(cylinder_u));

	gl_Position = MVP * vec4( cylinder, 1 );

	vs_out_pos = vs_in_pos;
	vs_out_norm = vs_in_norm;
	vs_out_tex = vs_in_tex;
}