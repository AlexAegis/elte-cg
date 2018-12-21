#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec2 vs_in_ground_tex;
in vec3 vs_in_norm;
out vec3 vs_out_norm;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec2 vs_out_ground_tex;

// shader külsõ paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 MVP;

void main()
{
	float pi = 3.1415f;
	float x = vs_in_pos[0];
	float y = vs_in_pos[2];
	float u = x * pi / 0.4 - 3.5;
	float v = y * pi / 0.4 - 3.5; 
	float cu = cos(u);
	float su = sin(u);
	float cv = cos(v);
	float sv = sin(v);
	float r = 8;
	float r_o = 10;
	float h = 1.4;
	float circ = ( (x - u) * (x - u)) + ((y - v) * (y - v));
	if( circ > r - 0.2 && circ < r + 0.2 ||  circ > r_o - 0.2 && circ < r_o + 0.2) {
		h = 1;
	}
	vec3 inner_circle = vec3(x, h, y);

	//


	gl_Position = MVP * vec4( inner_circle, 1 );

	vs_out_pos = vs_in_pos;
	vs_out_ground_tex = vs_in_ground_tex;
	vs_out_norm = normalize(inner_circle);
}


/*OG 

float pi = 3.1415f;
	float x = vs_in_pos[0] - 0.5;
	float y = vs_in_pos[2] - 0.5;
	float u = x * 2* pi;
	float v = y * 2 * pi; 
	float cu = cos(u);
	float su = sin(u);
	float cv = cos(v);
	float sv = sin(v);
	float r = 4.5;
	float r_o = 5.5;
	float h = 1.2;
	float circ = ( (x - u) * (x - u)) + ((y - v) * (y - v));
	if( circ > r - 0.2 && circ < r + 0.2 ||  circ > r_o - 0.2 && circ < r_o + 0.2) {
		h = 1;
	}
	vec3 inner_circle = vec3(x, h, y);

	//


	gl_Position = MVP * vec4( inner_circle, 1 );

	vs_out_pos = vs_in_pos;
	vs_out_ground_tex = vs_in_ground_tex;

	*/