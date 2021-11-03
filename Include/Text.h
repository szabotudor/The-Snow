#pragma once
#include<Snow.h>


namespace ss {
	class Text {
		Uint8 font_size = 0;
		string text = "";
		SDL_Color color;
		TTF_Font* font;
		SDL_Texture* texture;
		SDL_Surface* surface;
		SDL_Renderer* render;
		SDL_Rect rect;
	public:
		Vector position;
		Text(SDL_Renderer* renderer, const char* text, const char* font, Uint8 font_size = 24);
		//Sets the font file given as the Text object's font
		void set_font(const char* font, Uint8 font_size);
		//Sets the text to the given string
		void set_text(const char* text);
		//Sets the text to the given string
		void set_text(string text);
		//Draws the text on screen
		void draw();
		//Returns the text set on the Text object
		string get_text();
	};
}
