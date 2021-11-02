#pragma once
#include<thread>
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<Vector.h>
#include<Button.h>

using namespace std;

namespace ss {
	class Snow {
		SDL_Window* window;
		SDL_Surface* surface;
		SDL_Renderer* render;
		SDL_Event ev;
		unsigned int frame_delay = 16;
	public:
		Snow(const char* name = "The Snow", Vector resolution = Vector(512, 288), Uint32 SDL_falgs = SDL_WINDOW_SHOWN, unsigned int framerate = 60);
		//Function to update the SDL2 components
		void update();
		//Function to clear the screen to a color
		void clear(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
		//Stops all processes and background threads running in the helper library
		void quit();
	private:
		bool _run;
	};
}
