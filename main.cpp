#include<Snow.h>


void poll_event(SDL_Event event, bool &_run) {
	if (event.type == SDL_QUIT) {
		_run = false;
	}
}


int main(int argc, char* args[]) {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Could not initialize SDL video";
		return -1;
	}

	bool _run = true;
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Event event;
	SDL_Renderer* renderer;
	Uint32 current_time = SDL_GetTicks();

	SDL_Surface* image = IMG_Load("preview.png");
	SDL_Rect rect;
	rect.w = image->w;
	rect.h = image->h;
	rect.x = 0;
	rect.y = 0;

	//Create window, and get it's surface
	window = SDL_CreateWindow("The Snow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920 / 4, 1080 / 4, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);

	while (_run) {
		//Fill the window to clear previous frame
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 30, 50, 70));
		//Poll window events
		while (SDL_PollEvent(&event)) {
			poll_event(event, _run);
		}
		SDL_PumpEvents();
		const Uint8* keystate = SDL_GetKeyboardState(NULL);


		//Main loop
		if (keystate[SDL_SCANCODE_UP]) {
			rect.y += 1;
			rect.h -= 1;
		}
		if (keystate[SDL_SCANCODE_DOWN]) {
			rect.y -= 1;
			rect.h += 1;
		}
		if (keystate[SDL_SCANCODE_LEFT]) {
			rect.x += 1;
			rect.w -= 1;
		}
		if (keystate[SDL_SCANCODE_RIGHT]) {
			rect.x -= 1;
			rect.w += 1;
		}

		SDL_BlitSurface(image, &rect, surface, NULL);
		//End of main loop



		//Update the window to show drawn opjects
		SDL_UpdateWindowSurface(window);

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
}
