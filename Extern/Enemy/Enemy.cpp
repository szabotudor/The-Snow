#include"Enemy.h"


bool init = false;
SDL_Window* window = nullptr;
ss::Sprite enemy_sprite;

void init_enemy(ss::Snow& game) {
	init = true;
	const char* textures[6] = {
		"Sprites/Snowman/snowman_body0000.png",
		"Sprites/Snowman/snowman_body0001.png",
		"Sprites/Snowman/snowman_body0002.png",

		"Sprites/Snowman/snowman_head0000.png",
		"Sprites/Snowman/snowman_head0001.png",
		"Sprites/Snowman/snowman_head0002.png"
	};
	enemy_sprite = ss::Sprite(game.get_window(), 6, textures);
}


Enemy::Enemy(ss::Vector position) {
	Enemy::position = position;
	collision = ss::CollisionShape(ss::Vector(17, 27), position);
}

void Enemy::process(double delta) {
	delta /= 1000;
	lifetime += delta;
	if (lifetime > 0.8) {
		lifetime = 0;
		switch (look_dir) {
		case Enemy::LookDirection::UP:
			look_dir = LookDirection::RIGHT;
			break;
		case Enemy::LookDirection::DOWN:
			look_dir = LookDirection::LEFT;
			break;
		case Enemy::LookDirection::LEFT:
			look_dir = LookDirection::UP;
			break;
		case Enemy::LookDirection::RIGHT:
			look_dir = LookDirection::DOWN;
			break;
		default:
			break;
		}
	}
	position = collision.position;
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

void Enemy::draw(ss::Vector camera_offset) {
	enemy_sprite.position = position - camera_offset + ss::Vector(0, 13) - ss::Vector(1, 4);
#if defined _DEBUG
	collision.position -= camera_offset;
	collision.draw();
	collision.position += camera_offset;
#endif
	switch (look_dir) {
	case Enemy::LookDirection::UP:
		enemy_sprite.flip = SDL_FLIP_NONE;
		enemy_sprite.frame = 2;
		enemy_sprite.draw(1);
		enemy_sprite.frame = 5;
		enemy_sprite.position += ss::Vector(2, -11);
		enemy_sprite.draw(1);
		break;
	case Enemy::LookDirection::DOWN:
		enemy_sprite.flip = SDL_FLIP_NONE;
		enemy_sprite.frame = 1;
		enemy_sprite.draw(1);
		enemy_sprite.frame = 4;
		enemy_sprite.position += ss::Vector(2, -11);
		enemy_sprite.draw(1);
		break;
	case Enemy::LookDirection::LEFT:
		enemy_sprite.flip = SDL_FLIP_HORIZONTAL;
		enemy_sprite.frame = 0;
		enemy_sprite.draw(1);
		enemy_sprite.frame = 3;
		enemy_sprite.position += ss::Vector(2, -11);
		enemy_sprite.draw(1);
		break;
	case Enemy::LookDirection::RIGHT:
		enemy_sprite.flip = SDL_FLIP_NONE;
		enemy_sprite.frame = 0;
		enemy_sprite.draw(1);
		enemy_sprite.frame = 3;
		enemy_sprite.position += ss::Vector(2, -11);
		enemy_sprite.draw(1);
		break;
	default:
		break;
	}
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
