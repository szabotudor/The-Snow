#pragma once
#include<Snow.h>


void init_snowballs(ss::Snow& game);


class Snowball {
	ss::RandomNumberGenerator rng;
	ss::Vector position, direction;
	double height = 0;
	double v_velocity = -8;
	SDL_Window* window;
	SDL_Renderer* render;
	double scale;
	ss::ParticleEmitter* ptem;
	bool visible = true;
	bool melted = false;
public:
	ss::CollisionShape collision;

	Snowball(SDL_Window* window, ss::Vector position, ss::Vector direction, double scale = 1);
	//Advance the snowball's position forward
	void update(double delta, ss::Texture& ground_texture, bool**& ground_bool, long long& snow_ammount, ss::CollisionShape& player_cs);
	//Draw the snowball on the window
	void draw(ss::Vector camera_offset);
	//Returns true if the snowball hit the player or the ground
	bool is_melted();
	//Frees all components in the snowball
	void free();
};
