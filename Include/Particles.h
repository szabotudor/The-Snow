#pragma once
#include<SDL.h>
#include<Vector.h>
#include<iostream>
using namespace std;

namespace ss {
	class ParticleEmitter {
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
		class ParticleType {
			GravityType g_type = GravityType::DIRECTION;
			double g_force = 1;
			Vector g_direction = Vector(0, 1), g_position = Vector();
		};
	private:
		ParticleEmitter* sec_emitter;
		bool use_sec_emitter = false;
		SDL_Window* window;
		SDL_Renderer* render;
		int ammount = 0;
		int layer = 0;

		double p_lifelimit;
		SDL_Texture* p_texture;
		Vector init_position;

		double* p_lifetime;
		Vector* p_position;
		Vector* p_velocity;
		double* p_angle;
		double* p_angular_velocity;
	public:
		EmissionShape emitter_shape = EmissionShape::POINT;
		Vector position;
		bool use_gravity = false;

		GravityType g_type = GravityType::DIRECTION;
		double g_force = 1;
		Vector g_direction = Vector(0, 1), g_position = Vector(0);

		ParticleEmitter(SDL_Window* window, Vector position);
		//Adds a specified ammount of the given particle into the next free layer
		void add_particle_layer(int ammount, SDL_Texture* texture, double lifelimit);
		//Adds a seccondary emitter (actiong as a seccond layer of particles)
		void add_seccondary_emitter(ParticleEmitter* emitter);
		//Updates position of al particles
		void update(double delta);
		//Draws all particles in the emitter on the screen
		void draw();
	};
}
