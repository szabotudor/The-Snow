#include<Snow.h>


void ss::Snow::poll_events() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			quit();
		}
	}
}

ss::Snow::Snow(const char* name, ss::Vector resolution, Uint32 SDL_flags, unsigned int framerate) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Could not initialize SDL video: " << SDL_GetError();
	}

	Snow::resolution = resolution;
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution.x, resolution.y, SDL_flags);
	surface = SDL_GetWindowSurface(window);
	render = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	frame_delay = 1000 / framerate;
	time = SDL_GetTicks();
}

void ss::Snow::update() {
	if (!_run) {
		return;
	}
	SDL_RenderPresent(render);
	poll_events();
	keystate = (Uint8*)SDL_GetKeyboardState(NULL);

	unsigned int delay = frame_delay - (SDL_GetTicks() - time);
	if (delay > 0 and delay < frame_delay + 1) {
		SDL_Delay(delay);
	}
	time = SDL_GetTicks();
}

void ss::Snow::clear_screen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(render, r, g, b, a);
	SDL_RenderClear(render);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
}

Uint32 ss::Snow::get_time() {
	return time;
}

SDL_Renderer* ss::Snow::get_renderer() {
	return render;
}

bool ss::Snow::is_key_pressed(Uint8 key) {
	return keystate[key];
}

bool ss::Snow::running() {
	return _run;
}

void ss::Snow::quit() {
	SDL_DestroyWindow(window);
	SDL_Quit();
	_run = false;
}
