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
	public:
		Snow();
		//Stops all processes and background threads running in the helper library
		void quit();
	private:
		//Functiona to update the screen graphics
		void graphics_process();
		bool _run;
	};
}
