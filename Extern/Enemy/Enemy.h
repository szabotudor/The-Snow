#pragma once
#include<Snow.h>
#include"../Snowball/Snowball.h"


void init_enemy(ss::Snow& game);


class Enemy {
	//Random number generator
	ss::RandomNumberGenerator rng;

	//Enumerators for the possible states of the enemy
	enum class LookDirection {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
	enum class Anim {
		IDLE,
		SHOOT,
		WALK
	};
	LookDirection look_dir = LookDirection::DOWN;
	Anim animation = Anim::IDLE;
	bool visible = false;
	int life = 3;

	//Timers and other float counters
	double vert_velocity = 0;
	double height = 0;
	double rot_speed = 0;
	double visibility_timer = 0;
	double invulnerability = 1;
	double lifetime = 0;
	double spawn_timer = 0;
	double shoot_timer = 0;
	double random_move_timer = 0;
	double anim_time = 0;
	double snowball_timer = 2;

	double total_lifetime = 0;

	//Positional offsets
	ss::Vector col_draw_offset = ss::Vector(0, 6);
	ss::Vector draw_offset = ss::Vector(0);
	ss::Vector head_offset = 0, middle_offset = 0;
	ss::Vector ofs = ss::Vector(1, 0);
	ss::Vector position;
	ss::Vector velocity;
public:
	int move_speed = 45;
	double snowball_throw_delay = 2;
	bool attacking = false;
	//The enemy will shoot at this position
	ss::Vector target;

	//The enemy will movie towards this position
	ss::Vector move_target;

	ss::CollisionShape collision;
	//The are of effect for freezing the ground
	double aoe = 0;
	ss::Vector aoe_offset = ss::Vector(-1, 10);
	Enemy(ss::Vector position = ss::Vector(0));
	//Process movement, damage, invulnerability, etc...
	void process(double delta, int& num_of_snowballs, ss::Snow& game, Snowball**& snowball_list);
	//Damages the enemy by 1 point and gives it invulnerability for a short time
	void damage();
	//Draw all sprites
	void draw(ss::Vector camera_offset);
	//Returns true if the enemy is dead
	bool is_dead();
	//Returns the health points (life) of the enemy
	int get_hp();
	//Returns true if the enemy is invulnerable
	bool is_invulnerable();
	//Returns the total lifetime of the enemy
	uint32_t get_lifetime();
	//Returns the position of the enemy
	ss::Vector get_position();
};
