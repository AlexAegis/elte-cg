#version 400

in vec3 vs_out_pos;
out vec4 fs_out_col;

uniform float zoom;
uniform double x;
uniform double y;
uniform int max_iteration;

// Seg�df�ggv�ny a sz�nez�shez: HSV �rt�kekb�l RGB �rt�keket sz�mol
// H = Hue,			0 �s 360 k�z�tti �rt�k
// S = Saturation,	0 �s 1 k�z�tti �rt�k
// V = Value,		0 �s 1 k�z�tti �rt�k
vec3 HSVtoRGB(float h, float s, float v){
	
	// seg�dv�ltoz�k
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// kromatikus sz�rke
		return vec3(v,v,v);
	}

	h /= 60.0f;			// melyik tartom�nyba esik (0-5)
	i = int(floor( h ));
	f = h - i;			// h t�rt r�sze
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

	// a fregmens koordin�t�ja a komplex s�kon zoom �s eltol�s ut�n
	float x0 = vs_out_pos.x * zoom + float(x);
	float y0 = vs_out_pos.y * zoom + float(y);
	// az iter�ci�hoz haszn�lt �rt�kek
	float x_it = 0.0;
	float y_it = 0.0;
	int iteration = 0;
	// a Mandelbrot halmazhoz sz�ks�ges sz�m�t�s:
	// melyik komplex sz�m h�ny iter�ci� ut�n lesz "kell�en nagy"
	while(x_it * x_it + y_it * y_it < 4 && iteration < max_iteration){
		float xtemp = x_it * x_it - y_it * y_it + x0;
		y_it = 2 * x_it * y_it + y0;
		x_it = xtemp;
		iteration += 1;
	}
	
	// ha el�rte a max iter�ci� sz�mot, akkor r�sze a Mandelbrot halmaznak
	if(iteration == max_iteration){
		fs_out_col = vec4( 0, 0, 0, 1 );
	// k�l�nben az iter�ci� sz�mt�l f�gg�en sz�nez�nk
	} else {
		fs_out_col = vec4( HSVtoRGB(iteration % 360,1,1), 1 );
	}

}