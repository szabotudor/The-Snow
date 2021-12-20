#pragma once
#include<SDL.h>
#include<Vector.h>
#include<iostream>
using namespace std;

namespace ss {
	/*
	class ParticleGravity {
	public:
		enum class GravityType {
			DIRECTION,
			POINT
		};
		GravityType type = GravityType::DIRECTION;
		double force = 9.81;
		Vector direction;
		Vector position;
	};
	
	class Particle {
	public:
		SDL_Color color = SDL_Color();
		SDL_Texture* texture;
		SDL_Rect rect;
		Vector position;
		Vector velocity;
		double angular_velocity = 0.0;
		double angle = 0.0;
		double lifetime = 0.0;
		double entry_time = 0.0;
		double lifelimit = 1.0;
		int ammount = 16;
	};

	class ParticleDamper {
	public:
		double linear_damping = 0.015;
		double angular_damping = 0.015;
	};

	class ParticleEmitter {
		SDL_Window* window;
		SDL_Renderer* render;
		Particle* particles;
		int ammount = 0;
		int filled = 0;
		bool first_cycle = true;
	public:
		enum class EmissionShape {
			POINT
		};
		EmissionShape shape = EmissionShape::POINT;
		Vector position;
		ParticleEmitter(SDL_Window* window, Vector position, int emission_ammount = 16, EmissionShape shape = EmissionShape::POINT);
		void operator << (Particle particle);
		void operator << (ParticleGravity gravity);
		void operator << (ParticleDamper damper);
		//Applies all velocities to the particles and updates their position
		void update(float delta);
		//Draws all particles contained in the emitter to screen
		void draw();
		//Clears all particles in the particle emitter
		void clear();
	};
	*/
	class ParticleEmitter {
		enum class EmissionShape {
			POINT,
			CIRCLE,
			RECT,
			LINE
		};
		enum {
			PTex,
			PLifeTime,
			PLifeLimit,
			PPos,
			PVel,
			PAng,
			PAngVel
		};
		SDL_Window* window;
		SDL_Renderer* render;
		int ammount;
		int** particles;
		SDL_Texture** p_textures;
		double* p_lifetime;
		double* p_lifelimit;
		Vector* p_position;
		Vector* p_velocity;
		double* p_angle;
		double* p_angular_velocity;
	public:
		EmissionShape emitter_shape = EmissionShape::POINT;
		Vector position;
		bool use_gravity = false;
		double g_force = 1;
		Vector g_direction, g_position;
		ParticleEmitter(SDL_Window* window, Vector position, int ammount);
	};
}
