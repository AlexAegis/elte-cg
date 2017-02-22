#version 400

in vec3 vs_out_pos;
out vec4 fs_out_col;

uniform float zoom;
uniform double x;
uniform double y;
uniform int max_iteration;

// Segédfüggvény a színezéshez: HSV értékekbõl RGB értékeket számol
// H = Hue,			0 és 360 közötti érték
// S = Saturation,	0 és 1 közötti érték
// V = Value,		0 és 1 közötti érték
vec3 HSVtoRGB(float h, float s, float v){
	
	// segédváltozók
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// kromatikus szürke
		return vec3(v,v,v);
	}

	h /= 60.0f;			// melyik tartományba esik (0-5)
	i = int(floor( h ));
	f = h - i;			// h tört része
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			return vec3(v,t,p);
		case 1:
			return vec3(q,v,p);
		case 2:
			return vec3(p,v,t);
		case 3:
			return vec3(p,q,v);
		case 4:
			return vec3(t,p,v);
		default:		// case 5:
			return vec3(v,p,q);
	}
}

void main()
{

	// a fregmens koordinátája a komplex síkon zoom és eltolás után
	float x0 = vs_out_pos.x * zoom + float(x);
	float y0 = vs_out_pos.y * zoom + float(y);
	// az iterációhoz használt értékek
	float x_it = 0.0;
	float y_it = 0.0;
	int iteration = 0;
	// a Mandelbrot halmazhoz szükséges számítás:
	// melyik komplex szám hány iteráció után lesz "kellõen nagy"
	while(x_it * x_it + y_it * y_it < 4 && iteration < max_iteration){
		float xtemp = x_it * x_it - y_it * y_it + x0;
		y_it = 2 * x_it * y_it + y0;
		x_it = xtemp;
		iteration += 1;
	}
	
	// ha elérte a max iteráció számot, akkor része a Mandelbrot halmaznak
	if(iteration == max_iteration){
		fs_out_col = vec4( 0, 0, 0, 1 );
	// különben az iteráció számtól függõen színezünk
	} else {
		fs_out_col = vec4( HSVtoRGB(iteration % 360,1,1), 1 );
	}

}