#pragma once
#include<Snow.h>

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
	bool visible;
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
	double anim_time = 0;

	//Positional offsets
	ss::Vector col_draw_offset = ss::Vector(0, 6);
	ss::Vector draw_offset = ss::Vector(0);
	ss::Vector head_offset = 0, middle_offset = 0;
	ss::Vector ofs = ss::Vector(1, 0);
	ss::Vector position;
	ss::Vector velocity;
public:
	//The target position (the enemy will move towards the target and attack it)
	ss::Vector target;
	ss::CollisionShape collision;
	Enemy(ss::Vector position = ss::Vector(0));
	//Process movement, damage, invulnerability, etc...
	void process(double delta);
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
};
