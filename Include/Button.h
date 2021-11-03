#pragma once
#include<string.h>

using namespace std;


namespace ss {
	class Button {
	public:
		enum class Type {
			Press,
			Toggle
		};
		enum class BackgroundType {
			Rect,
			Empty,
			Texture
		};
		enum class State {
			OFF,
			ON
		};
		Uint8 font_size;
		State state = State::OFF;
		bool disabled = false;
		bool hovered = false;
		bool pressed = false;
		bool just_pressed = false;
		bool just_released = false;
		//Mouse button used to click the button
		const unsigned int mouse_button = SDL_BUTTON_LEFT;

	private:
		int border = 0;
		Type type;
		BackgroundType background_type;
		string text;
		SDL_Color text_color, border_color, fill_color;
		TTF_Font* font;
		SDL_Surface* surface;
		SDL_Texture* texture;
		SDL_Renderer* render;
		SDL_Rect rect, border_rect;
		Vector position;

	public:
		Button(SDL_Renderer* renderer, const char* text = "Button", const char* font = "basic.ttf", unsigned int font_size = 18);
		Button(SDL_Renderer* renderer, SDL_Color bgd_color, SDL_Color border_color, SDL_Color text_color, unsigned int border_thickness, const char* text, const char* font = "basic.ttf", unsigned int font_size = 18);
		Button(SDL_Renderer* renderer, SDL_Color text_color, const char* text, const char* font = "basic.ttf", unsigned int font_size = 18);
		//Button(Type type = Type::Press, const char* text = "Button", const char* font = "Pixel.ttf", BackgroundType background_type = BackgroundType::Rect);

		//Sets the button text to the given parameter
		void set_text(char* text);
		//Returns the text set on the button
		string get_text();
		//Loads specified font and uses it on the button
		void set_font(char* font);
		//Sets the position of the button (by layer)
		void set_position(int x, int y);
		//Sets the window the button should be drawn to
		void draw();
		//Updates the button state, based on mouse location and clicks
		void update(SDL_Window& window);
		//Call to set press type (toggle, or normal)
		void set_toggle(bool ON);
	};
}
