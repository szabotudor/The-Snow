#include<Particles.h>

/*
ss::ParticleEmitter::ParticleEmitter(SDL_Window* window, Vector position, int emission_ammount, EmissionShape shape) {
	ParticleEmitter::window = window;
	ParticleEmitter::render = SDL_GetRenderer(window);
	ParticleEmitter::position = position;
	ParticleEmitter::shape = shape;
	particles = new Particle[emission_ammount];
	ammount = emission_ammount;
}

void ss::ParticleEmitter::operator<<(Particle particle) {
	if (filled + particle.ammount > ammount) {
		throw invalid_argument("Ammount of particles given is higher than the ammount of free particle spots");
	}
	for (int i = filled; i < particle.ammount; i++) {
		particles[i] = particle;
		particles[i].rect;
		int w, h;
		SDL_QueryTexture(particles[i].texture, NULL, NULL, &w, &h);
		particles[i].rect.w = w;
		particles[i].rect.h = h;
		particles[i].entry_time = (double)(i - filled) / particles[i].ammount * particles[i].lifelimit;
		particles[i].lifetime = particles[i].lifelimit - particles[i].entry_time;
		particles[i].position = position;
	}
	filled += particle.ammount;
}

void ss::ParticleEmitter::operator<<(ParticleGravity gravity) {
	switch (gravity.type) {
	case ParticleGravity::GravityType::DIRECTION:
		for (int i = 0; i < filled; i++) {
			particles[i].velocity += gravity.direction * gravity.force;
		}
		break;
	case ParticleGravity::GravityType::POINT:
		for (int i = 0; i < filled; i++) {
			particles[i].velocity += particles[i].position.direction_to(gravity.position) * gravity.force;
		}
		break;
	default:
		break;
	}
}

void ss::ParticleEmitter::operator<<(ParticleDamper damper) {
	for (int i = 0; i < filled; i++) {
		particles[i].velocity -= particles[i].velocity * damper.linear_damping;
		particles[i].angular_velocity -= particles[i].angular_velocity * damper.angular_damping;
	}
}

void ss::ParticleEmitter::update(float delta) {
	for (int i = 0; i < filled; i++) {
		particles[i].lifetime += delta / 1000;
		if (particles[i].lifetime > particles[i].lifelimit) {
			particles[i].position = position;
			particles[i].velocity = Vector();
			particles[i].angular_velocity = 0.0f;
			particles[i].angle = 0.0f;
			particles[i].lifetime = 0.0f;
			first_cycle = false;
		}
		if (!first_cycle or (first_cycle and particles[i].lifetime > particles[i].entry_time)) {
			particles[i].position += particles[i].velocity * delta;
			particles[i].angle += particles[i].angular_velocity * delta;
			particles[i].rect.x = particles[i].position.x;
			particles[i].rect.y = particles[i].position.y;
		}
	}
}

void ss::ParticleEmitter::draw() {
	for (int i = 0; i < filled; i++) {
		if (!first_cycle or (first_cycle and particles[i].lifetime > particles[i].entry_time)) {
			SDL_Rect rect = particles[i].rect;
			SDL_RenderCopyEx(render, particles[i].texture, NULL, &rect, particles[i].angle, NULL, SDL_FLIP_NONE);
		}
	}
}

void ss::ParticleEmitter::clear() {
	for (int i = 0; i < filled; i++) {
		SDL_DestroyTexture(particles[i].texture);
	}
	filled = 0;
	delete[] particles;
}
*/

ss::ParticleEmitter::ParticleEmitter(SDL_Window* window, Vector position, int ammount) {
	ParticleEmitter::window = window;
	render = SDL_GetRenderer(window);
	ParticleEmitter::position = position;
}
