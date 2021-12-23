#include<Snow.h>


int avg_fps[60];


void show_fps(ss::Text& text, unsigned int fps, int &i, float delta) {
	if (i < 60) {
		avg_fps[i] = fps;
		i++;
	}
	else {
		i = 0;
		int sum = 0;
		for (int j = 0; j < 60; j++) {
			sum += avg_fps[j];
		}
		sum /= 60;

		text.set_text(to_string(sum) + " " + to_string(delta));
	}
	text.draw();
}


void player_move(ss::Sprite& player, ss::Snow &game, float delta) {
	ss::Vector velocity;
	velocity.y = game.is_key_pressed(SDL_SCANCODE_DOWN) - game.is_key_pressed(SDL_SCANCODE_UP);
	velocity.x = game.is_key_pressed(SDL_SCANCODE_RIGHT) - game.is_key_pressed(SDL_SCANCODE_LEFT);
	velocity.normalize();
	velocity *= delta / 10;
	player.position = player.position + velocity;

}


int main(int argc, char* args[]) {
	ss::Snow game("The Snow", ss::Vector(256, 144), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 144);
	SDL_Renderer* render = SDL_GetRenderer(game.get_window());
	ss::Text fps(game.get_window(), "", "basic.ttf", 9);
	fps.position = ss::Vector(5, 5);
	SDL_Color text_color, border_color, fill_color;

	text_color.r = 255; text_color.g = 255; text_color.b = 255; text_color.a = 255;
	border_color.r = 120; border_color.g = 140; border_color.b = 160; border_color.a = 255;
	fill_color.r = 60; fill_color.g = 80; fill_color.b = 100; fill_color.a = 255;

	float _dt = 0.0f;
	float _rdt = 0.0f;
	int i = 0;

	const char* frames[6] = {
		"Sprites/Player/player_idle0000.png",
		"Sprites/Player/player_idle0001.png",
		"Sprites/Player/player_idle0002.png",
		"Sprites/Player/player_idle0003.png",
		"Sprites/Player/player_idle0004.png",
		"Sprites/Player/player_idle0005.png",
	};
	
	ss::Sprite player = ss::Sprite(game.get_window(), 6, frames);
	player.play(0, 5, 8);

	ss::ParticleEmitter ptem(game.get_window(), ss::Vector(50));
	SDL_Surface* ball = IMG_Load("Sprites/ball.png");
	SDL_Texture* ballt = SDL_CreateTextureFromSurface(render, ball);
	SDL_FreeSurface(ball);
	ptem.add_particle_layer(24000, ballt, 3);
	ptem.particle_layer[0].use_gravity = true;
	ptem.particle_layer[0].g_type = ss::ParticleEmitter::GravityType::POINT;
	ptem.particle_layer[0].g_position = ss::Vector(50, 50);
	ptem.particle_layer[0].g_force = 5;

	/*
	SDL_Surface* sun = IMG_Load("sun.png");
	SDL_Texture* sunt = SDL_CreateTextureFromSurface(render, sun);
	SDL_FreeSurface(sun);
	ptem.add_particle_layer(16, sunt, 3);
	ptem.particle_layer[1].g_type = ss::ParticleEmitter::GravityType::DIRECTION;
	ptem.particle_layer[1].g_force = 5;
	ptem.particle_layer[1].use_gravity = true;
	*/

	SDL_Event* ev;

	while (game.running(_dt, _rdt)) {
		game.update();
		player_move(player, game, _dt);
		game.clear_screen();

		for (int i = 0; i < game.get_num_events(); i++) {
			if (game.events[i].type == SDL_QUIT) {
				game.quit();
			}
			if (game.is_key_pressed(SDL_SCANCODE_LCTRL) and game.is_key_just_pressed(SDL_SCANCODE_F)) {
				game.set_fullscreen(!game.get_fullscreen());
			}
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_SPACE)) {
			if (game.target_fps == 144) {
				game.target_fps = 60;
			}
			else {
				game.target_fps = 144;
			}
		}

		ptem.position = player.position;
		ptem.update(_dt);
		ptem.draw();
		player.draw(_dt);

		show_fps(fps, game.get_fps(), i, _rdt);
	}
	return 0;
}
