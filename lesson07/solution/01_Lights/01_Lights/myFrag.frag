#version 130

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_normal;
//flat in vec3 vs_out_normal;
in vec2 vs_out_tex0;

// kimen� �rt�k - a fragment sz�ne
out vec4 fs_out_col;

//
// uniform v�ltoz�k
//
uniform vec3 m_eyePos; // = vec3(1.0, 1.0, 1.0);


// sz�nt�r tulajdons�gok
vec4 La = vec4(0.5, 0.6, 0.4, 1.0);	//KOMMENT: ambiens f�ny sz�ne (t�rt f�ny)
vec4 Ld = vec4(0.8, 0.1, 0.2, 1.0);	//KOMMENT: directional f�ny sz�ne ("nap sugarai" --> p�rhuzamosak)
vec4 Ls = vec4(1.0, 1.0, 1.0, 1.0);	//KOMMENT: spekul�ris (csillan�) f�ny sz�ne


// f�nytulajdons�gok
vec3 lightDir = normalize(vec3(1.0, -1.0, 0.0));


// anyagtulajdons�gok
vec4 Ka = vec4(1.0, 1.0, 1.0, 1.0);	//KOMMENT: t�rgy f�nyvisszaver� tulajdons�gai...
vec4 Kd = vec4(1.0, 1.0, 1.0, 1.0);
vec4 Ks = vec4(1.0, 1.0, 1.0, 1.0);


uniform sampler2D texture;

void main()
{
	//
	// ambiens sz�n sz�m�t�sa
	//
	vec4 ambient = La * Ka;

	//
	// diff�z sz�n sz�m�t�sa
	//
	vec3 normal = normalize(vs_out_normal);
	vec3 dir = -lightDir;
	float di = clamp(dot(normal,dir), 0.0, 1.0);	//KOMMENT: 1 ha egybe esnek, 0 ha mer�legesek
													//KOMMENT: a "dot" = skal�ris szorz�s: A*B = ||A|| * ||B|| * cos(bez�rt_sz�g)
													//		--> a cos -1 �s 1 k�z�tt mozog ==> clamp
													//			, azaz: a m�g�tt�nk lev� f�ny is nulla intenzit�s�
	vec4 diffuse = Ld * Kd * di;
	
	//
	// f�nyfoltk�pz� sz�n
	//
	vec3 r = reflect(lightDir, normal);
	vec3 c = normalize(m_eyePos - vs_out_pos);
	float si = pow(clamp(dot(r,c), 0.0, 1.0), 16.0);
	vec4 specular = Ls * Ks * si;

	/* seg�ts�g:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/
	
	//
	// a fragment v�gs� sz�n�nek meghat�roz�sa
	//
	fs_out_col = (specular + diffuse + ambient) * texture2D(texture, vs_out_tex0.st);
}