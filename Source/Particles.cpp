#include<Particles.h>
#include<Utility.h>


ss::ParticleEmitter::ParticleEmitter(SDL_Window* window, Vector position) {
	ParticleEmitter::window = window;
	render = SDL_GetRenderer(window);
	ParticleEmitter::position = position;
}

void ss::ParticleEmitter::add_particle_layer(int ammount, SDL_Texture* texture, double lifelimit) {
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_lifetime);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_angle);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_angular_velocity);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_position);
	resize(ParticleEmitter::ammount, ParticleEmitter::ammount + ammount, p_velocity);
	p_texture = texture;
	p_lifelimit = lifelimit;

	for (int i = ParticleEmitter::ammount; i < ParticleEmitter::ammount + ammount; i++) {
		p_lifetime[i] = -lifelimit + lifelimit / ammount * (i + 1);
		p_angle[i] = 0;
		p_angular_velocity[i] = 0;
		p_position[i] = position;
		p_velocity[i] = 0;
	}

	ParticleEmitter::ammount += ammount;
}

void ss::ParticleEmitter::add_seccondary_emitter(ParticleEmitter* emitter) {
	sec_emitter = emitter;
	use_sec_emitter = true;
}

void ss::ParticleEmitter::update(double delta) {
	delta /= 1000;
	for (int i = 0; i < ammount; i++) {
		p_lifetime[i] += delta;
		if (p_lifetime[i] >= 0) {
			if (p_lifetime[i] > p_lifelimit) {
				p_lifetime[i] -= p_lifelimit;
				p_position[i] = position;
				p_velocity[i] = Vector();
				p_angle[i] = 0;
				p_angular_velocity[i] = 0;
			}
			if (use_gravity) {
				switch (g_type) {
				case ss::ParticleEmitter::GravityType::DIRECTION:
					p_velocity[i] += g_direction * g_force;
					break;
				case ss::ParticleEmitter::GravityType::POINT:
					p_velocity[i] += p_position[i].direction_to(g_position) * g_force;
					break;
				default:
					break;
				}
			}
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
	if (use_sec_emitter) {
		sec_emitter->update(delta * 1000);
	}
}

void ss::ParticleEmitter::draw() {
	SDL_Rect rect;
	for (int i = 0; i < ammount; i++) {
		if (p_lifetime[i] >= 0) {
			rect.x = p_position[i].x;
			rect.y = p_position[i].y;
			SDL_QueryTexture(p_texture, NULL, NULL, &rect.w, &rect.h);
			SDL_RenderCopyEx(render, p_texture, NULL, &rect, p_angle[i], NULL, SDL_FLIP_NONE);
		}
	}
	if (use_sec_emitter) {
		sec_emitter->draw();
	}
}
