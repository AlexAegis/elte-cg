#include <SDL.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}

void rotate(int a[4]) {
	int temp = a[0];
	a[0] = a[1];
	a[1] = a[2];
	a[2] = a[3];
	a[3] = temp;
}

void copyArray(int a[3], int b[3]) {
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. l�p�s: inicializ�ljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezz�k �s l�pj�n ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL ind�t�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. l�p�s: hozzuk l�tre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = 0;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejl�ce
							100,						// az ablak bal-fels� sark�nak kezdeti X koordin�t�ja
							100,						// az ablak bal-fels� sark�nak kezdeti Y koordin�t�ja
							640,						// ablak sz�less�ge
							480,						// �s magass�ga
							SDL_WINDOW_SHOWN);			// megjelen�t�si tulajdons�gok

	// ha nem siker�lt l�trehozni az ablakot, akkor �rjuk ki a hib�t, amit kaptunk �s l�pj�nk ki
    if (win == 0)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. l�p�s: hozzunk l�tre egy renderel�t, rajzol�t
	//

    SDL_Renderer *ren = 0;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendelj�k hozz� a renderert
								-1,  // melyik index� renderert inicializ�ljuka
									 // a -1 a harmadik param�terben meghat�rozott ig�nyeinknek megfelel� els� renderel�t jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az ig�nyeink, azaz
																						// hardveresen gyors�tott �s vsync-et bev�r�
    if (ren == 0)
	{
        std::cout << "[Renderer l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. l�p�s: ind�tsuk el a f� �zenetfeldolgoz� ciklust
	// 

	// v�get kell-e �rjen a program fut�sa?
	bool quit = false;
	// feldolgozand� �zenet ide ker�l
	SDL_Event ev;
	// eg�r X �s Y koordin�t�i
	Sint32 mouseX = 0, mouseY = 0;
	
	int redColours[3] = { 255, 20, 20 };
	int blueColours[3] = { 20, 20, 255 };
	int actualColours[3];
	copyArray(actualColours, redColours);
	int sizes[4] = { 20, 40, 80, 40 };
	int time = 0;
	while (!quit)
	{
		time++;
		if (time > 1000) {
			time = 0;
			rotate(sizes);
		}
		// am�g van feldolgozand� �zenet dolgozzuk fel mindet:
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
					quit = true;
				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT) {
					copyArray(actualColours, redColours);
				}
				else if (ev.button.button == SDL_BUTTON_RIGHT) {
					copyArray(actualColours, blueColours);
				}
				// eg�rgomb felenged�s�nek esem�nye; a felengedett gomb a ev.button.button -ban tal�lhat�
				// a lehets�ges gombok a k�vetkez�ek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				break;
			}
		}

		// t�r�lj�k a h�tteret feh�rre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktu�lis rajzol�si sz�n legyen z�ld �s rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer c�me, aminek a rajzol�si sz�n�t be akarjuk �ll�tani
								0,		// piros
								255,	// z�ld
								0,		// k�k
								255);	// �tl�tszatlans�g

		SDL_RenderDrawLine(	ren,	// renderer c�me, ahov� vonalat akarunk rajzolni
							0, 0, // vonal kezd�pontj�nak (x,y) koordin�t�i
							mouseX, mouseY);// vonal v�gpontj�nak (x,y) koordin�t�i
		
		// defini�ljunk egy (mouseX, mouseY) k�z�ppont�, tengelyekkel p�rhuzamos oldal�
		// 20x20-as n�gyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - sizes[0] / 2;
		cursor_rect.y = mouseY - sizes[0] / 2;
		cursor_rect.w = sizes[0];
		cursor_rect.h = sizes[0];
		// legyen a kit�lt�si sz�n piros
		SDL_SetRenderDrawColor( ren, actualColours[0], actualColours[1], actualColours[2], 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt id� f�ggv�ny�ben periodikusan n�jj�n �s cs�kkenjen
		//    az eg�rmutat� k�z�ppontj�val kirajzolt n�gysz�g

		// 2. feladat: ha a user a bal eg�rgombot nyomja meg akkor a t�glalap sz�ne v�ltson pirosra,
		//    ha a jobb eg�rgombot, akkor k�kre

		// 3. feladat: rajzolj ki egy 50 sugar� k�rvonalat az eg�rmutat� k�r�!
		// seg�ts�g: haszn�ld a SDL_RenderDrawLines()-t

		// jelen�ts�k meg a backbuffer tartalm�t
		SDL_RenderPresent(ren);
	}



	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}