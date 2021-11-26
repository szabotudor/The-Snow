#include<Snow.h>

#if defined _DEBUG
bool IS_DEBUG = true;
#else
bool IS_DEBUG = false;
#endif // DEBUG



void ss::Snow::poll_events() {
	SDL_PumpEvents();
	for (num_events = 0; SDL_PollEvent(&events[num_events]) and num_events < 31; num_events++);
	/* Used for debugging, only in case of nothing else working
	if (num_events == 31) {
		cout << "Possible event overflow: " << num_events << " events" << endl;
	}*/
}

void ss::Snow::scale_window(int w, int h) {
	//If the window is to small, resize it
	if (w < resolution.x) {
		w = resolution.x;
		SDL_SetWindowSize(window, w, h);
	}
	if (h < resolution.y) {
		h = resolution.y;
		SDL_SetWindowSize(window, w, h);
	}
	//Calculate the scales and positions
	sdl_window_size = iVector(w, h);
	Vector scale((float)w / (float)resolution.x, (float)h / (float)resolution.y);
	//Two cases, one to add borders on the top and bottom...
	if (scale.x > scale.y) {
		viewport.x = (w - scale.y * resolution.x) / 2;
		viewport.y = 0;
		viewport.w = resolution.x * (float)scale.y;
		viewport.h = h;
		scale.x = scale.y;
	}
	//...and one to add borders on the left and right
	else {
		viewport.x = 0;
		viewport.y = (h - scale.x * resolution.y) / 2;
		viewport.w = w;
		viewport.h = resolution.y * (float)scale.x;
		scale.y = scale.x;
	}
	//Set the viewport
	viewport.x /= scale.x;
	viewport.y /= scale.y;
	SDL_RenderSetScale(render, scale.x, scale.y);
	SDL_RenderSetViewport(render, &viewport);
	//Prepare the viewport border to be drawn (here for debugging, not used in final game)
	/*
	if (IS_DEBUG) {
		viewport.x = 0;
		viewport.y = 0;
		viewport.w /= scale.x;
		viewport.h /= scale.y;
	}
	*/
}

ss::Snow::Snow(const char* name, ss::iVector resolution, Uint32 SDL_flags, unsigned int framerate) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Could not initialize SDL video: " << SDL_GetError();
	}
	if (TTF_Init() < 0) {
		cout << "Could not initialize TTF library: " << TTF_GetError();
	}

	target_fps = framerate;
	Snow::resolution = resolution;
	sdl_window_size = resolution;
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution.x, resolution.y, SDL_flags);
	surface = SDL_GetWindowSurface(window);
	render = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	frame_delay = 1000 / framerate;
}

void ss::Snow::update() {
	//Poll the events and stop if should stop
	poll_events();
	if (!_run) {
		return;
	}

	//Test for keyboard state
	int numkeys;
	const Uint8* checkstate = SDL_GetKeyboardState(&numkeys);
	for (int i = 0; i < numkeys; i++) {
		previous_keystate[i] = keystate[i];
		keystate[i] = (bool)checkstate[i];
	}

	//Calculate frame delta and framerate
	current_frame_delay = (SDL_GetTicks() - time);
	if (current_frame_delay > 0 and current_frame_delay <= frame_delay) {
		frame_wait_time = frame_delay - current_frame_delay + (int)(fps >= target_fps);
		SDL_Delay(frame_wait_time);
	}
	else if (!frame_delay){
		frame_wait_time = 0;
	}
	time = SDL_GetTicks();
	if (frame_wait_time + current_frame_delay) {
		fps = 1000 / (frame_wait_time + current_frame_delay);
	}
	else {
		fps = 1001;
	}

	//Calculate window scale, accordin to resolution
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	//Set window scale and center the viewport
	if ((sdl_window_size.x != w or sdl_window_size.y != h)) {
		scale_window(w, h);
		//scale_window(w, h);
	}
	//Draw a border around the renderer's viewport (used only in debug mode)
	/*
	if (IS_DEBUG) {
		SDL_RenderDrawRect(render, &viewport);
	}
	*/
	//Present renderer (update the screen)
	SDL_RenderPresent(render);
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


SDL_Window* ss::Snow::get_window() {
	return window;
}

void ss::Snow::set_fullscreen(bool fs) {
	if (fs) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
	}
}

bool ss::Snow::get_fullscreen() {
	return SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
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

ss::Vector ss::Snow::get_mouse_position() {
	int x, y;
	float sx, sy;
	SDL_RenderGetScale(render, &sx, &sy);
	SDL_GetMouseState(&x, &y);
	x /= sx;
	y /= sy;
	x -= viewport.x;
	y -= viewport.y;
	return Vector(x, y);
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

int ss::Snow::get_num_events() {
	return num_events;
}

void ss::Snow::set_target_framerate(unsigned int framerate) {
	target_fps = framerate;
	if (framerate) {
		frame_delay = 1000 / framerate;
	}
	else {
		frame_delay = 0;
	}
}

unsigned int ss::Snow::get_target_framerate() {
	return target_fps;
}

void ss::Snow::quit() {
	SDL_DestroyWindow(window);
	SDL_Quit();
	_run = false;
}
