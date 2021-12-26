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
	player.position = ss::Vector(100, 100);
	player.play(0, 5, 8);

	ss::ParticleEmitter ptem(game.get_window(), ss::Vector(50));
	SDL_Surface* fire1 = SDL_CreateRGBSurface(NULL, 4, 4, 32, 0, 0, 0, 0);
	SDL_FillRect(fire1, NULL, SDL_MapRGB(fire1->format, 255, 10, 10));
	SDL_Texture* fire1_t = SDL_CreateTextureFromSurface(render, fire1);
	SDL_FreeSurface(fire1);
	ptem.add_particle_layer(150, fire1_t, 0.5);

	SDL_Surface* fire2 = SDL_CreateRGBSurface(NULL, 3, 3, 32, 0, 0, 0, 0);
	SDL_FillRect(fire2, NULL, SDL_MapRGB(fire2->format, 255, 120, 10));
	SDL_Texture* fire2_t = SDL_CreateTextureFromSurface(render, fire2);
	SDL_FreeSurface(fire2);
	ptem.add_particle_layer(50, fire2_t, 0.4);

	SDL_Surface* fire3 = SDL_CreateRGBSurface(NULL, 2, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(fire3, NULL, SDL_MapRGB(fire3->format, 255, 200, 20));
	SDL_Texture* fire3_t = SDL_CreateTextureFromSurface(render, fire3);
	SDL_FreeSurface(fire3);
	ptem.add_particle_layer(20, fire3_t, 0.3);

	ptem.particle_layer[0].use_gravity = true;
	ptem.particle_layer[0].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[0].initial_velocity_min = ss::Vector(-20, -40);
	ptem.particle_layer[0].initial_velocity_max = ss::Vector(20, 12);
	ptem.particle_layer[0].g_force = 70;
	ptem.particle_layer[0].velocity_damping = 0.1;

	ptem.particle_layer[1].use_gravity = true;
	ptem.particle_layer[1].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[1].initial_velocity_min = ss::Vector(-12, -20);
	ptem.particle_layer[1].initial_velocity_max = ss::Vector(12, 12);
	ptem.particle_layer[1].g_force = 70;
	ptem.particle_layer[1].velocity_damping = 0.1;

	ptem.particle_layer[2].use_gravity = true;
	ptem.particle_layer[2].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[2].initial_velocity_min = ss::Vector(-5, -10);
	ptem.particle_layer[2].initial_velocity_max = ss::Vector(5, 11);
	ptem.particle_layer[2].g_force = 70;
	ptem.particle_layer[2].velocity_damping = 0.1;

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

		ptem.position = player.position + 6;
		ptem.update(_dt);
		ptem.draw();
		//player.draw(_dt);

		show_fps(fps, game.get_fps(), i, _rdt);
	}
	return 0;
}
