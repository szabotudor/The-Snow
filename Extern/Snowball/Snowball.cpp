#include"../Setup.h"
#include"SDL_mixer.h"
#include"Snowball.h"


ss::Texture* snowball_tex;
ss::Texture* snowball_shadow;
SDL_Texture* snowball_p_tex;
Mix_Chunk* snd_snowball = NULL;


void init_snowballs(ss::Snow& game) {
	snd_snowball = Mix_LoadWAV("Sounds/Effects/snowball.wav");
	Mix_VolumeChunk(snd_snowball, 8);
	snowball_tex = new ss::Texture(game.get_window(), "Sprites/Snowball/Snowball.png", SDL_BLENDMODE_BLEND);
	snowball_shadow = new ss::Texture(game.get_window(), "Sprites/Snowball/Snowball_Shadow.png", SDL_BLENDMODE_BLEND);
	SDL_Surface* pt_sfc = SDL_CreateRGBSurface(NULL, 2, 2, 32, NULL, NULL, NULL, NULL);
	SDL_FillRect(pt_sfc, NULL, SDL_MapRGB(pt_sfc->format, 255, 255, 255));
	snowball_p_tex = SDL_CreateTextureFromSurface(game.get_renderer(), pt_sfc);
	SDL_FreeSurface(pt_sfc);
}


Snowball::Snowball(SDL_Window* window, ss::Vector position, ss::Vector direction, double scale) {
	Snowball::position = position;
	Snowball::direction = direction;
	Snowball::scale = scale;
	render = SDL_GetRenderer(window);
	rng.randomize();

	ptem = new ss::ParticleEmitter(window, position);
	ptem->one_time = true;
	ptem->add_particle_layer(8, snowball_p_tex, 0.8, 0.9);
	ptem->particle_layer[0].velocity_damping = 2;
	ptem->particle_layer[0].initial_direction_randomness = 1;
	ptem->particle_layer[0].initial_velocity_min = 40;
	ptem->particle_layer[0].initial_velocity_max = 50;
	ptem->emission_shape = ss::ParticleEmitter::EmissionShape::CIRCLE;
	ptem->emission_radius = 2;

	collision = ss::CollisionShape(6, position, window);
}

void Snowball::update(double delta, ss::Texture& ground_texture, bool**& ground_bool, long long& snow_ammount, ss::CollisionShape& player_cs, ss::Vector& ground_size, bool& hit_player) {
	if (height < 0) {
		if (first_play) {
			Mix_PlayChannel(CH_SNOWBALL, snd_snowball, 0);
			first_play = false;
		}
		visible = false;
		ptem->position = position + ss::Vector(2);
		ptem->update(delta * 10);

		//Leave snow on the ground for each particle
		melted = true;
		for (int i = 0; i < ptem->get_num_of_particles(); i++) {
			if (ptem->get_particle_lifetime(i) < ptem->particle_layer[0].lifelimit) {
				melted = false;
				ss::Vector p_pos = ptem->get_particle_position(i);
				for (int x = p_pos.x - 3; x < p_pos.x + 3; x++) {
					for (int y = p_pos.y - 3; y < p_pos.y + 3; y++) {
						if (x >= 0 and x < ground_size.x and y >= 0 and y < ground_size.y) {
							if (!ground_bool[x][y]) {
								if (p_pos.distance_to(ss::Vector(x, y)) < 3) {
									int r = rng.randi_range(235, 255);
									int g = rng.randi_range(ss::clamp(235, 255, r + 10), 255);
									ground_texture.set_pixel(ss::Vector(x, y), r, g, 255);
									ground_bool[x][y] = true;
									snow_ammount++;
								}
							}
						}
					}
				}
			}
		}
	}
	else {
		delta /= 1000;
		height -= v_velocity * delta * 5;
		v_velocity += delta * 24;
		position += direction * delta * 200;
		collision.position = position - ss::Vector(0, height);
		if (collision.is_colliding_with(player_cs)) {
			position.y -= height;
			height = 0;
			v_velocity = 0;
			hit_player = true;
		}
	}
}

void Snowball::draw(ss::Vector camera_offset) {
	if (visible) {
		snowball_tex->position = position - camera_offset - ss::Vector(0, height);
		snowball_shadow->position = position - camera_offset + ss::Vector(1, 1);

		snowball_shadow->draw();
		snowball_tex->draw();

		if (height < 0) {
			ptem->position = position - camera_offset;
			ptem->draw();
		}
	}
}

ss::Vector Snowball::get_direction() {
	return direction;
}

bool Snowball::is_melted() {
	return melted;
}

void Snowball::free() {
	ptem->free();
}
