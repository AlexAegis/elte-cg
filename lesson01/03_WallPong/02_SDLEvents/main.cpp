#include <SDL.h>
#include <iostream>
#include <algorithm>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = 0;
    win = SDL_CreateWindow( "Paddle Game!",				// az ablak fejléce
							200,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							200,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == 0)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	SDL_SetWindowResizable(win, SDL_TRUE);

    SDL_Renderer *ren = 0;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == 0)
	{
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;
	Sint32 unit = 20;

	while (!quit)
	{
		// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
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
				// egérgomb felengedésének eseménye; a felengedett gomb a ev.button.button -ban található
				// a lehetséges gombok a következõek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				break;
			}
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

		int w;
		int h;
		SDL_GetWindowSize(win, &w, &h);

		SDL_Rect paddle;
		paddle.w = unit * 4;
		paddle.h = unit * 1;
		paddle.x = std::min(w - paddle.w, std::max(0, mouseX - paddle.w / 2));
		paddle.y = h - (unit * 3);

		SDL_RenderFillRect(ren, &paddle);

		SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
							0, 0, // vonal kezdõpontjának (x,y) koordinátái
							mouseX, mouseY);// vonal végpontjának (x,y) koordinátái
		
		// definiáljunk egy (mouseX, mouseY) középpontó, tengelyekkel párhuzamos oldalú
		// 20x20-as négyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - 10;
		cursor_rect.y = mouseY - 10;
		cursor_rect.w = 20;
		cursor_rect.h = 20;
		// legyen a kitöltési szín piros
		SDL_SetRenderDrawColor( ren, 255, 0, 0, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt idõ függvényében periodikusan nõjjön és csökkenjen
		//    az egérmutató középpontjával kirajzolt négyszög

		// 2. feladat: ha a user a bal egérgombot nyomja meg akkor a téglalap színe váltson pirosra,
		//    ha a jobb egérgombot, akkor kékre

		// 3. feladat: rajzolj ki egy 50 sugarú körvonalat az egérmutató köré!
		// segítség: használd a SDL_RenderDrawLines()-t

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}