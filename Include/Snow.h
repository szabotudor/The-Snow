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
		iVector resolution;
		SDL_Event events[32];
	private:
		iVector sdl_window_size;
		SDL_Rect viewport;
		SDL_Window* window = NULL;
		SDL_Surface* surface = NULL;
		SDL_Renderer* render = NULL;
		int num_events = 0;
		int frame_delay = 16;
		int current_frame_delay;
		unsigned int fps;
		unsigned int target_fps = 60;
		unsigned int frame_wait_time;
		long long time;
		bool _run = true;
		//Uint8* previous_keystate = { 0 };
		//Uint8* keystate = { 0 };
		
		bool previous_keystate[512];
		bool keystate[512];

		//Poll game events
		void poll_events();
		//Scale the window and center the renderer
		void scale_window(int w, int h);
	public:
		Snow(const char* name, iVector resolution = iVector(512, 288), Uint32 SDL_falgs = SDL_WINDOW_SHOWN, unsigned int framerate = 60);
		//Function to update the SDL2 components
		void update();
		//Function to clear the screen to a color
		void clear_screen(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
		//Returns the time since init
		long long get_time();
		//Returns the frames per second
		int get_fps();
		//Returns the SDL window (used especially for automatic texture scaling)
		SDL_Window* get_window();
		//Sets fullscreen mode to the given value
		void set_fullscreen(bool fs);
		//Returns true if the game window is in fullscreen mode
		bool get_fullscreen();
		//Returnes true if the SDL_KeyScancode is pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_pressed(Uint8 key);
		//Returnes true if the SDL_KeyScancode is just pressed this frame pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_just_pressed(Uint8 key);
		//Returnes true if the SDL_KeyScancode is just released this frame pressed (eg: SDL_SCANCODE_RETURN)
		bool is_key_just_released(Uint8 key);
		//Checks if the engine is still running
		bool running(float &delta_time);
		//Returns the number of events in the queue
		int get_num_events();
		//Sets the target fps (set to negative for unlimited)
		void set_target_framerate(unsigned int framerate);
		//Gets the target fps priorly set
		unsigned int get_target_framerate();
		//Stops all processes and background threads running in the helper library
		void quit();
	};
}
