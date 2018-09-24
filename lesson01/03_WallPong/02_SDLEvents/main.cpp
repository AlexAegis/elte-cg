#include <SDL.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>

void exitProgram() {
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int clamp(int p, int l, int h) {
	return std::min(h, std::max(l, p));
}

bool between(int p, int l, int h) {
	return clamp(p, l, h) == p;
}

void drawDigit(SDL_Renderer* ren, int pos, int digit) {
	int offset = pos * 35;
	int h_offset = 30;
	if (digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0) {
		SDL_RenderDrawLine(ren,
			10 + offset, 10 + h_offset,
			10 + offset, 40 + h_offset);
	}

	if (digit == 2 || digit == 6 || digit == 8 || digit == 0) {
		SDL_RenderDrawLine(ren,
			10 + offset, 40 + h_offset,
			10 + offset, 70 + h_offset);
	}

	if (digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0) {
		SDL_RenderDrawLine(ren,
			10 + offset, 10 + h_offset,
			40 + offset, 10 + h_offset);
	}


	if (digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9) {
		SDL_RenderDrawLine(ren,
			10 + offset, 40 + h_offset,
			40 + offset, 40 + h_offset);
	}


	if (digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0) {
		SDL_RenderDrawLine(ren,
			10 + offset, 70 + h_offset,
			40 + offset, 70 + h_offset);
	}


	if (digit == 1 || digit == 2 || digit == 3 || digit == 4 || digit == 7 || digit == 8 || digit == 9 || digit == 0) {
		SDL_RenderDrawLine(ren,
			40 + offset, 10 + h_offset,
			40 + offset, 40 + h_offset);
	}


	if (digit == 1 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0) {
		SDL_RenderDrawLine(ren,
			40 + offset, 40 + h_offset,
			40 + offset, 70 + h_offset);
	}

}

std::vector<int> getdigits(int num) {
	std::vector<int> res;
	do {
		res.push_back(num % 10);
		num /= 10;
	} while (num > 0);
	std::reverse(res.begin(), res.end());
	return res;
}

void drawNumber(SDL_Renderer* ren, int number) {
	std::vector<int> digits = getdigits(number);
	for (int i = 1; i <= digits.size(); i++) {
		drawDigit(ren, i, digits.at(i - 1));
	}
}

int main( int argc, char* args[] ) {
	atexit( exitProgram );

	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	SDL_Window *win = 0;
    win = SDL_CreateWindow("Paddle Game!",
							200,
							200,
							640,
							480,
							SDL_WINDOW_SHOWN);

    if (win == 0) {
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	SDL_SetWindowResizable(win, SDL_TRUE);

    SDL_Renderer *ren = 0;
    ren = SDL_CreateRenderer(	win,
								-1,
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == 0) {
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	// véget kell-e érjen a program futása?
	bool quit = false;

	int w;
	int h;
	SDL_GetWindowSize(win, &w, &h);

	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;
	Sint32 unit = 20;
	Sint32 speed = 2;
	Sint32 credit = 0;
	Sint32 dirX = 1;
	Sint32 dirY = 1;

	SDL_Rect paddle;
	paddle.w = unit * 4;
	paddle.h = unit * 1;
	paddle.x = std::min(w - paddle.w, std::max(0, mouseX - paddle.w / 2));
	paddle.y = h - (unit * 3);

	SDL_Rect ball;
	ball.x = (w / 2 - unit / 2);
	ball.y = (h / 2 - unit / 2);
	ball.w = unit;
	ball.h = unit;

	SDL_Rect wall_w;
	wall_w.x = 0;
	wall_w.y = 0;
	wall_w.w = unit;
	wall_w.h = h;

	SDL_Rect wall_n;
	wall_n.x = unit;
	wall_n.y = 0;
	wall_n.w = w - unit * 2;
	wall_n.h = unit;

	SDL_Rect wall_e;
	wall_e.x = w - unit;
	wall_e.y = 0;
	wall_e.w = unit;
	wall_e.h = h;

	while (!quit) {
		while ( SDL_PollEvent(&ev) ) {
			switch (ev.type) {
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

				break;
			}
		}

		SDL_GetWindowSize(win, &w, &h);

		wall_w.x = 0;
		wall_w.y = 0;
		wall_w.w = unit;
		wall_w.h = h;

		wall_n.x = unit;
		wall_n.y = 0;
		wall_n.w = w - unit * 2;
		wall_n.h = unit;

		wall_e.x = w - unit;
		wall_e.y = 0;
		wall_e.w = unit;
		wall_e.h = h;


		paddle.x = clamp(mouseX - paddle.w / 2, wall_w.x + wall_w.w, wall_e.x - paddle.w);
		paddle.y = h - (unit * 3);

		ball.x = ball.x + (dirX * speed);
		ball.y = ball.y + (dirY * speed);

		// "collision"

		if (!between(ball.x, wall_w.x + wall_w.w, wall_e.x - ball.w)) {
			dirX *= -1;
			credit++;
		}

		if (std::max(wall_n.y + wall_n.h, ball.y) != ball.y) {
			dirY *= -1;
			credit++;
		}

		if (std::min(h - ball.h, ball.y) != ball.y) {
			quit = true;
		}


		// "speed guard"

		int originalDirY = dirY;

		if (between(ball.y + ball.h, paddle.y, paddle.y + paddle.h) 
			&& between(ball.x + ball.w / 2, paddle.x, paddle.x + paddle.w)) {
			dirY = -1;
		}

		if (originalDirY != dirY) {
			speed++;
			credit++;
		}
		

		// "render farm"
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderFillRect(ren, &paddle);
		SDL_SetRenderDrawColor( ren, 255, 255, 255, 255 );
		SDL_RenderFillRect( ren, &ball);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderFillRect(ren, &wall_w);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderFillRect(ren, &wall_n);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderFillRect(ren, &wall_e);

		drawNumber(ren, credit);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}