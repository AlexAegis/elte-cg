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
	float circle_u = vs_in_pos.x * 2 * pi;
	float circle_v = vs_in_pos.z * 2;

	float r = 1;
	
	vec3 circle = vec3( r * cos(circle_u), 0, r * sin(circle_u));

	gl_Position = MVP * vec4( circle, 1 );

	vs_out_pos = vs_in_pos;
	vs_out_norm = vs_in_norm;
	vs_out_tex = circle.xz / r;
}