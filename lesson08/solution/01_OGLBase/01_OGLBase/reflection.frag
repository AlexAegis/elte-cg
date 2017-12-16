#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_normal;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;
uniform vec3 camera_pos;

void main()
{
	fs_out_col = texture( skyboxTexture, vs_out_pos );

	//MEGJEGYZÉS:
	//		1. Most a normalt a vertex shaderben a vs_in_pos-zal adtuk meg, de
	//		   ha elmozgatnánk például a gömbünket, akkor már nem használhatnánk
	//		   ezt a módszert, tehát minden egyébe esetben a felületi normálisokat
	//		   is fel kell tölteni a vertex shader felé...
	//		2. Hogy valóban a skybox tükrözõdik-e, nézzétek meg, hogy például a
	//		   gömb széle felé "torzul" a "textrúra", mikor mozog a kamera...

	vec3 v_camera = vs_out_pos - camera_pos;
	vec3 v_reflected = reflect( v_camera, vs_out_normal );

	//fs_out_col = texture( skyboxTexture, v_reflected );
}
