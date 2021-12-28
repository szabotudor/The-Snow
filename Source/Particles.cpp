#include<Particles.h>
#include<Utility.h>


ss::ParticleEmitter::ParticleEmitter(SDL_Window* window, Vector position, bool sort_by_lifetie) {
	ParticleEmitter::sort_by_lifetime = sort_by_lifetie;
	ParticleEmitter::window = window;
	render = SDL_GetRenderer(window);
	ParticleEmitter::position = position;
}

void ss::ParticleEmitter::add_particle_layer(int ammount, SDL_Texture* texture, double lifelimit) {
	resize(layer, layer + 1, particle_layer);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_lifetime);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_angle);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_angular_velocity);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_position);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_velocity);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_layer);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_drawn);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_order);
	particle_layer[layer].texture = texture;
	particle_layer[layer].lifelimit = lifelimit;

	for (int i = ParticleEmitter::ammount; i < ParticleEmitter::ammount + ammount; i++) {
		p_lifetime[i] = -lifelimit + lifelimit / ammount * (i + 1);
		p_angle[i] = 0;
		p_angular_velocity[i] = 0;
		p_position[i] = position;
		p_velocity[i] = 0;
		p_layer[i] = layer;
		p_drawn[i] = true;
		p_order[i] = i;
	}

	ParticleEmitter::ammount += ammount;
	layer++;
}

void ss::ParticleEmitter::add_seccondary_emitter(ParticleEmitter* emitter) {
	sec_emitter = emitter;
	use_sec_emitter = true;
}

void ss::ParticleEmitter::remove_seccondary_emitter() {
	sec_emitter = nullptr;
	use_sec_emitter = false;
}

void ss::ParticleEmitter::update(double delta) {
	delta /= 1000;
	for (int i = 0; i < ammount; i++) {
		p_lifetime[i] += delta;
		p_order[i] = i;

		if (p_lifetime[i] >= 0) {
			int ly = p_layer[i];

			if (p_lifetime[i] > particle_layer[ly].lifelimit - particle_layer[ly].lifetime_random) {
				double randl = rng.randd(particle_layer[ly].lifetime_random);
				if (p_lifetime[i] > particle_layer[ly].lifelimit - randl) {
					p_lifetime[i] -= particle_layer[ly].lifelimit - randl;
					p_position[i] = position;
					p_velocity[i] = (particle_layer[ly].initial_direction * (1 - particle_layer[ly].initial_direction_randomness) +
						rng.randdir() * particle_layer[ly].initial_direction_randomness) *
						rng.randd_range(particle_layer[ly].initial_velocity_min, particle_layer[ly].initial_velocity_max) +
						particle_layer[ly].initial_velocity;
					p_angular_velocity[i] = rng.randd_range(particle_layer[ly].initial_angular_velocity_min, particle_layer[ly].initial_angular_velocity_max);
					p_angle[i] = 0;
				}
			}
			if (particle_layer[ly].use_gravity) {
				switch (particle_layer[ly].g_type) {
				case ss::ParticleEmitter::GravityType::DIRECTION:
					p_velocity[i] += particle_layer[ly].g_direction * particle_layer[ly].g_force * delta;
					break;
				case ss::ParticleEmitter::GravityType::POINT:
					p_velocity[i] += p_position[i].direction_to(particle_layer[ly].g_position) * particle_layer[ly].g_force * delta;
					break;
				default:
					break;
				}
			}
			p_velocity[i] -= p_velocity[i] * particle_layer[ly].velocity_damping * delta;
			p_angular_velocity[i] -= p_angular_velocity[i] * particle_layer[ly].angular_velocity_damping * delta;
			if (p_lifetime[i] > delta) {
				p_position[i] += p_velocity[i] * delta;
				p_angle[i] += p_angular_velocity[i] * delta;
			}
			else {
				p_position[i] += p_velocity[i] * p_lifetime[i];
				p_angle[i] += p_angular_velocity[i] * p_lifetime[i];
			}
		}
		else {
			p_position[i] = position;
		}
	}

	if (sort_by_lifetime) {
		for (int i = 1; i < ammount; i++) {
			for (int j = i; j > 0 and p_lifetime[p_order[j]] < p_lifetime[p_order[j - 1]]; j--) {
				int aux = p_order[j];
				p_order[j] = p_order[j - 1];
				p_order[j - 1] = aux;
			}
		}
	}

	if (use_sec_emitter) {
		sec_emitter->update(delta * 1000);
	}
}

void ss::ParticleEmitter::draw() {
	SDL_Rect rect;
	SDL_Color color;
	int prev_ly = -1;
	int i, i_end;
	if (reverse_draw_order) {
		i = ammount - 1;
		i_end = -1;
	}
	else {
		i = 0;
		i_end = ammount;
	}
	while (i != i_end) {
		int j = p_order[i];
		if (p_lifetime[j] >= 0) {
			if (p_position[j].distance_to(p_position[j + 1]) > 0.5) {
				int ly = p_layer[j];
				if (ly != prev_ly) {
					SDL_QueryTexture(particle_layer[ly].texture, NULL, NULL, &rect.w, &rect.h);
					prev_ly = ly;
				}
				rect.x = p_position[j].x;
				rect.y = p_position[j].y;
				if (particle_layer[ly].get_colors_in_gradient() > 0) {
					color = particle_layer[ly].get_color_at_timestamp(p_lifetime[j]);
					SDL_SetTextureColorMod(particle_layer[ly].texture, color.r, color.g, color.b);
				}
				SDL_RenderCopyEx(render, particle_layer[ly].texture, NULL, &rect, p_angle[j], NULL, SDL_FLIP_NONE);
			}
		}
		if (reverse_draw_order) {
			i--;
		}
		else {
			i++;
		}
	}
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	if (use_sec_emitter) {
		sec_emitter->draw();
	}
}

int ss::ParticleEmitter::get_num_of_layers() {
	return layer;
}

SDL_Color ss::ParticleEmitter::ParticleType::get_color_at_timestamp(double time) {
	float r = 255, g = 255, b = 255;
	SDL_Color color{};
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	for (int i = 0; i < colors - 1; i++) {
		if (time > gradient_times[i]) {
			time -= gradient_times[i];
			double dt = time / (gradient_times[i + 1] - gradient_times[i]);
			r = lerp(gradient[i].r, gradient[i + 1].r, dt);
			g = lerp(gradient[i].g, gradient[i + 1].g, dt);
			b = lerp(gradient[i].b, gradient[i + 1].b, dt);
			
			color.r = clamp(0, 255, r);
			color.g = clamp(0, 255, g);
			color.b = clamp(0, 255, b);

			return color;
		}
	}
	return color;
}

int ss::ParticleEmitter::ParticleType::get_colors_in_gradient() {
	return colors;
}

void ss::ParticleEmitter::ParticleType::add_color_to_gradient(SDL_Color color, double timestamp) {
	resize(colors, colors + 1, gradient);
	resize(colors, colors + 1, gradient_times);

	gradient[colors] = color;
	gradient_times[colors] = timestamp;

	colors++;
}

void ss::ParticleEmitter::ParticleType::add_color_to_gradient(int r, int g, int b, double timestamp) {
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	add_color_to_gradient(color, timestamp);
}
