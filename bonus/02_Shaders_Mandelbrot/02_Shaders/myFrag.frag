#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

// A nagyítás és az eltolás mértéke a CPU oldaláról jönnek
// (billentyûlenyomásra változtatjuk).
uniform float scale;
uniform vec2 offset;

// Függvény komplex számok összeszorzására.
vec2 cpx_mul(vec2 u, vec2 v){
	return vec2(u.x * v.x - u.y * v.y,  // valós rész
				u.x * v.y + u.y * v.x); // képzetes rész
}

// A Mandelbrot-halmaz definíciója:
// Egy z komplex szám akkor eleme a halmaznak, ha c = z esetén a
// z_{n+1} = z{n}^2 + c kifejezés határértéke véges.

// Egy (x,y) koordinátájú fregmens fog megfelelni egy adott
// z = x + i*y (másképp jelölve: z = (x,y)) komplex számnak.

void main()
{
	// A kezdeti (x,y) koordinátákon módosítjuk:
	// a nagyítás mértékével rászorzunk, és offset-tel eltoljuk.
	vec2 z = vs_out_pos.xy * scale + offset;
	vec2 c = z;

	// Legfeljebb 500-szor futtatjuk az iterációt,
	// de ha z értéke túl gyorsan nõ, hamarabb kilépünk.
	int i;
	for(i = 0; i < 500 && length(z) < 4; ++i){
		z = cpx_mul(z,z) + c;
	}

	// Ha most i == 500, akkor z kellõen kicsi maradt:
	// "konvergáltunk", és a fregmens a Mandelbrot-halmaz része.
	// Ekkor a fregmens színe legyen tiszta fekete.
	// Ha i < 500, akkor z értéke túl nagy lett (kiugrottunk a ciklusból):
	// a fregmens nem a Mandelbrot-halmaz része, és a színt kékre
	// állítjuk. A színezéshez felhasználjuk i értékét:
	// i == 0 a tiszta fehér, i == 499 a (majdnem) tiszta kék.
	fs_out_col = vec4(1-i/500.0f, 1-i/500.0f, (i == 500) ? 0.0f : 1.0f, 1);

	// Egy érdekes színezés, érdemes kipróbálni:
	//float col = i / 500.0f;
	//fs_out_col = vec4(col, col*col, 1-(1-col)*(1-col), 1);

}