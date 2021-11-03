#include<Snow.h>


void ss::Snow::poll_events() {
	SDL_PumpEvents();
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

	target_fps = framerate;
	Snow::resolution = resolution;
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution.x, resolution.y, SDL_flags);
	surface = SDL_GetWindowSurface(window);
	render = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	frame_delay = 1000 / framerate;
}

void ss::Snow::update() {
	poll_events();
	if (!_run) {
		return;
	}

	SDL_RenderPresent(render);


	int numkeys;
	const Uint8* checkstate = SDL_GetKeyboardState(&numkeys);
	for (int i = 0; i < numkeys; i++) {
		previous_keystate[i] = keystate[i];
		keystate[i] = (bool)checkstate[i];
	}

	current_frame_delay = (SDL_GetTicks() - time);
	//cout << SDL_GetTicks() << " " << time << " " << current_frame_delay << endl;
	if (current_frame_delay > 0 and current_frame_delay <= frame_delay) {
		frame_wait_time = frame_delay - current_frame_delay + (int)(fps >= target_fps);
		SDL_Delay(frame_wait_time);
	}
	time = SDL_GetTicks();
	if (frame_wait_time + current_frame_delay) {
		fps = 1000 / (frame_wait_time + current_frame_delay);
	}
	else {
		fps = 1001;
	}
}

void ss::Snow::clear_screen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(render, r, g, b, a);
	SDL_RenderClear(render);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
}

long long ss::Snow::get_time() {
	return time;
}

int ss::Snow::get_fps() {
	return fps;
}

SDL_Renderer* ss::Snow::get_renderer() {
	return render;
}

bool ss::Snow::is_key_pressed(Uint8 key) {
	if (!keystate) {
		return false;
	}
	else {
		return keystate[key];
	}
}

bool ss::Snow::is_key_just_pressed(Uint8 key) {
	if (!keystate or !previous_keystate) {
		return false;
	}
	else {
		return (!previous_keystate[key] and keystate[key]);
	}
}

bool ss::Snow::is_key_just_released(Uint8 key) {
	if (!keystate or !previous_keystate) {
		return false;
	}
	else {
		return (previous_keystate[key] and !keystate[key]);
	}
}

bool ss::Snow::running(float &delta_time) {
	delta_time = current_frame_delay + frame_wait_time;
	return _run;
}

void ss::Snow::set_target_framerate(unsigned int framerate) {
	target_fps = framerate;
	frame_delay = 1000 / framerate;
}

unsigned int ss::Snow::get_target_framerate() {
	return target_fps;
}

void ss::Snow::quit() {
	SDL_DestroyWindow(window);
	SDL_Quit();
	_run = false;
}
