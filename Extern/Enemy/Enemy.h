#pragma once
#include<Snow.h>


class Enemy {
	ss::Vector position;
	ss::Vector velocity;
	ss::Vector target;
	int life = 3;
	double invulnerability = 1;
	ss::CollisionShape col[3];
public:
	Enemy(ss::Vector position = ss::Vector(0));
	//Process movement, damage, invulnerability, etc...
	void process(double delta);
};
