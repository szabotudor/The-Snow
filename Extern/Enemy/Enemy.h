#pragma once
#include<Snow.h>

void init_enemy(ss::Snow& game);


class Enemy {
	ss::Vector position;
	ss::Vector velocity;
	ss::Vector target;
	int life = 3;
	double invulnerability = 1;
	double lifetime = 0;
	enum class LookDirection {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
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
