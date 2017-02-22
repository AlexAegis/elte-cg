#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

// A nagy�t�s �s az eltol�s m�rt�ke a CPU oldal�r�l j�nnek
// (billenty�lenyom�sra v�ltoztatjuk).
uniform float scale;
uniform vec2 offset;

// F�ggv�ny komplex sz�mok �sszeszorz�s�ra.
vec2 cpx_mul(vec2 u, vec2 v){
	return vec2(u.x * v.x - u.y * v.y,  // val�s r�sz
				u.x * v.y + u.y * v.x); // k�pzetes r�sz
}

// A Mandelbrot-halmaz defin�ci�ja:
// Egy z komplex sz�m akkor eleme a halmaznak, ha c = z eset�n a
// z_{n+1} = z{n}^2 + c kifejez�s hat�r�rt�ke v�ges.

// Egy (x,y) koordin�t�j� fregmens fog megfelelni egy adott
// z = x + i*y (m�sk�pp jel�lve: z = (x,y)) komplex sz�mnak.

void main()
{
	// A kezdeti (x,y) koordin�t�kon m�dos�tjuk:
	// a nagy�t�s m�rt�k�vel r�szorzunk, �s offset-tel eltoljuk.
	vec2 z = vs_out_pos.xy * scale + offset;
	vec2 c = z;

	// Legfeljebb 500-szor futtatjuk az iter�ci�t,
	// de ha z �rt�ke t�l gyorsan n�, hamarabb kil�p�nk.
	int i;
	for(i = 0; i < 500 && length(z) < 4; ++i){
		z = cpx_mul(z,z) + c;
	}

	// Ha most i == 500, akkor z kell�en kicsi maradt:
	// "konverg�ltunk", �s a fregmens a Mandelbrot-halmaz r�sze.
	// Ekkor a fregmens sz�ne legyen tiszta fekete.
	// Ha i < 500, akkor z �rt�ke t�l nagy lett (kiugrottunk a ciklusb�l):
	// a fregmens nem a Mandelbrot-halmaz r�sze, �s a sz�nt k�kre
	// �ll�tjuk. A sz�nez�shez felhaszn�ljuk i �rt�k�t:
	// i == 0 a tiszta feh�r, i == 499 a (majdnem) tiszta k�k.
	fs_out_col = vec4(1-i/500.0f, 1-i/500.0f, (i == 500) ? 0.0f : 1.0f, 1);

	// Egy �rdekes sz�nez�s, �rdemes kipr�b�lni:
	//float col = i / 500.0f;
	//fs_out_col = vec4(col, col*col, 1-(1-col)*(1-col), 1);

}