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

	const int win_width = 640;
	const int win_height = 480;
	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejl�ce
							100,						// az ablak bal-fels� sark�nak kezdeti X koordin�t�ja
							100,						// az ablak bal-fels� sark�nak kezdeti Y koordin�t�ja
							win_width,						// ablak sz�less�ge
							win_height,						// �s magass�ga
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
	//SDL_Texture* tex = IMG_LoadTexture( ren, "kep.png" );
	SDL_Texture* tex = IMG_LoadTexture(ren, "animation_sheet.gif");
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
	// KOMMENT: jobbra vagy balra men�s
	bool left = false;
	bool right = false;


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
				if ( ev.key.keysym.sym == SDLK_LEFT )
					left = true;
				if ( ev.key.keysym.sym == SDLK_RIGHT )
					right = true;
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
		// KOMMENT:       texture,  format,  access,      width,      height
		SDL_QueryTexture(     tex, nullptr, nullptr, &tex_width, &tex_height);

		SDL_Rect cursor_rect; 
		cursor_rect.w = tex_width;
		cursor_rect.h = tex_height;
		/*
		cursor_rect.x = mouseX - tex_width /2;
		cursor_rect.y = mouseY - tex_height /2;

		SDL_RenderCopy( ren,				// melyik renderel�re rajzoljunk
						tex,				// melyik text�r�t rajzoljuk r�
						nullptr,			// a text�ra melyik al-rect-j�t
						&cursor_rect );		// a renderel� fel�let�nek mely r�sz�re
		*/

		// 1. feladat: pattogtassuk a k�perny�n a kirajzolt k�pet! Induljon el a k�p
		//    a k�perny� k�zep�r�l egy ir�nyba �s amikor valamelyik sz�le az ablak sz�l�hez
		//    k�zeledik, pattanjon vissza!
		
		// KOMMENT: "SDL_GetTicks()"-szel:
		/*
		cursor_rect.x = (1 + sin(SDL_GetTicks() / 1000.0f)) / 2.0f * (win_width-tex_width);
		cursor_rect.y = 200;
		SDL_RenderCopy(ren, tex, nullptr, &cursor_rect);
		*/

		// KOMMENT: "if"-el:
		/*
		static int rect_x = 0;
		static int x_movement = 5;

		if (x_movement > 0 && rect_x >= win_width - tex_width)
			x_movement *= -1;
		if (x_movement < 0 && rect_x <= 0)
			x_movement *= -1;
		
		rect_x += x_movement;

		cursor_rect.x = rect_x;
		cursor_rect.y = 100;
		SDL_RenderCopy(ren, tex, nullptr, &cursor_rect);
		*/


		// 2. feladat: az animation_sheet.gif-ben tal�lhat� anim�ci�t rajzoljuk ki �gy,
		//    hogy a fut�s egy f�zisa 3 mp-ig tartson! Tipp: a source rect-et kell m�dos�tani!
		
		/*
		cursor_rect.w = tex_width/6;
		cursor_rect.h = tex_height/5;
		cursor_rect.x = mouseX - tex_width / 12;
		cursor_rect.y = mouseY - tex_height / 10;

		SDL_Rect source_rect;
		source_rect.w = tex_width / 6;
		source_rect.h = tex_height / 5;
		source_rect.x = int(SDL_GetTicks() / 3000.0f * 30) % 6 * tex_width  / 6;
		source_rect.y = int(SDL_GetTicks() / 3000.0f *  5) % 5 * tex_height / 5;

		SDL_RenderCopy(ren, tex, &source_rect, &cursor_rect);
		*/
		

		// 3. feladat: ne legyen pattog�s, a felhaszn�l� tudjon jobbra-balra futtatni a 
		//    figur�t a balra/jobbra billenty�k lenyom�s�val. Ha elengedi a billenty�t,
		//    akkor �lljon meg a figura, ahol van (tipp: kell �j k�pf�jl)
		static int dir = 0;
		int direction = 0;
		if (left != right) {
			if (left)
				dir--;
			if (right)
				dir++;
		}
		left = false;
		right = false;

		cursor_rect.w = tex_width / 6;
		cursor_rect.h = tex_height / 5;
		cursor_rect.x = dir*5;
		cursor_rect.y = 100;

		SDL_Rect source_rect;
		source_rect.w = tex_width / 6;
		source_rect.h = tex_height / 5;
		source_rect.x = (dir % 6) * tex_width / 6;
		source_rect.y = (dir / 6 % 5) * tex_height / 5;

		SDL_RenderCopy(ren, tex, &source_rect, &cursor_rect);
		

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