#include"Enemy.h"


bool init = false;
SDL_Window* window = nullptr;
ss::Sprite enemy_sprite;
ss::Sprite melt_sprite;

void init_enemy(ss::Snow& game) {
	//Load all textures
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
	rot_speed = rng.randf_range(0.03f, 0.06f);
}

void Enemy::process(double delta, int& num_of_snowballs, ss::Snow& game, Snowball**& snowball_list) {
	delta /= 1000;
	lifetime += delta;

	//Vertical velocity
	vert_velocity -= delta * 1500;

	//Change the height according to the velocity
	if (invulnerability > 0 and life < 3 and height >= 0) {
		height += vert_velocity * delta;
		if (height < 0) {
			height = 0;
		}
	}
	
	//Animate the enemy
	if (spawn_timer >= 1) {
		anim_time += delta;
		switch (animation) {
		case Enemy::Anim::IDLE:
			ofs = ss::lerp(ofs, ss::Vector(1, -9), delta * 7);
			break;
		case Enemy::Anim::SHOOT:
			break;
		case Enemy::Anim::WALK:
			break;
		default:
			break;
		}
	}
	

	//Flicker effect when the enemy is invulnerable
	visibility_timer += delta;
	if (visibility_timer > 0.05) {
		visible = !visible;
		visibility_timer = 0;
	}

	//Rotate the enemy in the air when taking damage
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

	//Set the position of the enemy according to the draw offset
	position = collision.position + col_draw_offset - ss::Vector(0, height);

	//Advance the spawn timer, which is ued to tell wether the spawn animation is done playing
	if (life == 3 and spawn_timer < 1) {
		spawn_timer += delta;
	}
	else {
		spawn_timer = 2;
	}

	//Increase the area of effect
	if (aoe < 13) {
		aoe = ss::lerp(aoe, 13, delta);
	}

	//Throw snowballs
	if (num_of_snowballs < 512) {
		ss::Vector target_p;
		if (position.distance_to(target) < 110) {
			target_p = position.direction_to(target + ss::Vector(0, 5));
		}
		else {
			target_p = position.direction_to(target);
		}
		if (snowball_timer <= 0) {
			snowball_list[num_of_snowballs] = new Snowball(game.get_window(), position + ss::Vector(5, 0), target_p);
			num_of_snowballs++;
			snowball_timer = rng.randf_range(1.5, 3);
		}
		else {
			snowball_timer -= delta;
		}
	}

	if (spawn_timer >= 1) {
		if (invulnerability <= 0) {
			//Move towards player
			ss::Vector direction = collision.position.direction_to(target).normalized();
			if (collision.position.distance_to(target) > 100) {
				velocity = ss::lerp(velocity, direction * delta * 45, delta * 5);
			}
			else if (collision.position.distance_to(target) < 86) {
				velocity = ss::lerp(velocity, ss::Vector() - direction * delta * 45, delta * 5);
			}
			else {
				velocity = ss::lerp(velocity, 0, delta * 5);
			}

			//Look towards player
			if (ss::natural(direction.x) > ss::natural(direction.y)) {
				if (direction.x > 0) {
					look_dir = LookDirection::RIGHT;
				}
				else if (direction.x < 0) {
					look_dir = LookDirection::LEFT;
				}
			}
			else {
				if (direction.y > 0) {
					look_dir = LookDirection::DOWN;
				}
				else if (direction.y < 0) {
					look_dir = LookDirection::UP;
				}
			}
		}
		else {
			velocity = ss::lerp(velocity, 0, delta * 5);
		}
	}

	collision.position += velocity;

	//Advance the invulnerability timer
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
		aoe = 0;

		//This is for the jumping into the air animation
		vert_velocity = 300;

		//Update sprite offsets and collision size when parts are knocked off the enemy
		switch (life) {
		case 2:
			col_draw_offset -= ss::Vector(0, 9);
			collision.size -= ss::Vector(2, 11);
			collision.position.y += 11;
			aoe_offset = ss::Vector(-0.8, 7);
			break;
		case 1:
			col_draw_offset -= ss::Vector(0, 8);
			collision.size -= ss::Vector(2, 10);
			collision.position.y += 10;
			aoe_offset = ss::Vector(-1, -1);
			break;
		case 0:
			aoe_offset = ss::Vector(-2.5, 2);
			break;
		default:
			break;
		}
	}
}

void Enemy::draw(ss::Vector camera_offset) {
	enemy_sprite.position = position - camera_offset + ss::Vector(0, 13) - ss::Vector(1, 4);

	//Draw the melt sprite and animate it when the enemy takes damage
	if (invulnerability > 0) {
		switch (life) {
		case 3:
			melt_sprite.frame = (int)(invulnerability * 4);
			melt_sprite.position = collision.position + ss::Vector(-1, 14) - camera_offset;
			melt_sprite.draw(1);
			visible = false;
			break;
		case 2:
			melt_sprite.frame = (int)((1 - invulnerability) * 4);
			melt_sprite.position = collision.position + ss::Vector(-2, 6) - camera_offset;
			melt_sprite.draw(1);
			break;
		case 1:
			melt_sprite.frame = (int)((1 - invulnerability) * 4);
			melt_sprite.position = collision.position + ss::Vector(-3, -7) - camera_offset;
			melt_sprite.draw(1);
			break;
		case 0:
			melt_sprite.frame = (int)((1 - invulnerability) * 4);
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

	//Draw the enemy
	if (visible) {
		switch (look_dir) {
		case Enemy::LookDirection::UP:
			enemy_sprite.flip = SDL_FLIP_NONE;
			if (life > 2) {
				enemy_sprite.frame = 2;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + draw_offset + middle_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 5;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset + head_offset;
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
				enemy_sprite.position += ofs + draw_offset + middle_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 4;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset + head_offset;
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
				enemy_sprite.position += ofs + draw_offset + middle_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 3;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset + head_offset;
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
				enemy_sprite.position += ofs + draw_offset + middle_offset;
			}

			if (life > 1) {
				enemy_sprite.frame = 3;
				enemy_sprite.draw(1);
				enemy_sprite.position += ofs + ss::Vector(0, 1) + draw_offset + head_offset;
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
