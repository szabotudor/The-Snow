#pragma once
#include<SDL.h>
#include<Vector.h>
#include<iostream>
using namespace std;

namespace ss {
	class ParticleGravity {
	public:
		enum class Type {
			DIRECTION,
			POINT,
			LINE
		};
		float force = 9.81;
	private:
		Vector direction;
		Vector position;
	};
	class Particle {
	public:
		SDL_Color color;
		SDL_Texture* texture;
		Vector position;
		Vector velocity;
		float angular_velocity = 0.0f;
		float angle = 0.0f;
		float lifetime = 0.0f;
		float lifelimit = 1.0f;
		int ammount = 16;
	};
	class ParticleEmitter {
		SDL_Window* window;
		SDL_Renderer* render;
		Particle* particles;
		int ammount = 0;
		int filled = 0;
	public:
		Vector position;
		ParticleEmitter(Vector position, int emission_ammount = 16);
		void operator << (Particle particle);
	};
}
