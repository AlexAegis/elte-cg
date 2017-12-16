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
	//KOMMENT: +t ==> lefelé viszi
}

//1.: az alapszín maradjon egy helyben, de a zaj textúra mozogjon folyamatosan jobbra és lefelé
// MyApp.h --> protected
// MyApp.cpp --> Init (vége felé)
// MyApp.cpp --> Render (közepe felé)
