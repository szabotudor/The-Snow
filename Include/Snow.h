#pragma once
#include<chrono>
#include<iostream>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<Vector.h>
#include<Button.h>
#include<Sprite.h>
#include<Text.h>

using namespace std;

namespace ss {
	class Snow {
	public:
		Vector resolution;
		SDL_Event events[32];
		unsigned int target_fps = 60;
	private:
		Vector sdl_window_size;
		SDL_Rect viewport;
		SDL_Window* window = NULL;
		SDL_Surface* surface = NULL;
		SDL_Renderer* render = NULL;
		int num_events = 0;
		unsigned int fps;
		long long time;
		Uint64 NOW = 0, LAST = 0;
		double delta_time;
		float target_wait_time;
		bool _run = true;
		int numkeys;
		const Uint8* checkstate;
		
		bool previous_keystate[512];
		bool keystate[512];

		//Poll game events
		void poll_events();
		//Scale the window and center the renderer
		void scale_window(int w, int h);
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
		//Returns the mouse position
		Vector get_mouse_position();
		//Returns the SDL window (used especially for automatic texture scaling)
		SDL_Window* get_window();
		//Sets fullscreen mode to the given value
		void set_fullscreen(bool fs);
		//Returns true if the game window is in fullscreen mode
		bool get_fullscreen();
		//Returnes true if the SDL_KeyScancode is pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_pressed(Uint8 key);
		//Returnes true if the SDL_KeyScancode is just pressed this frame (eg: SDL_SCANCODE_RETURN)
		bool is_key_just_pressed(Uint8 key);
		//Returnes true if the SDL_KeyScancode is just released this frame (eg: SDL_SCANCODE_RETURN)
		bool is_key_just_released(Uint8 key);
		//Checks if the engine is still running
		bool running(float &delta_time);
		//Returns the number of events in the queue
		int get_num_events();
		//Stops all processes and background threads running in the helper library
		void quit();
	};
}
