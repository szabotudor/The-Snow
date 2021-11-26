#include<Sprite.h>

ss::Sprite::Sprite(SDL_Window *window, const char* texture) {
	frames = 0;
	IMG_Init(IMG_INIT_PNG);
	surface = IMG_Load(texture);
	render = SDL_GetRenderer(window);
	this->textures[0] = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);
}

ss::Sprite::Sprite(SDL_Window* window, int frames, const char** textures) {
	this->frames = frames;
	render = SDL_GetRenderer(window);
	IMG_Init(IMG_INIT_PNG);
	for (int i = 0; i < frames; i++) {
		surface = IMG_Load(textures[i]);
		this->textures[i] = SDL_CreateTextureFromSurface(render, surface);
		SDL_FreeSurface(surface);
	}
}
