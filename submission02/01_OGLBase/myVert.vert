#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out float _dampened_iteration_count;

// shader külsõ paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 MVP;

void main()
{

	float x0 = vs_in_pos[0] * 2.4 - 1.5;
	float y0 = vs_in_pos[1] * 2.4 - 1.2;
	float x = 0.0;
	float y = 0.0;
	int iteration = 0;
	int max_iteration = 14000;
	bool found = false;
	while (x*x + y*y < 2*2  &&  iteration < max_iteration && !found) {
		float xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		if(abs(x) > 2) {
			found = true;
		}
		iteration = iteration + 1;
	}
	float dampened_iteration_count = 0.1*exp(-0.04 * iteration);
	float dampened_x = x / max_iteration;
	float task = iteration / max_iteration;
	vs_out_pos = vs_in_pos;
	_dampened_iteration_count = dampened_iteration_count;
	gl_Position = MVP * vec4(vs_in_pos[0], vs_in_pos[1], dampened_iteration_count /* a második képen a task a magasságtérkép az elsőn a dampened_iteration_count */ , 0.2); // -0.1*exp(-0.1 * iteration)

}