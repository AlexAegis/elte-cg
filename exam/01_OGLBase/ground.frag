#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_ground_tex;
in vec3 vs_out_norm;

out vec4 fs_out_col;

uniform sampler2D m_texture_ground;

// színtér tulajdonságok
uniform vec3 eye_pos = vec3(0, 15, 15);

// fénytulajdonságok
uniform vec3 light_pos = vec3( 5, 5, 5 );

uniform vec4 La = vec4(0.1f, 0.1f, 0.1f, 1);
uniform vec4 Ld = vec4(0.75f, 0.75f, 0.75f, 1);
uniform vec4 Ls = vec4(1, 1, 1, 1);

// anyagtulajdonságok
uniform vec4 Ka = vec4(1, 1, 1, 0);
uniform vec4 Kd = vec4(0.75f, 0.25f, 0.125f, 1);
uniform vec4 Ks = vec4(0, 1, 0, 0);
uniform float specular_power = 32;

void main()
{
	//fs_out_col = vec4(vs_out_pos, 1) * texture(m_texture_ground, vs_out_ground_tex.st) ;
	//fs_out_col =  texture(m_texture_ground, vs_out_pos.xz);



	vec4 ambient = La * Ka;
	vec3 normal = normalize( vs_out_norm );
	vec3 toLight = normalize(light_pos - vs_out_pos);
	float di = clamp( dot( toLight, normal), 0.0f, 1.0f );
	vec4 diffuse = vec4(Ld.rgb*Kd.rgb*di, Kd.a);


	fs_out_col =  ambient * diffuse * 100 * vec4(vec3(201 / 255, 226 / 255, 255 / 255), 1) * texture(m_texture_ground, vs_out_pos.xz);

	//fs_out_col =  ambient * diffuse * 100 *  texture(m_texture_ground, vs_out_pos.xz);
}