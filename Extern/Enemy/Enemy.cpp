#include"Enemy.h"


Enemy::Enemy(ss::Vector position) {
	Enemy::position = position;
	collision = ss::CollisionShape(12, position);
}

void Enemy::process(double delta) {
	delta /= 1000;
	if (invulnerability > 0) {
		invulnerability -= delta;
#if defined _DEBUG
		collision.draw_color.r = 255;
		collision.draw_color.b = 120;
#endif
	}
	else {
#if defined _DEBUG
		collision.draw_color.r = 120;
		collision.draw_color.b = 255;
#endif
	}
}

void Enemy::damage() {
	if (invulnerability <= 0) {
		life--;
		invulnerability = 1;
	}
}

void Enemy::draw() {
}

bool Enemy::is_dead() {
	return life <= 0;
}

int Enemy::get_hp() {
	return life;
}

bool Enemy::is_invulnerable() {
	return invulnerability >= 0;
}

void EnemyUtil::init_enemy(ss::Snow& game) {
	init = true;
	const char* textures[3] = {
		"t1",
		"t2",
		"t3",
	};
	*enemy_sprite = ss::Sprite(game.get_window(), 3, textures);
}
