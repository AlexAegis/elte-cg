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

	int wh = 640, ww = 480;

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello HZ!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							wh,						// ablak szélessége
							ww,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == nullptr)
	{
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: tölstünk be egy képfájlt
	//
	SDL_Texture* tex = IMG_LoadTexture(ren, "hz.png" );
	SDL_Texture* tex2 = IMG_LoadTexture(ren, "kata.png");
	if ( tex == nullptr)
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

	 Sint32 x = 100, y = 100, hs = 2, vs = 2, hd = 1, vd = 1;
	 Sint32 x2 = 400, y2 = 400, hs2 = 2, vs2 = 2, hd2 = -1, vd2 = -1;

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

		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// rajzoljuk ki a betöltött képet az egékurzor köré!
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

		SDL_RenderCopy( ren,				// melyik renderelõre rajzoljunk
						tex,				// melyik textúrát rajzoljuk rá
						nullptr,			// a textúra melyik al-rect-jét
						&cursor_rect );		// a renderelõ felületének mely részére


		// 1. feladat: pattogtassuk a képernyõn a kirajzolt képet! Induljon el a kép
		//    a képernyõ közepérõl egy irányba és amikor valamelyik széle az ablak széléhez
		//    közeledik, pattanjon vissza!

	
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