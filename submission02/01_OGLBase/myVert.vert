#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;

// shader külsõ paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 MVP;

void main()
{
	//gl_Position = MVP * vec4( vs_in_pos, 1 );
	//gl_Position = MVP * vec4( vec3(vs_in_pos[0], vs_in_pos[1], vs_in_pos[2]), 1 );
	/* gömböcske
	float u = vs_in_pos[0] * 2 * 3.1415f;
	float v = vs_in_pos[1] * 3.1415f;
	float cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	float r = 2;
	gl_Position = MVP * vec4(vec3(r*cu*sv, r*cv, r*su*sv), 1);
	vs_out_pos = vs_in_pos;
	*/


	float x0 = vs_in_pos[0] * 2.4 - 1.5; //scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.5, 1))
	float y0 = vs_in_pos[1] * 2.4 - 1.2; //scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1, 1))
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
	float dampened_iteration_count = 1.4*exp(-0.02 * iteration);
	vs_out_pos = vec3(vs_in_pos[0] / dampened_iteration_count * 1.2, vs_in_pos[1]/ dampened_iteration_count * 0.6, vs_in_pos[2] / dampened_iteration_count) * 1.2; // color = palette[iteration]

	gl_Position = MVP * vec4(vs_in_pos[0], vs_in_pos[1], -0.1*exp(-0.1 * iteration), 0.2);

	// plot(Px, Py, color)
}