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
	bool first_play = true;
public:
	ss::CollisionShape collision;

	Snowball(SDL_Window* window, ss::Vector position, ss::Vector direction, double scale = 1);
	//Advance the snowball's position forward
	void update(double delta, ss::Texture& ground_texture, bool**& ground_bool, long long& snow_ammount, ss::CollisionShape& player_cs, ss::Vector& ground_size, bool& hit_player);
	//Draw the snowball on the window
	void draw(ss::Vector camera_offset);
	//Returns the directions in which the snowball is moving
	ss::Vector get_direction();
	//Returns true if the snowball hit the player or the ground
	bool is_melted();
	//Force the snowball to melt
	void force_melt();
	//Frees all components in the snowball
	void free();
};
