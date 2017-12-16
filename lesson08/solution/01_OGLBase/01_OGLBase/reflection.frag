#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_normal;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;
uniform vec3 camera_pos;

void main()
{
	fs_out_col = texture( skyboxTexture, vs_out_pos );

	//MEGJEGYZ�S:
	//		1. Most a normalt a vertex shaderben a vs_in_pos-zal adtuk meg, de
	//		   ha elmozgatn�nk p�ld�ul a g�mb�nket, akkor m�r nem haszn�lhatn�nk
	//		   ezt a m�dszert, teh�t minden egy�be esetben a fel�leti norm�lisokat
	//		   is fel kell t�lteni a vertex shader fel�...
	//		2. Hogy val�ban a skybox t�kr�z�dik-e, n�zz�tek meg, hogy p�ld�ul a
	//		   g�mb sz�le fel� "torzul" a "textr�ra", mikor mozog a kamera...

	vec3 v_camera = vs_out_pos - camera_pos;
	vec3 v_reflected = reflect( v_camera, vs_out_normal );

	//fs_out_col = texture( skyboxTexture, v_reflected );
}
