#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

void drawLetter(SDL_Renderer* ren, int pos, std::string letter) {
	int offset = pos * 35;

	if (letter == "H" || letter == "E"  || letter == "L" || letter == "O") {
		SDL_RenderDrawLine(ren,
			10 + offset, 10, // vonal kezdõpontjának (x,y) koordinátái
			10 + offset, 40);
	}

	if (letter == "H" || letter == "E" || letter == "L" || letter == "O") {
		SDL_RenderDrawLine(ren,
			10 + offset, 40, // vonal kezdõpontjának (x,y) koordinátái
			10 + offset, 70);
	}

	if (letter == "E" || letter == "O") {
		SDL_RenderDrawLine(ren,
			10 + offset, 10, // vonal kezdõpontjának (x,y) koordinátái
			40 + offset, 10);
	}


	if (letter == "H" || letter == "E") {
		SDL_RenderDrawLine(ren,
			10 + offset, 40, // vonal kezdõpontjának (x,y) koordinátái
			40 + offset, 40);
	}


	if (letter == "E" || letter == "L" || letter == "O") {
		SDL_RenderDrawLine(ren,
			10 + offset, 70, // vonal kezdõpontjának (x,y) koordinátái
			40 + offset, 70);
	}


	if (letter == "H" || letter == "O") {
		SDL_RenderDrawLine(ren,
			40 + offset, 10, // vonal kezdõpontjának (x,y) koordinátái
			40 + offset, 40);
	}


	if (letter == "H" || letter == "O") {
		SDL_RenderDrawLine(ren,
			40 + offset, 40, // vonal kezdõpontjának (x,y) koordinátái
			40 + offset, 70);
	}

}

int main( int argc, char* args[] )
{

	// állítsuk be, hogy kilépés elõtt hívja meg a rendszer az exitProgram() függvényt
	atexit( exitProgram );

	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjünk ki
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
	// 3. lépés: töröljük az ablak háttérszínét és várjunk 2 másodpercet
	//

	// aktuális rajzolási szín legyen fekete és töröljük az aktuális rajzolási színnel az ablak kliensterületét
	SDL_SetRenderDrawColor(	ren,	// melyik renderelõnek állítjuk be az aktuális rajzolási színét
							0,	// vörös intenzitás - 8 bites elõjel nélküli egész szám
							0,		// zöld intenzitás - 8 bites elõjel nélküli egész szám
							0,		// kék intenzitás - 8 bites elõjel nélküli egész szám
							255);	// átlátszóság - 8 bites elõjel nélküli egész szám
	SDL_RenderClear(ren);

	// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
	SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
							0,		// piros
							255,	// zöld
							0,		// kék
							255);	// átlátszatlanság





	// 3. feladat: 2 másodpercenként változzon a háttér színe! Elõször legyen piros, aztán zöld és végül kék,
	// majd lépjen ki a program!
	std::vector<int> currColor{ 255, 0, 0 };
	while (true) {
		// aktuális rajzolási szín legyen fekete és töröljük az aktuális rajzolási színnel az ablak kliensterületét
		SDL_SetRenderDrawColor(ren,	// melyik renderelõnek állítjuk be az aktuális rajzolási színét
			currColor[0],	// vörös intenzitás - 8 bites elõjel nélküli egész szám
			currColor[1],		// zöld intenzitás - 8 bites elõjel nélküli egész szám
			currColor[2],		// kék intenzitás - 8 bites elõjel nélküli egész szám
			255);	// átlátszóság - 8 bites elõjel nélküli egész szám
		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
			0,		// piros
			255,	// zöld
			0,		// kék
			255);	// átlátszatlanság


	// 1. feladat: egészítsük ki a fenti vonalat egy H betûvé!


		drawLetter(ren, 1, "H");

		// 2. feladat: írjuk ki a "HELLO" szöveget a képernyõre! Ehhez használható a
		// SDL_RenderDrawLines( <renderer ptr>, <SDL_Point tömb>, <pontok száma>); parancs!

		drawLetter(ren, 2, "E");
		drawLetter(ren, 3, "L");
		drawLetter(ren, 4, "L");
		drawLetter(ren, 5, "O");

			// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);

		// várjunk 2 másodpercet
		SDL_Delay(500);
		std::rotate(currColor.begin(), currColor.begin() + 1, currColor.end());
	}


	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}
