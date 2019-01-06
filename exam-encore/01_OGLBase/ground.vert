#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei - most a h�rom transzform�ci�s m�trixot k�l�n-k�l�n vessz�k �t
uniform mat4 MVP;


void main()
{

	// s�np�r
	float h = vs_in_pos.y;
	float circ = sqrt(pow(vs_in_pos.x - 0.5, 2) + pow(vs_in_pos.z - 0.5, 2));
	float inner_r = 8.0f / 20.0f; // 0.4f a terulet 20 szeles es magasra lett kint skalazva
	float outer_r = 9.0f / 20.0f; // 0.4f a terulet 20 szeles es magasra lett kint skalazva
	float rail_width = 0.4f / 20.0f;
	if(circ >= inner_r - rail_width / 2 && circ <= inner_r + rail_width / 2
		|| circ >= outer_r - rail_width / 2 && circ <= outer_r + rail_width / 2) { 
		h = h + 0.2;
	}

	gl_Position = MVP * vec4( vec3(vs_in_pos.x, h, vs_in_pos.z), 1 );


	vs_out_pos = vs_in_pos;
	vs_out_norm = vs_in_norm;
	vs_out_tex = vs_in_tex;
}


/*

	float pi = 3.1415f;
	float globe_u = vs_in_pos.x * 2 * pi;
	float globe_v = vs_in_pos.z * pi;
	float circle_u = vs_in_pos.x * 2 * pi; //
	float circle_v = vs_in_pos.z * 2; //
	float cylinder_u = vs_in_pos.x * 2 * pi; //
	float cylinder_v = vs_in_pos.z; // magass�ga a cilindernek
	float torus_u = vs_in_pos.x * -2 * pi;
	float torus_v = vs_in_pos.z * 2 * pi;

	float r = 2;
	float torus_width_radius = 1;
	
	// g�mb - rajzol�si m�d GL_TRIANGLES a glDrawElements-n�l
	vec3 globe = vec3(cos(globe_u) * sin(globe_v), cos(globe_v), sin(globe_u)*sin(globe_v));

	// k�r - rajzol�si m�d GL_TRIANGLE_FAN a glDrawElements-n�l
	vec3 circle = vec3( r * cos(circle_u), 0, r * sin(circle_u));
	// vs_out_tex = circle.xz / r;
	// gl_Position = MVP * vec4(circle, 1);

	// hengerpal�st - rajzol�si m�d GL_TRIANGLES a glDrawElements-n�l
	vec3 cylinder = vec3(r * cos(cylinder_u), cylinder_v, r * -sin(cylinder_u));

	// t�rus - rajzol�si m�d GL_TRIANGLES a glDrawElements-n�l
	vec3 torus = vec3((r + torus_width_radius * cos(torus_v)) * cos(torus_u), torus_width_radius * sin(torus_v), (r + torus_width_radius * cos(torus_v)) * sin(torus_u));


	*/