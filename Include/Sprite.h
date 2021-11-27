#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<Vector.h>


namespace ss {
	class Sprite {
		int frames, frame;
		SDL_Window* window;
		SDL_Renderer* render;
		SDL_Surface* surface;
		SDL_Texture** textures;
		SDL_Rect rect;
	public:
		Vector position;
		Sprite(SDL_Window *window, const char* texture);
		Sprite(SDL_Window* window, int frames, const char ** textures);
		//Draws the sprite on the window
		void draw();
	};
}
