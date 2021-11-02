#pragma once
#include<thread>
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<Vector.h>
#include<Button.h>
#include<Text.h>

using namespace std;

namespace ss {
	class Snow {
	public:
		Vector resolution;
	private:
		SDL_Window* window = NULL;
		SDL_Surface* surface = NULL;
		SDL_Renderer* render = NULL;
		SDL_Event event;
		int frame_delay = 16;
		Uint32 time = 0;
		bool _run = true;
		Uint8* keystate = { 0 };

		//Poll game events
		void poll_events();
	public:
		Snow(const char* name, Vector resolution = Vector(512, 288), Uint32 SDL_falgs = SDL_WINDOW_SHOWN, unsigned int framerate = 60);
		//Function to update the SDL2 components
		void update();
		//Function to clear the screen to a color
		void clear_screen(Uint8 r = 30, Uint8 g = 50, Uint8 b = 70, Uint8 a = 255);
		//Returns the time since init
		Uint32 get_time();
		//Redurns the SDL renderer
		SDL_Renderer* get_renderer();
		//Returnes true if the SDL_KeyScancode is pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_pressed(Uint8 key);
		//Checks if the engine is still running
		bool running();
		//Stops all processes and background threads running in the helper library
		void quit();
	};
}
