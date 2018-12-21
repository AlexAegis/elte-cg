#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
out vec3 vs_out_norm;
// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;

// shader k�ls� param�terei - most a h�rom transzform�ci�s m�trixot k�l�n-k�l�n vessz�k �t
uniform mat4 MVP;

void main()
{


	float pi = 3.1415f;
	float rad = 2*pi;
	float x = vs_in_pos[0];
	float y = vs_in_pos[2];
	float u = vs_in_pos[0]*2*pi;
	float v = vs_in_pos[2]*pi;
	float cu = cos(u);
	float su = sin(u);
	float cv = cos(v);
	float sv = sin(v);
	float r = 2;
	//vec3 m = vec3( r*cu*sv, r*cv, r*su*sv );
	//vec3 m = vec3( r*cu*sv, r*cv, r);

	// k�z�ppont
	float x0 = 0;
	float y0 = 0;
	float xd = x - x0;
	float yd = y - y0;
	float d = sqrt(xd * xd + yd * yd);

	//vec3 m = vec3( x0 + sin(rad * (d / (2 * r * pi))) * r, 10, y0 + cos(rad * (d / (2 * r * pi))) * r);
		vec3 m = vec3( r*cu*sv, r*cv, 1 );
	gl_Position = MVP * vec4( m, 1 );

	//gl_Position = MVP * vec4( vs_in_pos, 0.3 );
	vs_out_pos = vs_in_pos;
	vs_out_norm = normalize(m);
}

//  r(fi)={u+R*cos(fi),v+R*sin(fi)}.


/* K�R

	float pi = 3.1415f;
	float u = vs_in_pos[0]*2*pi;
	float v = vs_in_pos[2]*pi;
	float cu = cos(u);
	float su = sin(u);
	float cv = cos(v);
	float sv = sin(v);
	float r = 2;
	//vec3 m = vec3( r*cu*sv, r*cv, r*su*sv );
	vec3 m = vec3( r*cu*sv, r*cv, 1 );
	gl_Position = MVP * vec4( m, 1 );

	vs_out_pos = vs_in_pos;
*/