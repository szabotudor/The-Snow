#pragma once
#include<SDL.h>
#include<Particles.h>
#include<Vector.h>


namespace ss {
	void resize(int start_size, int end_size, ss::Vector*& arr);
	void resize(int start_size, int end_size, double*& arr);
	void resize(int start_size, int end_size, SDL_Texture**& arr);
	void resize(int start_size, int end_size, int*& arr);
	void resize(int start_size, int end_size, int**& arr);
	void resize(int start_size, int end_size, ParticleEmitter::ParticleType*& arr);
	void resize(int start_size, int end_size, bool*& arr);
}
