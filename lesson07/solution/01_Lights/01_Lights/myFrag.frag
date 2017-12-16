#version 130

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_normal;
//flat in vec3 vs_out_normal;
in vec2 vs_out_tex0;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

//
// uniform változók
//
uniform vec3 m_eyePos; // = vec3(1.0, 1.0, 1.0);


// színtér tulajdonságok
vec4 La = vec4(0.5, 0.6, 0.4, 1.0);	//KOMMENT: ambiens fény színe (tört fény)
vec4 Ld = vec4(0.8, 0.1, 0.2, 1.0);	//KOMMENT: directional fény színe ("nap sugarai" --> párhuzamosak)
vec4 Ls = vec4(1.0, 1.0, 1.0, 1.0);	//KOMMENT: spekuláris (csillanó) fény színe


// fénytulajdonságok
vec3 lightDir = normalize(vec3(1.0, -1.0, 0.0));


// anyagtulajdonságok
vec4 Ka = vec4(1.0, 1.0, 1.0, 1.0);	//KOMMENT: tárgy fényvisszaverõ tulajdonságai...
vec4 Kd = vec4(1.0, 1.0, 1.0, 1.0);
vec4 Ks = vec4(1.0, 1.0, 1.0, 1.0);


uniform sampler2D texture;

void main()
{
	//
	// ambiens szín számítása
	//
	vec4 ambient = La * Ka;

	//
	// diffúz szín számítása
	//
	vec3 normal = normalize(vs_out_normal);
	vec3 dir = -lightDir;
	float di = clamp(dot(normal,dir), 0.0, 1.0);	//KOMMENT: 1 ha egybe esnek, 0 ha merõlegesek
													//KOMMENT: a "dot" = skaláris szorzás: A*B = ||A|| * ||B|| * cos(bezárt_szög)
													//		--> a cos -1 és 1 között mozog ==> clamp
													//			, azaz: a mögöttünk levõ fény is nulla intenzitású
	vec4 diffuse = Ld * Kd * di;
	
	//
	// fényfoltképzõ szín
	//
	vec3 r = reflect(lightDir, normal);
	vec3 c = normalize(m_eyePos - vs_out_pos);
	float si = pow(clamp(dot(r,c), 0.0, 1.0), 16.0);
	vec4 specular = Ls * Ks * si;

	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/
	
	//
	// a fragment végsõ színének meghatározása
	//
	fs_out_col = (specular + diffuse + ambient) * texture2D(texture, vs_out_tex0.st);
}