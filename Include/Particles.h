#pragma once
#include<SDL.h>
#include<RandomNumberGenerator.h>
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
		public:
			SDL_Texture* texture;
			double lifelimit = 1.0;
			double velocity_damping = 0.0125;
			double angular_velocity_damping = 0.0125;

			GravityType g_type = GravityType::DIRECTION;
			double g_force = 1;
			Vector g_direction = Vector(0, 1);
			Vector g_position = Vector();
			bool use_gravity = false;

			Vector initial_velocity_min = Vector(0, 0);
			Vector initial_velocity_max = Vector(0, 0);

			double initial_angular_velocity_min = 0.0;
			double initial_angular_velocity_max = 0.0;
		};
	private:
		RandomNumberGenerator rng = RandomNumberGenerator(98132479);
		ParticleEmitter* sec_emitter;
		bool use_sec_emitter = false;
		SDL_Window* window;
		SDL_Renderer* render;
		int ammount = 0;
		int layer = 0;

		double* p_lifetime;
		Vector* p_position;
		Vector* p_velocity;
		double* p_angle;
		double* p_angular_velocity;
		int* p_layer;
		int* p_order;
		bool* p_drawn;
	public:
		bool sort_by_lifetime = false;
		ParticleType* particle_layer;
		EmissionShape emitter_shape = EmissionShape::POINT;
		Vector position;

		ParticleEmitter(SDL_Window* window, Vector position, bool sort_by_lifetie = false);
		//Adds a specified ammount of the given particle into the next free layer
		void add_particle_layer(int ammount, SDL_Texture* texture, double lifelimit);
		//Adds a seccondary emitter (actiong as a seccond layer of particles)
		void add_seccondary_emitter(ParticleEmitter* emitter);
		//Turns off and removes the seccondary emitter (if it exists)
		void remove_seccondary_emitter();
		//Updates position of al particles
		void update(double delta);
		//Draws all particles in the emitter on the screen
		void draw();
		//Returns the number of particle layers in the emitter
		int get_num_of_layers();
	};
}
