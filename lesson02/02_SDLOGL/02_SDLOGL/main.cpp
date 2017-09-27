#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <sstream>

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
	// 2. lépés: állítsuk be az OpenGL-es igényeinket, hozzuk létre az ablakunkat, indítsuk el az OpenGL-t
	//

	// 2a: OpenGL indításának konfigurálása, ezt az ablak létrehozása elõtt kell megtenni!

	// beállíthatjuk azt, hogy pontosan milyen OpenGL context-et szeretnénk létrehozni - ha nem tesszük, akkor
	// automatikusan a legmagasabb elérhetõ verziójút kapjuk
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// állítsuk be, hogy hány biten szeretnénk tárolni a piros, zöld, kék és átlátszatlansági információkat pixelenként
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
	// duplapufferelés
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		1);
	// mélységi puffer hány bites legyen
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          24);

	// antialiasing - ha kell
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	// hozzuk létre az ablakunkat
	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL&OpenGL!",		// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok


	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre az OpenGL context-et - ezen keresztül fogunk rajzolni
	//

	SDL_GLContext	context	= SDL_GL_CreateContext(win);
    if (context == nullptr)
	{
		std::cout << "[OGL context létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }	

	// megjelenítés: várjuk be a vsync-et
	SDL_GL_SetSwapInterval(1);

	// kérdezzük le az OpenGL verziót
	int glVersion[2] = {-1, -1}; 
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
	std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;

	if ( glVersion[0] == -1 && glVersion[1] == -1 )
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow( win );

		std::cout << "[OGL context létrehozása] Nem sikerült létrehozni az OpenGL context-et! Lehet, hogy az SDL_GL_SetAttribute(...) hívásoknál az egyik beállítás helytelen." << std::endl;

		return 1;
	}

	std::stringstream window_title;
	window_title << "OpenGL " << glVersion[0] << "." << glVersion[1];
	SDL_SetWindowTitle(win, window_title.str().c_str());

	//
	// 3. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;

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
			}
		}

		// töröljük a hátteret fehérre
		glClearColor ( 0.0f, 1.0f, 0.0f, 1.0f );
		glClear ( GL_COLOR_BUFFER_BIT );

		// 1. feladat: az ablak háttérszíne 2 másodpercenként módosuljon, diszkrét ugrásokkal piros, zöld és kék között
		// ehhez segitségül SDL_GetTicks(): http://wiki.libsdl.org/moin.fcg/SDL_GetTicks

		// 2. feladat: az ablak háttérszíne 5 másodperces periódussal váltson feketérõl zöldre és vissza, folyamatosan

		SDL_GL_SwapWindow(win);
	}



	//
	// 4. lépés: lépjünk ki
	// 

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}