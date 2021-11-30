#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<Vector.h>

using namespace std;

namespace ss {
	class Sprite {
		int frames, frame, start, end;
		float time = 0.0f, frame_delay;
		bool playing = false;
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
		void draw(float delta);
		//Starts playing an animation from the start frame to the end frame at the given fps
		void play(int start, int end, int fps);
	};
}
