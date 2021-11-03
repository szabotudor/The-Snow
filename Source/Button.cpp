#include<Snow.h>

ss::Button::Button(SDL_Renderer* renderer, const char* text, const char* font, unsigned int font_size) {
	TTF_Init();
	Button::font = TTF_OpenFont(font, font_size);
	render = renderer;
	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
	text_color.a = 255;
	surface = TTF_RenderText_Solid(Button::font, text, text_color);
	texture = SDL_CreateTextureFromSurface(render, surface);
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_FreeSurface(surface);
	Button::text = text;
	Button::font_size = font_size;

	background_type = BackgroundType::Empty;
	type = Type::Press;
}

ss::Button::Button(SDL_Renderer* renderer, SDL_Color bgd_color, SDL_Color border_color, SDL_Color text_color, unsigned int border_thickness, const char* text, const char* font, unsigned int font_size) {
	TTF_Init();
	Button::font = TTF_OpenFont(font, font_size);
	render = renderer;
	Button::text_color = text_color;
	Button::fill_color = bgd_color;
	Button::border_color = border_color;
	border = border_thickness;
	surface = TTF_RenderText_Solid(Button::font, text, text_color);
	texture = SDL_CreateTextureFromSurface(render, surface);
	rect.w = surface->w;
	rect.h = surface->h;
	border_rect.x = -border;
	border_rect.y = -border;
	border_rect.w = surface->w + border * 2;
	border_rect.h = surface->h + border * 2;
	SDL_FreeSurface(surface);
	Button::text = text;
	Button::font_size = font_size;

	background_type = BackgroundType::Rect;
	type = Type::Press;
}

void ss::Button::draw() {
	SDL_SetRenderDrawColor(render, border_color.r, border_color.g, border_color.b, border_color.a);
	SDL_RenderDrawRect(render, &border_rect);
	SDL_SetRenderDrawColor(render, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	SDL_RenderDrawRect(render, &rect);
	SDL_SetRenderDrawColor(render, text_color.r, text_color.g, text_color.b, text_color.a);
	SDL_RenderCopy(render, texture, NULL, &rect);
}
