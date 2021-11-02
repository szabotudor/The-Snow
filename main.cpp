#include<Snow.h>


void poll_event(SDL_Event event, bool &_run) {
	if (event.type == SDL_QUIT) {
		_run = false;
	}
}


int main(int argc, char* args[]) {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO)) {
		cout << "Could not initialize SDL video";
		return -1;
	}

	bool _run = true;
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Event event;

	//Create window, and get it's surface
	window = SDL_CreateWindow("The Snow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 288, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);

	while (_run) {
		//Poll window events
		while (SDL_PollEvent(&event)) {
			poll_event(event, _run);
		}
		//Fill the window to clear previous frame
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 30, 50, 70));

		//Main loop

		//End of main loop

		//Update the window to show drawn opjects
		SDL_UpdateWindowSurface(window);
	}
	//Destroy the window and close SDL
	SDL_DestroyWindow(window);
	SDL_Quit();
}
