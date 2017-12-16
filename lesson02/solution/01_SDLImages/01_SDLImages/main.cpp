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

	const int win_width = 640;
	const int win_height = 480;
	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							win_width,						// ablak szélessége
							win_height,						// és magassága
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
	//SDL_Texture* tex = IMG_LoadTexture( ren, "kep.png" );
	SDL_Texture* tex = IMG_LoadTexture(ren, "animation_sheet.gif");
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
	// KOMMENT: jobbra vagy balra menés
	bool left = false;
	bool right = false;


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
		// KOMMENT:       texture,  format,  access,      width,      height
		SDL_QueryTexture(     tex, nullptr, nullptr, &tex_width, &tex_height);

		SDL_Rect cursor_rect; 
		cursor_rect.w = tex_width;
		cursor_rect.h = tex_height;
		/*
		cursor_rect.x = mouseX - tex_width /2;
		cursor_rect.y = mouseY - tex_height /2;

		SDL_RenderCopy( ren,				// melyik renderelõre rajzoljunk
						tex,				// melyik textúrát rajzoljuk rá
						nullptr,			// a textúra melyik al-rect-jét
						&cursor_rect );		// a renderelõ felületének mely részére
		*/

		// 1. feladat: pattogtassuk a képernyõn a kirajzolt képet! Induljon el a kép
		//    a képernyõ közepérõl egy irányba és amikor valamelyik széle az ablak széléhez
		//    közeledik, pattanjon vissza!
		
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


		// 2. feladat: az animation_sheet.gif-ben található animációt rajzoljuk ki úgy,
		//    hogy a futás egy fázisa 3 mp-ig tartson! Tipp: a source rect-et kell módosítani!
		
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
		

		// 3. feladat: ne legyen pattogás, a felhasználó tudjon jobbra-balra futtatni a 
		//    figurát a balra/jobbra billentyûk lenyomásával. Ha elengedi a billentyût,
		//    akkor álljon meg a figura, ahol van (tipp: kell új képfájl)
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