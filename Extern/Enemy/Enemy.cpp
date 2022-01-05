#include"Enemy.h"


Enemy::Enemy(ss::Vector position) {
	Enemy::position = position;
	col[0] = ss::CollisionShape(12, position);
}

void Enemy::process(double delta) {
	delta /= 1000;
	if (invulnerability > 0) {
		invulnerability -= delta;
	}
	else {

	}
}