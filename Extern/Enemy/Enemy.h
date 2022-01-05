#pragma once
#include<Snow.h>


static class EnemyUtil {
public:
	bool init = false;
	SDL_Window* window = nullptr;
	ss::Sprite* enemy_sprite;
	void init_enemy(ss::Snow& game);
};


class Enemy {
	ss::Vector position;
	ss::Vector velocity;
	ss::Vector target;
	int life = 3;
	double invulnerability = 1;
public:
	int id = 0;
	ss::CollisionShape collision;
	Enemy(ss::Vector position = ss::Vector(0));
	//Process movement, damage, invulnerability, etc...
	void process(double delta);
	//Damages the enemy by 1 point and gives it invulnerability for a short time
	void damage();
	//Draw all sprites
	void draw();
	//Returns true if the enemy is dead
	bool is_dead();
	//Returns the health points (life) of the enemy
	int get_hp();
	//Returns true if the enemy is invulnerable
	bool is_invulnerable();
};
