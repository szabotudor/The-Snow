#pragma once
#include<chrono>
#include<iostream>
#include<string>
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
		int current_frame_delay;
		unsigned int fps;
		unsigned int frame_wait_time;
		long long time;
		bool _run = true;
		//Uint8* previous_keystate = { 0 };
		//Uint8* keystate = { 0 };
		
		bool previous_keystate[512];
		bool keystate[512];

		//Poll game events
		void poll_events();
	public:
		Snow(const char* name, Vector resolution = Vector(512, 288), Uint32 SDL_falgs = SDL_WINDOW_SHOWN, unsigned int framerate = 60);
		//Function to update the SDL2 components
		void update();
		//Function to clear the screen to a color
		void clear_screen(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
		//Returns the time since init
		long long get_time();
		//Returns the frames per second
		int get_fps();
		//Redurns the SDL renderer
		SDL_Renderer* get_renderer();
		//Returnes true if the SDL_KeyScancode is pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_pressed(Uint8 key);
		//Returnes true if the SDL_KeyScancode is just pressed this frame pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_just_pressed(Uint8 key);
		//Returnes true if the SDL_KeyScancode is just released this frame pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_just_released(Uint8 key);
		//Checks if the engine is still running
		bool running(float &delta_time);
		//Stops all processes and background threads running in the helper library
		void quit();
	};
}
