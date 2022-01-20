#pragma once
#include<Snow.h>


class Snowball {
	ss::RandomNumberGenerator rng;
	ss::Vector position, direction;
	SDL_Window* window;
	SDL_Renderer* render;
	double scale;
	ss::Texture* tex_big;
	ss::ParticleEmitter* ptem;
public:
	Snowball(SDL_Window* window, ss::Vector position, ss::Vector direction, double scale);
	//Advance the snowball's position forward
	void update();
	//Draw the snowball on the window
	void draw();
};
