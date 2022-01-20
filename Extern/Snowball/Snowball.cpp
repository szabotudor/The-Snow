#include "Snowball.h"

Snowball::Snowball(SDL_Window* window, ss::Vector position, ss::Vector direction, double scale) {
	Snowball::position = position;
	Snowball::direction = direction;
	Snowball::scale = scale;
	render = SDL_GetRenderer(window);
	rng.randomize();

	tex_big = new ss::Texture(window, "Sprites/Snowball/Snowball.png");
	
	SDL_Surface* pt_sfc = SDL_CreateRGBSurface(NULL, 2, 2, 32, NULL, NULL, NULL, NULL);
	SDL_Texture* pt_tex = SDL_CreateTextureFromSurface(render, pt_sfc);

	ptem = new ss::ParticleEmitter(window, position);
	ptem->add_particle_layer(16, pt_tex, 1);
}

void Snowball::update() {
}

void Snowball::draw() {
	tex_big->position = position;
	tex_big->draw();
}
