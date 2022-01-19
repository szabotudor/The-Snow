#include"Enemy.h"


bool init = false;
SDL_Window* window = nullptr;
ss::Sprite enemy_sprite;
ss::Sprite melt_sprite;

void init_enemy(ss::Snow& game) {
	init = true;
	const char* textures[9] = {
		"Sprites/Snowman/snowman_body0000.png",
		"Sprites/Snowman/snowman_body0001.png",
		"Sprites/Snowman/snowman_body0002.png",

		"Sprites/Snowman/snowman_middle0000.png",
		"Sprites/Snowman/snowman_middle0001.png",
		"Sprites/Snowman/snowman_middle0002.png",

		"Sprites/Snowman/snowman_head0000.png",
		"Sprites/Snowman/snowman_head0001.png",
		"Sprites/Snowman/snowman_head0002.png"
	};
	const char* melt_textures[5] = {
		"Sprites/Snowman/melt0000.png",
		"Sprites/Snowman/melt0001.png",
		"Sprites/Snowman/melt0002.png",
		"Sprites/Snowman/melt0003.png",
		"Sprites/Snowman/melt0004.png"
	};
	enemy_sprite = ss::Sprite(game.get_window(), 9, textures);
	melt_sprite = ss::Sprite(game.get_window(), 5, melt_textures);
}


Enemy::Enemy(ss::Vector position) {
	Enemy::position = position;
	collision = ss::CollisionShape(ss::Vector(17, 33), position);
	rng.randomize();
	rot_speed = rng.randf_range(0.03, 0.06);
}

void Enemy::process(double delta) {
	delta /= 1000;
	lifetime += delta;
	vert_velocity -= delta * 1500;
	if (invulnerability > 0 and life < 3 and height >= 0) {
		height += vert_velocity * delta;
		if (height < 0) {
			height = 0;
		}
	}
	/*
	anim_time += delta;
	switch (animation) {
	case Enemy::Anim::IDLE:
		if (anim_time > 0.8) {
			anim_time = 0;
			if (draw_offset == ss::Vector(0, -1)) {
				draw_offset = ss::Vector(0, 1);
			}
			else {
				draw_offset = ss::Vector(0, -1);
			}
		}
		break;
	case Enemy::Anim::SHOOT:
		break;
	case Enemy::Anim::WALK:
		break;
	default:
		break;
	}
	*/

	visibility_timer += delta;
	if (visibility_timer > 0.05) {
		visible = !visible;
		visibility_timer = 0;
	}

	if (invulnerability > 0 and height > 0) {
		if (lifetime > rot_speed) {
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
	}
	position = collision.position + col_draw_offset - ss::Vector(0, height);
	if (invulnerability > 0) {
		invulnerability -= delta;
#if defined _DEBUG
		collision.draw_color.r = 255;
		collision.draw_color.b = 120;
	}
	else {
		collision.draw_color.r = 120;
		collision.draw_color.b = 255;
#endif
	}
}

void Enemy::damage() {
	if (invulnerability <= 0) {
		life--;
		invulnerability = 1;
		vert_velocity = 300;
		switch (life) {
		case 2:
			col_draw_offset -= ss::Vector(0, 9);
			collision.size -= ss::Vector(2, 11);
			collision.position.y += 11;
			break;
		case 1:
			col_draw_offset -= ss::Vector(0, 8);
			collision.size -= ss::Vector(2, 10);
			collision.position.y += 10;
			break;
		default:
			break;
		}
	}
}

void Enemy::draw(ss::Vector camera_offset) {
	enemy_sprite.position = position - camera_offset + ss::Vector(0, 13) - ss::Vector(1, 4);

	if (invulnerability > 0) {
		melt_sprite.frame = (int)((1 - invulnerability) * 4);
		switch (life) {
		case 2:
			melt_sprite.position = collision.position + ss::Vector(-2, 6) - camera_offset;
			melt_sprite.draw(1);
			break;
		case 1:
			melt_sprite.position = collision.position + ss::Vector(-3, -7) - camera_offset;
			melt_sprite.draw(1);
			break;
		case 0:
			melt_sprite.position = collision.position + ss::Vector(-4, -7) - camera_offset;
			melt_sprite.draw(1);
			break;
		default:
			break;
		}
	}
	else {
		visible = true;
	}

	if (visible) {
		ss::Vector ofs(1, -9);
		switch (look_dir) {
		case Enemy::LookDirection::UP:
			enemy_sprite.flip = SDL_FLIP_NONE;
			if (life > 2) {
				enemy_sprite.frame = 2;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + draw_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 5;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset;
			}

			if (life > 0) {
				enemy_sprite.frame = 8;
				enemy_sprite.draw(1);
			}
			break;
		case Enemy::LookDirection::DOWN:
			enemy_sprite.flip = SDL_FLIP_NONE;
			if (life > 2) {
				enemy_sprite.frame = 1;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + draw_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 4;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset;
			}

			if (life > 0) {
				enemy_sprite.frame = 7;
				enemy_sprite.draw(1);
			}
			break;
		case Enemy::LookDirection::LEFT:
			enemy_sprite.flip = SDL_FLIP_HORIZONTAL;
			if (life > 2) {
				enemy_sprite.frame = 0;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + draw_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 3;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset;
			}

			if (life > 0) {
				enemy_sprite.frame = 6;
				enemy_sprite.draw(1);
			}
			break;
		case Enemy::LookDirection::RIGHT:
			enemy_sprite.flip = SDL_FLIP_NONE;
			if (life > 2) {
				enemy_sprite.frame = 0;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + draw_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 3;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset;
			}

			if (life > 0) {
				enemy_sprite.frame = 6;
				enemy_sprite.draw(1);
			}
			break;
		default:
			break;
		}
	}
#if defined _DEBUG
	collision.position -= camera_offset;
	collision.draw();
	collision.position += camera_offset;
#endif
}

bool Enemy::is_dead() {
	return life <= 0 and invulnerability <= 0;
}

int Enemy::get_hp() {
	return life;
}

bool Enemy::is_invulnerable() {
	return invulnerability >= 0;
}
