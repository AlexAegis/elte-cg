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
	// 3. l�p�s: t�lst�nk be egy k�pf�jlt
	//

	// Kek kep texturaja
	SDL_Texture* tex_kek = IMG_LoadTexture( ren, "kep.png" );
	if ( tex_kek == 0 )
	{
        std::cout << "[K�p bet�lt�se] Hiba: " << IMG_GetError() << std::endl;
		SDL_DestroyWindow( win );
        return 1;
	}

	// Jobbra futo ember kepkockai
	SDL_Texture* tex_fut_jobbra = IMG_LoadTexture(ren, "animation_sheet.gif");
	if (tex_kek == 0)
	{
		std::cout << "[K�p bet�lt�se] Hiba: " << IMG_GetError() << std::endl;
		SDL_DestroyWindow(win);
		return 1;
	}

	// Balra futo ember kepkockai (az elozo kep tukrozve)
	SDL_Texture* tex_fut_balra = IMG_LoadTexture(ren, "animation_sheet_left.gif");
	if (tex_kek == 0)
	{
		std::cout << "[K�p bet�lt�se] Hiba: " << IMG_GetError() << std::endl;
		SDL_DestroyWindow(win);
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
	//Sint32 mouseX = 0, mouseY = 0;

	int x_anim = 300; // az interaktiv futo emberke x koordinataja
	bool kurzorgomb_lenyomva = false; // lenyomtuk-e valamelyik kurzor gombot?
	bool jobbra_mozog = true; // jobbra kell-e futnia az embernek?

	while (!quit)
	{
		// am�g van feldolgozand� �zenet dolgozzuk fel mindet:
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				
				// Lenyomtuk a jobbra mutato kurzor billentyut
				if (ev.key.keysym.sym == SDLK_RIGHT){
					kurzorgomb_lenyomva = true;
					jobbra_mozog = true;
				}
				// Lenyomtuk a balra mutato kurzor billentyut
				if (ev.key.keysym.sym == SDLK_LEFT){
					kurzorgomb_lenyomva = true;
					jobbra_mozog = false;
				}

				if (ev.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
				break;
				
			// Felengedtuk valamelyik kurzorgombot
			case SDL_KEYUP:
				if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_LEFT){
					kurzorgomb_lenyomva = false;
				}
				break;
			/*
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			
			case SDL_MOUSEBUTTONUP:
				// eg�rgomb felenged�s�nek esem�nye; a felengedett gomb a ev.button.button -ban tal�lhat�
				// a lehets�ges gombok a k�vetkez�ek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				break;
			*/
			}
			
		}

		// t�r�lj�k a h�tteret feh�rre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		/*******************************************
		KEK TEXTURA PATTOGTATASA
		*********************************************/

		// x,y coordinatak kezdetben
		static int x = 200;
		static int y = 200;

		// Sebessegvektorok kezdetben
		static int vx = 5;
		static int vy = 5;

		// A texturat ide fogjuk kirajzolni...
		SDL_Rect cursor_rect;
		cursor_rect.x = x;
		cursor_rect.y = y;

		// ...ekkora meretben
		SDL_QueryTexture(tex_kek, 0, 0, &cursor_rect.w, &cursor_rect.h);

		// Egesz textura masolasa a teglalapba
		SDL_RenderCopy(ren, tex_kek, 0, &cursor_rect);

		// Mozgas a sebessegvektorok szerint
		x += vx;
		y += vy;

		// Ha falat ertunk, valtsunk iranyt
		if (x <= 0 || x >= 640 - cursor_rect.w){
			vx *= -1;
		}
		if (y <= 0 || y >= 480 - cursor_rect.h){
			vy *= -1;
		}

		/**********************************************************
		NEM-INTERAKTIV FUTO EMBER ANIMACIOJA (a bal felso sarokban)
		***********************************************************/

		// Lekerjuk egy kis kepkocka meretet
		int big_w, big_h;
		SDL_QueryTexture(tex_fut_jobbra, 0, 0, &big_w, &big_h);
		int small_w = big_w / 6;
		int small_h = big_h / 5;

		// Ide fogjuk a kepernyore rajzolni
		SDL_Rect dst_rect;
		dst_rect.x = 0;
		dst_rect.y = 0;
		dst_rect.w = small_w / 2; // felere kicsinyitjuk, mert miert ne
		dst_rect.h = small_h / 2;

		// Idozites

		// Azt akarjuk, hogy timer = 0, 1, ..., 29 erteket vegyen fel 3 mp alatt, es ciklizaljon
		// azaz timer ertekei: 0, 1, ..., 29, 0, 1, ..., 29, 0, 1, ... legyenek, egy teljes kort 3 mp alatt megteve

		// Elso megoldas:
		// timer = SDL_GetTicks();					// t erteke 1 mp alatt 1000-rel no
		// timer = SDL_GetTicks() % 1000;			// t erteke 1 mp alatt 1000-rel no, de 999 utan ujra 0 lesz
		// timer = SDL_GetTicks() % 3000;			// t erteke 1 mp alatt 1000-rel no, de 2999 utan ujra 0 lesz
													//		AZAZ: t erteke 3 mp alatt no 0-rol 2999-re (es utana ujra 0 lesz)
		// timer = (SDL_GetTicks() % 3000) / 100;	// t erteke 3 mp alatt megy 0-rol 29-re (es utana ujra 0 lesz)

		// Elso megoldas:
		int timer = (SDL_GetTicks() % 3000) / 100;

		// Ugyanaz a megoldas masik megkozelitesben:
		// timer = SDL_GetTicks();						// t erteke 1 mp alatt 1000-rel no
		// timer = SDL_GetTicks() / 1000;				// t erteke 1 mp alatt 1-gyel no
		// timer = SDL_GetTicks() / 3000;				// t erteke 3 mp alatt 1-gyel no
		// timer = 30 * SDL_GetTicks() / 3000;			// t erteke 3 mp alatt 30-cal no
		// timer = (30 * SDL_GetTicks() / 3000) % 30;	// t erteke 3 mp alatt 30-cal no, de 29 utan ujra 0 lesz

		// Masodik megoldas:
		// int timer = (30 * SDL_GetTicks() / 3000) % 30;

		// timer fuggvenyeben hanyadik oszlopban (i) es sorban (j) vagyunk
		int i = timer % 6;
		int j = timer / 6;

		// Innen vagunk ki egy darabot a texturabol
		SDL_Rect src_rect;
		src_rect.x = i * small_w;
		src_rect.y = j * small_h;
		src_rect.w = small_w;
		src_rect.h = small_h;

		// Rahuzzuk a textura megfelelo reszet a megfelelo feluletre
		SDL_RenderCopy(ren, tex_fut_jobbra, &src_rect, &dst_rect);

		/**********************************************************
		INTERAKTIV FUTO EMBER ANIMACIOJA (kozeptajt)
		***********************************************************/

		// Ismerjuk mar fentrol: small_w, small_h
		// Egyebkent ha nem ismernenk:
		/*
		int big_w, big_h;
		SDL_QueryTexture(tex_fut_jobbra, 0, 0, &big_w, &big_h);
		int small_w = big_w / 6;
		int small_h = big_h / 5;
		*/

		// Csak akkor animalunk, ha le van nyomva az egyik kurzorgomb
		static int timer_anim = 0;
		if (kurzorgomb_lenyomva){
			// timer: 0.5 mp alatt 0-rol 29-re, ciklizal
			timer_anim = (SDL_GetTicks() % 500) * (30.0f / 500.0f);
		}

		// timer fuggvenyeben hanyadik oszlopban es sorban vagyunk
		int i_anim = timer_anim % 6;
		int j_anim = timer_anim / 6;

		// Ha nyomjuk a jobbra gombot es nem vagyunk a kep szelen, mozogjunk jobbra
		if (kurzorgomb_lenyomva && jobbra_mozog && x_anim <= 640 - small_w){
			x_anim += 5;
		}
		// Hasonlokepp balra
		else if (kurzorgomb_lenyomva && !jobbra_mozog && x_anim >= 0){
			x_anim -= 5;
		}

		// Ide rajzoljuk az animacio kepkockajat a rajzolofeluletre (ablakba)
		SDL_Rect dst_rect_anim;
		dst_rect_anim.x = x_anim;
		dst_rect_anim.y = 200;
		dst_rect_anim.w = small_w;
		dst_rect_anim.h = small_h;

		// Innen vagjuk ki a textura adott reszet
		SDL_Rect src_rect_anim;
		// Jobbra mozgas eseten a kepkockak sorfolytonos bejarasa balrol jobbra tortenik...
		// (oszlopok sorszama: 0, 1, 2, 3, 4, 5)
		if (jobbra_mozog){
			src_rect_anim.x = i_anim * small_w;
		} else {
			// ...egyebkent a tukrozes miatt jobbrol balra
			// (oszlopok sorszama: 5, 4, 3, 2, 1, 0)
			src_rect_anim.x = (5 - i_anim) * small_w;
		}
		src_rect_anim.y = j_anim * small_h;
		src_rect_anim.w = small_w;
		src_rect_anim.h = small_h;

		// Mozgasiranytol fugg, melyik texturabol rajzoljuk a kepkockat
		if (jobbra_mozog){
			SDL_RenderCopy(ren, tex_fut_jobbra, &src_rect_anim, &dst_rect_anim);
		}
		else {
			SDL_RenderCopy(ren, tex_fut_balra, &src_rect_anim, &dst_rect_anim);
		}
		// jelen�ts�k meg a backbuffer tartalm�t
		SDL_RenderPresent(ren);
	}



	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyTexture( tex_kek );
	SDL_DestroyTexture( tex_fut_jobbra );
	SDL_DestroyTexture( tex_fut_balra );
	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}