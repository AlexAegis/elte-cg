#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <time.h>
#include <vector>

void exitProgram(){
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
}

// Make a random permutation with "size" number of elements
int* knuth_shuffles(int size){
	int* array;
	array = new int[size];
	for (int i = 0; i < size; ++i) array[i] = i;
	for (int i = 0; i < size - 1; ++i){
		int j = rand() % (size - i) + i;
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
	return array;
}

// Do a unicyclic or a uniform random permutation
void scramble(int* digit_perm, bool is_unicyclic_perm){
	for (int i = 0; i < 6; ++i) digit_perm[i] = i + 1;
	if (is_unicyclic_perm){
		int* array = knuth_shuffles(5);
		for (int i = 0; i < 5; ++i){
			int temp = digit_perm[0];
			digit_perm[0] = digit_perm[array[i] + 1];
			digit_perm[array[i] + 1] = temp;
		}
		delete[] array;
	}
	else {
		int* array = knuth_shuffles(6);
		for (int i = 0; i < 6; ++i){
			digit_perm[i] = array[i] + 1;
		}
		delete array;
	}
}

int main(int argc, char* args[]){

	atexit(exitProgram);
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window *win = 0;
	win = SDL_CreateWindow("BlindPerm Game!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == 0){
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Renderer *ren = 0;
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == 0){
		std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* TEXTURE */
	SDL_Texture* tex_digit = IMG_LoadTexture(ren, "digits.png");
	SDL_Texture* tex_check = IMG_LoadTexture(ren, "check.png");
	SDL_Texture* tex_text = IMG_LoadTexture(ren, "text.png");

	if (tex_digit == 0 || tex_check == 0 || tex_text == 0){
		std::cout << "[Kép betöltése] Hiba: " << IMG_GetError() << std::endl;
		SDL_DestroyWindow(win);
		return 1;
	}

	std::cout << "BlindPerm Game!\n\n";
	std::cout << "Can you solve a permutation of 6 digits by not looking at them?\n\n";
	std::cout << "1) Click on 'Unicyclic permutation' or 'Uniform random permutation'\n";
	std::cout << "   to get a scramble and start the timer.\n";
	std::cout << "2) Memorize the permutation of the digits.\n";
	std::cout << "3) Press spacebar to hide the digits.\n";
	std::cout << "4) Solve the permutation by swapping two hidden digits at a time.\n";
	std::cout << "5) Press spacebar again to stop the timer and show the result.\n\n";

	srand(time(NULL));

	int digit_w;
	int digit_h;
	SDL_QueryTexture(tex_digit, 0, 0, &digit_w, &digit_h);
	digit_w /= 10;

	int top_left_pos_x = (640 - 6 * digit_w) / 2;
	int top_left_pos_y = (480 - digit_h) / 2;

	int mouse_over_idx = -1;
	int mouse_selected_idx = -1;
	bool selected = false;

	bool is_unicyclic_perm = true;
	bool show_scramble = true;

	int time_result = 0;
	bool show_timer = false;
	int time_starts_at = 0;
	bool stop_timer = false;

	int* digit_perm;
	digit_perm = new int[6];
	for (int i = 0; i < 6; ++i) digit_perm[i] = i + 1;

	bool quit = false;
	SDL_Event ev;
	Sint32 mouseX = 0, mouseY = 0;

	while (!quit){
		// While we have events
		while (SDL_PollEvent(&ev)){
			switch (ev.type){
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
				if (ev.key.keysym.sym == SDLK_SPACE){
					show_scramble = !show_scramble;
					if (show_scramble) stop_timer = true;
					if (!show_scramble && stop_timer) show_timer = false;
				}
				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;

				// Select which digit is the mouse over
				if (top_left_pos_x < mouseX && mouseX < top_left_pos_x + 6 * digit_w &&
					top_left_pos_y < mouseY && mouseY < top_left_pos_y + digit_h){
					mouse_over_idx = (mouseX - top_left_pos_x) / digit_w;
				}
				else {
					mouse_over_idx = -1;
				}

				break;
			case SDL_MOUSEBUTTONUP:

				if (ev.button.button == SDL_BUTTON_LEFT){

					// Select digits and swap if necessary
					if (top_left_pos_x < mouseX && mouseX < top_left_pos_x + 6 * digit_w &&
						top_left_pos_y < mouseY && mouseY < top_left_pos_y + digit_h){
						if (!selected){
							mouse_selected_idx = (mouseX - top_left_pos_x) / digit_w;
							selected = true;
							if (show_scramble) show_timer = false;
						}
						else {
							int temp = digit_perm[mouse_selected_idx];
							digit_perm[mouse_selected_idx] = digit_perm[mouse_over_idx];
							digit_perm[mouse_over_idx] = temp;
							selected = false;
							mouse_selected_idx = -1;
						}
					}

					// Select the type of permutation scrambling
					if (top_left_pos_x < mouseX && mouseX < top_left_pos_x + 20 &&
						top_left_pos_y + digit_h + 70 < mouseY && mouseY < top_left_pos_y + digit_h + 90){
						is_unicyclic_perm = true;
						scramble(digit_perm, is_unicyclic_perm);
						show_scramble = true;
						show_timer = true;
						stop_timer = false;
						time_starts_at = SDL_GetTicks();
						selected = false;
						mouse_selected_idx = -1;
					}
					if (top_left_pos_x < mouseX && mouseX < top_left_pos_x + 20 &&
						top_left_pos_y + digit_h + 110 < mouseY && mouseY < top_left_pos_y + digit_h + 130){
						is_unicyclic_perm = false;
						scramble(digit_perm, is_unicyclic_perm);
						show_scramble = true;
						show_timer = true;
						stop_timer = false;
						time_starts_at = SDL_GetTicks();
						selected = false;
						mouse_selected_idx = -1;
					}
				}
				break;
			}
		}

		// Clear the background
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		/* *********************************************************/
		/* Draw digits */

		for (int i = 0; i < 6; ++i){

			// Movable digits
			SDL_Rect movable_from_rect;
			movable_from_rect.x = digit_perm[i] * digit_w;
			movable_from_rect.y = 0;
			movable_from_rect.w = digit_w;
			movable_from_rect.h = digit_h;

			SDL_Rect movable_to_rect;
			movable_to_rect.x = top_left_pos_x + i * digit_w;
			movable_to_rect.y = top_left_pos_y;
			movable_to_rect.w = digit_w;
			movable_to_rect.h = digit_h;
			if (show_scramble){
				SDL_RenderCopy(ren, tex_digit, &movable_from_rect, &movable_to_rect);
			}

			// Static digits
			SDL_Rect static_from_rect;
			static_from_rect.x = (i + 1) * digit_w;
			static_from_rect.y = 0;
			static_from_rect.w = digit_w;
			static_from_rect.h = digit_h;

			SDL_Rect static_to_rect;
			static_to_rect.x = top_left_pos_x + i * digit_w;
			static_to_rect.y = top_left_pos_y - digit_h;
			static_to_rect.w = digit_w;
			static_to_rect.h = digit_h;
			SDL_RenderCopy(ren, tex_digit, &static_from_rect, &static_to_rect);

			// Check symbols
			SDL_Rect check_from_rect;
			check_from_rect.x = 0;
			check_from_rect.y = 0;
			int check_w, check_h;
			SDL_QueryTexture(tex_check, 0, 0, &check_w, &check_h);
			check_from_rect.w = check_w / 2;
			check_from_rect.h = check_h;
			if (digit_perm[i] != i + 1){
				check_from_rect.x = check_w / 2;
			}

			SDL_Rect check_to_rect;
			check_to_rect.x = top_left_pos_x + i * digit_w + (digit_w - 30) / 2;
			check_to_rect.y = top_left_pos_y + digit_h + 10;
			check_to_rect.w = 30;
			check_to_rect.h = 30;
			if (show_scramble){
				SDL_RenderCopy(ren, tex_check, &check_from_rect, &check_to_rect);
			}
		}

		/* *********************************************************/
		/* Draw selection frames */

		SDL_Rect frame_over_rect;
		frame_over_rect.x = top_left_pos_x + mouse_over_idx * digit_w + 5;
		frame_over_rect.y = top_left_pos_y + 5;
		frame_over_rect.w = digit_w - 10;
		frame_over_rect.h = digit_h - 10;
		SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
		if (mouse_over_idx != -1){
			SDL_RenderDrawRect(ren, &frame_over_rect);
		}

		SDL_Rect frame_selected_rect;
		frame_selected_rect.x = top_left_pos_x + mouse_selected_idx * digit_w;
		frame_selected_rect.y = top_left_pos_y;
		frame_selected_rect.w = digit_w;
		frame_selected_rect.h = digit_h;
		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
		if (mouse_selected_idx != -1){
			SDL_RenderDrawRect(ren, &frame_selected_rect);
		}

		/* *********************************************************/
		/* Draw buttons */

		SDL_Rect button_filled_unicyclic;
		button_filled_unicyclic.x = top_left_pos_x;
		button_filled_unicyclic.y = top_left_pos_y + digit_h + 70;
		button_filled_unicyclic.w = 20;
		button_filled_unicyclic.h = 20;
		SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
		SDL_RenderFillRect(ren, &button_filled_unicyclic);

		SDL_Rect button_filled_uniform;
		button_filled_uniform.x = top_left_pos_x;
		button_filled_uniform.y = top_left_pos_y + digit_h + 110;
		button_filled_uniform.w = 20;
		button_filled_uniform.h = 20;
		SDL_RenderFillRect(ren, &button_filled_uniform);

		SDL_Rect button_selected;
		button_selected.w = 26;
		button_selected.h = 26;
		button_selected.x = top_left_pos_x - 3;
		if (is_unicyclic_perm){
			button_selected.y = top_left_pos_y + digit_h + 70 - 3;
		}
		else {
			button_selected.y = top_left_pos_y + digit_h + 110 - 3;
		}
		SDL_RenderDrawRect(ren, &button_selected);

		/* *********************************************************/
		/* Draw text */

		SDL_Rect text_from_rect;
		text_from_rect.x = 0;
		text_from_rect.y = 0;
		SDL_QueryTexture(tex_text, 0, 0, &text_from_rect.w, &text_from_rect.h);

		SDL_Rect text_to_rect;
		text_to_rect.x = top_left_pos_x + 30;
		text_to_rect.y = top_left_pos_y + digit_h + 60;
		text_to_rect.w = text_from_rect.w;
		text_to_rect.h = text_from_rect.h;
		SDL_RenderCopy(ren, tex_text, &text_from_rect, &text_to_rect);

		/* *********************************************************/
		/* Draw timer */

		// Calculate the time
		int time;
		if (!stop_timer){
			time = (SDL_GetTicks() - time_starts_at) / 100;
			time_result = time;
		}
		else {
			time = time_result;
		}

		// Draw time
		int digit_num = 0;
		if (show_timer){
			do {
				int digit = time % 10;

				SDL_Rect timer_from_rect;
				timer_from_rect.x = digit * digit_w;
				timer_from_rect.y = 0;
				timer_from_rect.w = digit_w;
				timer_from_rect.h = digit_h;

				SDL_Rect timer_to_rect;
				timer_to_rect.x = 500 - digit_num * 40;
				timer_to_rect.y = 10;
				timer_to_rect.w = 30;
				timer_to_rect.h = 40;
				SDL_RenderCopy(ren, tex_digit, &timer_from_rect, &timer_to_rect);

				time /= 10;
				++digit_num;
			} while (time > 0);

			// Draw dot
			SDL_Rect dot_rect;
			dot_rect.x = 490;
			dot_rect.y = 43;
			dot_rect.w = 5;
			dot_rect.h = 5;
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			SDL_RenderFillRect(ren, &dot_rect);
		}

		SDL_RenderPresent(ren);
	}

	delete[] digit_perm;

	SDL_DestroyTexture(tex_digit);
	SDL_DestroyTexture(tex_check);
	SDL_DestroyTexture(tex_text);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;
}