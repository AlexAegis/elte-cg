#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	// állítsuk be, hogy kilépés elõtt hívja meg a rendszer az exitProgram() függvényt - Kérdés: mi lenne enélkül?
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
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == 0)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

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
	// 3. lépés: tölstünk be egy képfájlt
	//
	SDL_Texture* tex = IMG_LoadTexture( ren, "animation_sheet.gif" );
	if ( tex == 0 )
	{
        std::cout << "[Kép betöltése] Hiba: " << IMG_GetError() << std::endl;
		SDL_DestroyWindow( win );
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
	int posX = 320;
	int posY = 240;
	int velX = 0;
	int velY = 0;
	int currX = 0;
	int currY = 0;

	int tStart = SDL_GetTicks();


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
				if (ev.key.keysym.sym == SDLK_RIGHT)
					velX = 10;
				if (ev.key.keysym.sym == SDLK_LEFT)
					velX = -10;
				if (ev.key.keysym.sym == SDLK_UP)
					velY = -10;
				if (ev.key.keysym.sym == SDLK_DOWN)
					velY = 10;
				break;
			case SDL_KEYUP:
				if (ev.key.keysym.sym == SDLK_RIGHT)
					velX = 0;
				if (ev.key.keysym.sym == SDLK_LEFT)
					velX = 0;
				if (ev.key.keysym.sym == SDLK_UP)
					velY = 0;
				if (ev.key.keysym.sym == SDLK_DOWN)
					velY = 0;
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

		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// rajzoljuk ki a betöltött képet az egékurzor köré!
		int tex_width, tex_height;
		SDL_QueryTexture( tex, 0, 0, &tex_width, &tex_height);

		int t = (SDL_GetTicks() / 40) % 100;

		SDL_Rect cursor_rect; 
		cursor_rect.w = tex_width / 6;
		cursor_rect.h = tex_height / 5;
		cursor_rect.x = posX - cursor_rect.w /2;
		cursor_rect.y = posY - cursor_rect.h /2;


		SDL_Rect sub_rect;
		sub_rect.w = 85.33333333333333f;
		sub_rect.h = 102.4f;
		sub_rect.x = currX * sub_rect.w;
		sub_rect.y = currY * sub_rect.h;

		SDL_RenderCopy( ren,				// melyik renderelõre rajzoljunk
						tex,				// melyik textúrát rajzoljuk rá
						&sub_rect,					// a textúra melyik al-rect-jét
						&cursor_rect);		// a renderelõ felületének mely részére
		currX++;
		if (currX >= 5) {
			currX = 0;
			currY++;
		}

		if (currY >= 5) {
			currY = 0;
		}		
		
		posX += velX;
		posY += velY;
		/*
		if (posX + cursor_rect.w / 2 > 640 || posX + cursor_rect.w / 2 < cursor_rect.w) {
			velX = 0;
		}
		if (posY + cursor_rect.h / 2 > 480 || posY + cursor_rect.h / 2 < cursor_rect.h) {
			velY = 0;
		}*/



		// 1. feladat: pattogtassuk a képernyõn a kirajzolt képet! Induljon el a kép
		//    a képernyõ közepérõl egy irányba és amikor valamelyik széle az ablak széléhez
		//    közeledik, pattanjon vissza!

		// 2. feladat: az animation_sheet.gif-ben található animációt rajzoljuk ki úgy,
		//    hogy a futás egy fázisa 3 mp-ig tartson! Tipp: a source rect-et kell módosítani!

		// 3. feladat: ne legyen pattogás, a felhasználó tudjon jobbra-balra futtatni a 
		//    figurát a balra/jobbra billentyûk lenyomásával. Ha elengedi a billentyût,
		//    akkor álljon meg a figura, ahol van (tipp: kell új képfájl)

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}



	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyTexture( tex );
	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}