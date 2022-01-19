#pragma once
#include<Snow.h>

void init_enemy(ss::Snow& game);


class Enemy {
	ss::RandomNumberGenerator rng;
	ss::Vector position;
	double height = 0;
	ss::Vector velocity;
	double vert_velocity = 0;
	ss::Vector target;
	ss::Vector col_draw_offset = ss::Vector(0, 12);
	bool visible;
	double visibility_timer = 0;
	double rot_speed = 0;
	int life = 3;
	double invulnerability = 1;
	double lifetime = 0;
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
	Anim animation = Anim::IDLE;
	ss::Vector draw_offset = ss::Vector(0);
	double anim_time = 0;
	LookDirection look_dir = LookDirection::DOWN;
public:
	int id = 0;
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
