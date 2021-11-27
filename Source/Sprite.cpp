#include<Sprite.h>

ss::Sprite::Sprite(SDL_Window *window, const char* texture) {
	frames = 0;
	IMG_Init(IMG_INIT_PNG);
	surface = IMG_Load(texture);
	render = SDL_GetRenderer(window);
	textures = new SDL_Texture * [1];
	Sprite::textures[0] = SDL_CreateTextureFromSurface(render, surface);
	rect = surface->clip_rect;
	rect.x = position.x;
	rect.y = position.y;
	SDL_FreeSurface(surface);
}

ss::Sprite::Sprite(SDL_Window* window, int frames, const char** textures) {
	Sprite::frames = frames;
	render = SDL_GetRenderer(window);
	IMG_Init(IMG_INIT_PNG);
	Sprite::textures = new SDL_Texture * [frames];
	for (int i = 0; i < frames; i++) {
		surface = IMG_Load(textures[i]);
		Sprite::textures[i] = SDL_CreateTextureFromSurface(render, surface);
		SDL_FreeSurface(surface);
	}
}

void ss::Sprite::draw() {
	if (rect.x != position.x or rect.y != position.y) {
		rect.x = position.x;
		rect.y = position.y;
	}
	if (!frames) {
		SDL_RenderCopy(render, textures[0], NULL, &rect);
	}
}
