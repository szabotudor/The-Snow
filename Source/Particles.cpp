#include<Particles.h>

ss::ParticleEmitter::ParticleEmitter(Vector position, int emission_ammount) {
	ParticleEmitter::position = position;
	particles = new Particle[emission_ammount];
	ammount = emission_ammount;
}

void ss::ParticleEmitter::operator<<(Particle particle) {
	if (filled + particle.ammount > ammount) {
		throw invalid_argument("Ammount of particles given is higher than the ammount of free particle spots");
	}
	for (filled; filled < particle.ammount; filled++) {
		particles[filled] = particle;
	}
}
