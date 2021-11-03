#include<Text.h>


ss::Text::Text(SDL_Renderer* renderer, const char* text, const char* font, Uint8 font_size) {
	TTF_Init();
	render = renderer;
	Text::font = TTF_OpenFont(font, font_size);
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	surface = TTF_RenderText_Solid(Text::font, text, color);
	texture = SDL_CreateTextureFromSurface(render, surface);
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_FreeSurface(surface);
	Text::text = text;
	Text::font_size = font_size;
}

void ss::Text::set_font(const char* font, Uint8 font_size) {
	Text::font = TTF_OpenFont(font, font_size);
	surface = TTF_RenderText_Solid(Text::font, text.c_str(), color);
	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);
	Text::font_size = font_size;
}

void ss::Text::set_text(const char* text) {
	surface = TTF_RenderText_Solid(Text::font, text, color);
	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(render, surface);
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_FreeSurface(surface);
	Text::text = text;
}

void ss::Text::set_text(string text) {
	set_text(text.c_str());
}

void ss::Text::draw() {
	if (rect.x != position.x or rect.y != position.y) {
		rect.x = position.x;
		rect.y = position.y;
	}
	SDL_RenderCopy(render, texture, NULL, &rect);
}

string ss::Text::get_text() {
	return Text::text;
}
