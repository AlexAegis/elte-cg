#version 130

in vec3 vs_out_col;
in vec2 vs_out_tex0;
out vec4 fs_out_col;

uniform sampler2D texImage;


//1.:
uniform vec2 offset;


void main()
{
	//fs_out_col = vec4(vs_out_col, 1) * texture(texImage, vs_out_tex0.st);

	//1.:
	fs_out_col = vec4(vs_out_col, 1) * texture(texImage, vec2(vs_out_tex0.s-offset.s,vs_out_tex0.t+offset.t));
	//KOMMENT: -s ==> jobbra viszi
	//KOMMENT: +t ==> lefel� viszi
}

//1.: az alapsz�n maradjon egy helyben, de a zaj text�ra mozogjon folyamatosan jobbra �s lefel�
// MyApp.h --> protected
// MyApp.cpp --> Init (v�ge fel�)
// MyApp.cpp --> Render (k�zepe fel�)
