#include<Snow.h>


int avg_fps[60];
ss::Vector velocity;


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


void player_move(ss::Sprite& player, ss::ParticleEmitter& fire, ss::Snow &game, float delta) {
	ss::Vector direction;

	//Calculate the velocity with which the player should pe moving, and move him with that velocity
	direction.y = game.is_key_pressed(SDL_SCANCODE_S) - game.is_key_pressed(SDL_SCANCODE_W);
	direction.x = game.is_key_pressed(SDL_SCANCODE_D) - game.is_key_pressed(SDL_SCANCODE_A);
	direction.normalize();
	direction *= delta / 10;
	velocity = ss::lerp(velocity, direction, delta / 125);
	player.position += velocity;

	//Stop player from going outside the screen
	ss::Vector size = player.get_size();
	if (player.position.x > game.resolution.x - size.x) {
		player.position.x = game.resolution.x - size.x;
		velocity.x = 0;
	}
	else if (player.position.x < size.x) {
		player.position.x = size.x;
		velocity.x = 0;
	}
	if (player.position.y > game.resolution.y - size.y / 2) {
		player.position.y = game.resolution.y - size.y / 2;
		velocity.y = 0;
	}
	else if (player.position.y < size.y) {
		player.position.y = size.y;
		velocity.y = 0;
	}

	if (game.is_button_pressed(SDL_BUTTON_LEFT)) {
		fire.particle_layer[0].initial_direction = fire.position.direction_to(game.get_mouse_position()) * 20;
		fire.particle_layer[0].initial_velocity = velocity * delta * 20;
		fire.particle_layer[0].initial_velocity_min = 30;
	}
	else {
		fire.particle_layer[0].initial_direction = ss::Vector(0, -1);
		fire.particle_layer[0].initial_velocity_min = -10;
		fire.particle_layer[0].initial_velocity = ss::Vector(0);
	}

	fire.position = player.position;
}


int main(int argc, char* args[]) {
	ss::Snow game("The Snow", ss::Vector(256, 144), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 144);
	game.resize_window(512, 288);
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
	SDL_Surface* fire1 = SDL_CreateRGBSurface(NULL, 2, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(fire1, NULL, SDL_MapRGB(fire1->format, 255, 255, 255));
	SDL_Texture* fire1_t = SDL_CreateTextureFromSurface(render, fire1);
	SDL_FreeSurface(fire1);
	ptem.add_particle_layer(500, fire1_t, 0.8);
	ptem.particle_layer[0].add_color_to_gradient(255, 255, 10, 0);
	ptem.particle_layer[0].add_color_to_gradient(255, 120, 10, 0.2);
	ptem.particle_layer[0].add_color_to_gradient(255, 0, 0, 0.35);
	ptem.particle_layer[0].add_color_to_gradient(200, 0, 0, 0.45);
	ptem.sort_by_lifetime = true;
	ptem.reverse_draw_order = true;

	/*
	SDL_Surface* fire2 = SDL_CreateRGBSurface(NULL, 3, 3, 32, 0, 0, 0, 0);
	SDL_FillRect(fire2, NULL, SDL_MapRGB(fire2->format, 255, 120, 10));
	SDL_Texture* fire2_t = SDL_CreateTextureFromSurface(render, fire2);
	SDL_FreeSurface(fire2);
	ptem.add_particle_layer(75, fire2_t, 0.4);

	SDL_Surface* fire3 = SDL_CreateRGBSurface(NULL, 2, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(fire3, NULL, SDL_MapRGB(fire3->format, 255, 200, 20));
	SDL_Texture* fire3_t = SDL_CreateTextureFromSurface(render, fire3);
	SDL_FreeSurface(fire3);
	ptem.add_particle_layer(30, fire3_t, 0.3);
	*/

	ptem.particle_layer[0].use_gravity = true;
	ptem.particle_layer[0].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[0].initial_direction_randomness = 0.85;
	ptem.particle_layer[0].initial_direction = ss::Vector(0, -1);
	ptem.particle_layer[0].initial_velocity_min = -15;
	ptem.particle_layer[0].initial_velocity_max = 30;
	ptem.particle_layer[0].g_force = 80;
	ptem.particle_layer[0].velocity_damping = 0.1;
	ptem.particle_layer[0].lifetime_random = 0.3;

	/*
	ptem.particle_layer[1].use_gravity = true;
	ptem.particle_layer[1].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[1].initial_direction_randomness = 0.7;
	ptem.particle_layer[1].initial_velocity_min = 2;
	ptem.particle_layer[1].initial_velocity_max = 20;
	ptem.particle_layer[1].g_force = 70;
	ptem.particle_layer[1].velocity_damping = 0.1;

	ptem.particle_layer[2].use_gravity = true;
	ptem.particle_layer[2].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[2].initial_direction_randomness = 0.7;
	ptem.particle_layer[2].initial_velocity_min = 1;
	ptem.particle_layer[2].initial_velocity_max = 10;
	ptem.particle_layer[2].g_force = 70;
	ptem.particle_layer[2].velocity_damping = 0.1;
	*/

	SDL_Event* ev;

	while (game.running(_dt, _rdt)) {
		game.update();
		player_move(player, ptem, game, _dt);
		game.clear_screen();

		for (int i = 0; i < game.get_num_events(); i++) {
			if (game.events[i].type == SDL_QUIT) {
				game.quit();
			}
			if (game.is_key_pressed(SDL_SCANCODE_LCTRL) and game.is_key_just_pressed(SDL_SCANCODE_F)) {
				game.set_fullscreen(!game.get_fullscreen());
			}
		}

		ptem.update(_dt);
		ptem.draw();
		//player.draw(_dt);

		show_fps(fps, game.get_fps(), i, _rdt);
	}
	return 0;
}
