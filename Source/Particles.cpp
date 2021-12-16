#include<Particles.h>

ss::ParticleEmitter::ParticleEmitter(Vector position, int emission_ammount, EmissionShape shape) {
	ParticleEmitter::position = position;
	ParticleEmitter::shape = shape;
	particles = new Particle[emission_ammount];
	ammount = emission_ammount;
}

void ss::ParticleEmitter::operator<<(Particle particle) {
	if (filled + particle.ammount > ammount) {
		throw invalid_argument("Ammount of particles given is higher than the ammount of free particle spots");
	}
	for (filled; filled < particle.ammount; filled++) {
		particles[filled] = particle;
		particles[filled].rect;
	}
}

void ss::ParticleEmitter::update(float delta) {
	for (int i = 0; i < filled; i++) {
		particles[i].position += particles[i].velocity * delta;
		if (particles[i].position.x != particles[i].rect.x or particles[i].position.y != particles[i].rect.y) {
			particles[i].rect.x = particles[i].position.x;
			particles[i].rect.y = particles[i].position.y;
		}
		particles[i].angle += particles[i].angular_velocity * delta;
	}
}

void ss::ParticleEmitter::draw() {
	for (int i = 0; i < filled; i++) {
		SDL_RenderCopyEx(render, particles[i].texture, NULL, &particles[i].rect, particles[i].angle, NULL, SDL_FLIP_NONE);
	}
}

void ss::ParticleEmitter::clear() {
	for (int i = 0; i < filled; i++) {
		SDL_DestroyTexture(particles[i].texture);
	}
	filled = 0;
	delete[] particles;
}
