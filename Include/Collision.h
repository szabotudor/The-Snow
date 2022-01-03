#pragma once
#include<Vector.h>
#include<SDL.h>

namespace ss {
	class CollisionShape {
		SDL_Window* window;
		SDL_Renderer* render;
		bool draw_enabled = false;
		SDL_Rect rect;
	public:
		Vector position;
		Vector size;
		CollisionShape(Vector size = Vector(1), Vector position = Vector(0), SDL_Window* window = nullptr);
		//Returns true if this is overlapping with the given CollisionShape
		bool is_colliding_with(CollisionShape cs);
		//Enables drawing of the CollisionShape to the window (usually only used in debug)
		void enable_draw(SDL_Window* window);
		//Draws the CollisionShape to the window (usually only used in debug)
		void draw();
	};
}
