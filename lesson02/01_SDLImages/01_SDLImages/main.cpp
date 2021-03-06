#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	// �ll�tsuk be, hogy kil�p�s el�tt h�vja meg a rendszer az exitProgram() f�ggv�nyt - K�rd�s: mi lenne en�lk�l?
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

	int wh = 640, ww = 480;

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello HZ!",				// az ablak fejl�ce
							100,						// az ablak bal-fels� sark�nak kezdeti X koordin�t�ja
							100,						// az ablak bal-fels� sark�nak kezdeti Y koordin�t�ja
							wh,						// ablak sz�less�ge
							ww,						// �s magass�ga
							SDL_WINDOW_SHOWN);			// megjelen�t�si tulajdons�gok

	// ha nem siker�lt l�trehozni az ablakot, akkor �rjuk ki a hib�t, amit kaptunk �s l�pj�nk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. l�p�s: hozzunk l�tre egy renderel�t, rajzol�t
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendelj�k hozz� a renderert
								-1,  // melyik index� renderert inicializ�ljuka
									 // a -1 a harmadik param�terben meghat�rozott ig�nyeinknek megfelel� els� renderel�t jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az ig�nyeink, azaz
																						// hardveresen gyors�tott �s vsync-et bev�r�
    if (ren == nullptr)
	{
        std::cout << "[Renderer l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. l�p�s: t�lst�nk be egy k�pf�jlt
	//
	SDL_Texture* tex = IMG_LoadTexture(ren, "hz.png" );
	SDL_Texture* tex2 = IMG_LoadTexture(ren, "kata.png");
	if ( tex == nullptr)
	{
        std::cout << "[K�p bet�lt�se] Hiba: " << IMG_GetError() << std::endl;
		SDL_DestroyWindow( win );
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

	 Sint32 x = 100, y = 100, hs = 2, vs = 2, hd = 1, vd = 1;
	 Sint32 x2 = 400, y2 = 400, hs2 = 2, vs2 = 2, hd2 = -1, vd2 = -1;

	while (!quit)
	{
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
				// eg�rgomb felenged�s�nek esem�nye; a felengedett gomb a ev.button.button -ban tal�lhat�
				// a lehets�ges gombok a k�vetkez�ek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				break;
			}
		}

		// t�r�lj�k a h�tteret feh�rre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// rajzoljuk ki a bet�lt�tt k�pet az eg�kurzor k�r�!
		int tex_width, tex_height;
		int tex_width2, tex_height2;
		SDL_QueryTexture(tex2, nullptr, nullptr, &tex_width2, &tex_height2);
		SDL_QueryTexture( tex, nullptr, nullptr, &tex_width, &tex_height);


		SDL_Rect cursor_rect; 
		cursor_rect.w = tex_width;
		cursor_rect.h = tex_height;
		cursor_rect.x = x - tex_width /2;
		cursor_rect.y = y - tex_height /2;

		SDL_Rect cursor_rect2;
		cursor_rect2.w = tex_width;
		cursor_rect2.h = tex_height;
		cursor_rect2.x = (x2 - tex_width2 / 2);
		cursor_rect2.y = (y2 - tex_height2 / 2);

		SDL_RenderCopy(ren,
			tex2,
			nullptr,
			&cursor_rect2);

		SDL_RenderCopy( ren,				// melyik renderel�re rajzoljunk
						tex,				// melyik text�r�t rajzoljuk r�
						nullptr,			// a text�ra melyik al-rect-j�t
						&cursor_rect );		// a renderel� fel�let�nek mely r�sz�re


		// 1. feladat: pattogtassuk a k�perny�n a kirajzolt k�pet! Induljon el a k�p
		//    a k�perny� k�zep�r�l egy ir�nyba �s amikor valamelyik sz�le az ablak sz�l�hez
		//    k�zeledik, pattanjon vissza!

	
		x = x + hs * hd;
		y = y + vs * vd;

		x2 = x2 + hs2 * hd2;
		y2 = y2 + vs2 * vd2;

		if (x > ww - (tex_width / 4 ) || x < 0 + (tex_width / 4)) {
			hd = hd * -1;
		}
		if (y > wh - (tex_height * 1.5) || y < 0 ) {
			vd = vd * -1;
		}

		if (x2 > ww - (tex_width2 / 4) || x2 < 0 + (tex_width2 / 4)) {
			hd2 = hd2 * -1;
		}
		if (y2 > wh - (tex_height2 * 1.5) || y2 < 0) {
			vd2 = vd2 * -1;
		}


		// 2. feladat: az animation_sheet.gif-ben tal�lhat� anim�ci�t rajzoljuk ki �gy,
		//    hogy a fut�s egy f�zisa 3 mp-ig tartson! Tipp: a source rect-et kell m�dos�tani!

		// 3. feladat: ne legyen pattog�s, a felhaszn�l� tudjon jobbra-balra futtatni a 
		//    figur�t a balra/jobbra billenty�k lenyom�s�val. Ha elengedi a billenty�t,
		//    akkor �lljon meg a figura, ahol van (tipp: kell �j k�pf�jl)

		// jelen�ts�k meg a backbuffer tartalm�t
		SDL_RenderPresent(ren);
	}



	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyTexture( tex );
	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}