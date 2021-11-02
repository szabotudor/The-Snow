#include<Snow.h>


int main(int argc, char* args[]) {
	ss::Snow game("The Snow", ss::Vector(512, 288), SDL_WINDOW_SHOWN, 60);
	ss::Text text(game.get_renderer(), "Test String", "Pixel.ttf", 20);
	float _dt = 0.0f;
	int div_fps = 60;
	int avg_fps[60] = { 0 }, i = 0;

	while (game.running(_dt)) {
		game.clear_screen();
		if (i < div_fps) {
			avg_fps[i] = game.get_fps();
			i++;
		}
		else {
			i = 0;
			int sum = 0;
			for (int j = 0; j < div_fps; j++) {
				sum += avg_fps[j];
				avg_fps[j] = 0;
			}
			sum /= div_fps;
			div_fps = sum;

			text.set_text("FPS: " + to_string(sum) + " | Aprox frame delta(ms): " + to_string(_dt));
		}
		text.draw();
		if (game.is_key_just_pressed(SDL_SCANCODE_SPACE)) {
			cout << "Space just pressed" << endl;
		}
		else if (game.is_key_just_released(SDL_SCANCODE_SPACE)) {
			cout << "Space just released" << endl;
		}
		game.update();
	}
	return 0;
	/*
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Could not initialize SDL video";
		return -1;
	}

	bool _run = true;
	SDL_Event event;
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	Uint32 current_time = SDL_GetTicks();

	SDL_Surface* image = IMG_Load("preview.png");

	//Create window, and get it's surface and renderer
	window = SDL_CreateWindow("The Snow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920 / 4, 1080 / 4, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 30, 50, 70, 255);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = image->w;
	rect.h = image->h;
	SDL_FreeSurface(image);

	while (_run) {
		//Fill the window to clear previous frame
		SDL_RenderClear(renderer);

		//Poll window events
		while (SDL_PollEvent(&event)) {
			poll_event(event, _run);
		}
		SDL_PumpEvents();
		const Uint8* keystate = SDL_GetKeyboardState(NULL);


		//Main loop

		if (keystate[SDL_SCANCODE_UP]) {
			rect.y -= 1;
		}
		if (keystate[SDL_SCANCODE_DOWN]) {
			rect.y += 1;
		}
		if (keystate[SDL_SCANCODE_LEFT]) {
			rect.x -= 1;
		}
		if (keystate[SDL_SCANCODE_RIGHT]) {
			rect.x += 1;
		}

		SDL_RenderCopy(renderer, texture, NULL, &rect);

		//End of main loop



		//Update the window to show drawn opjects
		SDL_RenderPresent(renderer);

		//Wait for delta, to have consistant FPS
		int delay = 16 - (SDL_GetTicks() - current_time);
		if (delay > 0) {
			SDL_Delay(delay);
		}
		current_time = SDL_GetTicks();
	}

	//Destroy the window and close SDL
	SDL_DestroyWindow(window);
	SDL_Quit();
	*/
}
