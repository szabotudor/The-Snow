#include<Sprite.h>

ss::Sprite::Sprite(SDL_Window *window, const char* texture) {
	frames = 0;
	frame = 0;
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
	frame = 0;
	render = SDL_GetRenderer(window);
	IMG_Init(IMG_INIT_PNG);
	Sprite::textures = new SDL_Texture * [frames];
	for (int i = 0; i < frames; i++) {
		surface = IMG_Load(textures[i]);
		Sprite::textures[i] = SDL_CreateTextureFromSurface(render, surface);
		if (!i) {
			rect = surface->clip_rect;
			rect.x = position.x;
			rect.y = position.y;
		}
		SDL_FreeSurface(surface);
	}
}

void ss::Sprite::load(SDL_Window* window, int frames, const char** textures) {

}

void ss::Sprite::draw(float delta) {
	if (rect.x != position.x or rect.y != position.y) {
		rect.x = position.x;
		rect.y = position.y;
	}
	SDL_RenderCopy(render, textures[frame], NULL, &rect);

	if (playing) {
		time += delta;
		frame = (int)(time / frame_delay);
		if (frame > end) {
			frame = 0;
			time = 0;
		}
	}
}

void ss::Sprite::play(int start, int end, int fps) {
	Sprite::start = start;
	Sprite::end = end;
	Sprite::frame_delay = 1000.0f / fps;
	playing = true;
}

void ss::Sprite::resume() {
	playing = true;
}

void ss::Sprite::stop() {
	frame = 0;
	time = 0;
	playing = false;
}

void ss::Sprite::pause() {
	playing = false;
}

ss::Vector ss::Sprite::get_size() {
	int w, h;
	SDL_QueryTexture(textures[frame], NULL, NULL, &w, &h);
	return Vector(w, h);
}
