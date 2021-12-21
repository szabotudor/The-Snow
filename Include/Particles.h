#pragma once
#include<SDL.h>
#include<Vector.h>
#include<iostream>
using namespace std;

namespace ss {
	class ParticleEmitter {
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
		int ammount = 0;
		int types = 0;

		int* p_arg;
		double* p_lifelimit;
		SDL_Texture** p_textures;

		double* p_lifetime;
		Vector* p_position;
		Vector* p_velocity;
		double* p_angle;
		double* p_angular_velocity;
	public:
		enum class EmissionShape {
			POINT,
			CIRCLE,
			RECT,
			LINE
		};
		enum class GravityType {
			DIRECTION,
			POINT
		};
		EmissionShape emitter_shape = EmissionShape::POINT;
		Vector position;
		bool use_gravity = false;

		GravityType g_type = GravityType::DIRECTION;
		double g_force = 1;
		Vector g_direction = Vector(0, 1), g_position = Vector(0);

		ParticleEmitter(SDL_Window* window, Vector position);
		//Adds a specified ammount of the given particle
		void add_particles(int ammount, SDL_Texture* texture, double lifelimit);
		//Updates position of al particles
		void update();
		//Draws all particles in the emitter on the screen
		void draw();
	};
}
