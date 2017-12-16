#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;

out vec4 fs_out_col;

//2.
uniform vec3 m_color;

//3.3.
uniform float m_time;

//4.
vec2 complex_square(vec2 u) {
	float real = u.x*u.x - u.y*u.y;
	float imaginary = u.x*u.y + u.y*u.x;
	return vec2(real, imaginary);
}

void main()
{
	//fs_out_col = vec4(vs_out_col, 1);

	//1.
	//fs_out_col = vec4(1, 1, 1, 1);

	//2.
	//fs_out_col = vec4(m_color, 1);

	//3.1.
	//float circle_equation_value = sqrt( pow(vs_out_pos.x, 2) + pow(vs_out_pos.y, 2) );
	//if ( circle_equation_value > 1 )
	//	discard; //KOMMENT: eldobjuk a fragment sz�n�t ==> a t�rl�si sz�n fog l�tszani
	//fs_out_col = vec4(m_color, 1);

	//3.2.
	//float circle_equation_value = sqrt( pow(vs_out_pos.x, 2) + pow(vs_out_pos.y, 2) );
	//if ( circle_equation_value > 1 || circle_equation_value < 0.995 )
	//	discard;
	//fs_out_col = vec4(m_color, 1);

	//3.3.
	//float circle_equation_value = sqrt( pow(vs_out_pos.x, 2) + pow(vs_out_pos.y, 2) );
	//float radius = (sin(2 * 3.1415 * m_time / 1000.0) + 1) / 2.0;
	//if ( circle_equation_value > radius || circle_equation_value < radius - 0.005 )
	//	discard;
	//fs_out_col = vec4(m_color, 1);

	//4.
	vec2 x = vs_out_pos.xy;
	vec2 c = x;
	for (int i=0; i<20; ++i) {
		x = complex_square(x) + c;
	}
	if (length(x) > 1)
		discard;
	fs_out_col = vec4(m_color, 1);
}

// 1. feladat: rajzoljuk ki feh�rrel a t�glalapot!
// --> "myFrag.frag"

// 2. feladat: uniform v�ltoz�k - az alkalmaz�s �ll�tsa be, hogy milyen sz�nnel t�lts�nk ki!
// --> "myApp.h"::"protected:"
// --> "myApp.cpp"::"Init()"
// --> "myApp.cpp"::"Update()"
// --> "myApp.cpp"::"Render()"
// --> "myFrag.frag"

// 3. feladat: rajzoljuk ki az orig� k�z�ppont�, 1 sugar� k�rt (Eml.: discard eldobja a fragmentet)
// --> "myFrag.frag"
// 3.1. feladat: csak a k�rvonalat rajzoljuk ki
// --> "myFrag.frag"
// 3.3. feladat: a k�rvonal sugara 1 m�sodperc alatt periodikusan v�ltozzon 1 �s 0.5 k�z�tt
// --> "myApp.h"::"protected:"
// --> "myApp.cpp"::"Init()"
// --> "myApp.cpp"::"Update()"
// --> "myApp.cpp"::"Render()"
// --> "myFrag.frag"

// 4. feladat: Mandelbrot frakt�l (komplex sz�mok)
// --> "myFrag.frag"
// KOMMENT: Mandelbrot frakt�l szab�lya: google... --> wikipedia...
