#include<Snow.h>
#include"Extern/Enemy/Enemy.h"


ss::Snow game("The Snow", ss::Vector(256, 144), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 144);


enum class PlayerMoveType {
	IDLE,
	MOVING,
	SHOOTING
};
ss::RandomNumberGenerator rng;
int avg_fps[60];

ss::Vector velocity;
double blink_timer = 1;
PlayerMoveType player_move_type = PlayerMoveType::IDLE;
ss::CollisionShape player_cs(ss::Vector(12), ss::Vector(0));
ss::CollisionShape window_cs;


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


void player_process(ss::Sprite& player, ss::ParticleEmitter& fire, ss::Snow &game, float delta) {
	ss::Vector direction;

	//Calculate the velocity with which the player should pe moving, and move him with that velocity
	direction.y = game.is_key_pressed(SDL_SCANCODE_S) - game.is_key_pressed(SDL_SCANCODE_W);
	direction.x = game.is_key_pressed(SDL_SCANCODE_D) - game.is_key_pressed(SDL_SCANCODE_A);
	if (direction.lenght() > 0) {
		direction.normalize();
		direction *= delta / 10;
		player_move_type = PlayerMoveType::MOVING;
	}
	else {
		if (player_move_type != PlayerMoveType::IDLE) {
			player.frame = 0;
		}
		player_move_type = PlayerMoveType::IDLE;
	}
	velocity = ss::lerp(velocity, direction, delta / 125);
	player.position += velocity;
	player_cs.position = player.position + 2;

	//Stop player from going outside the window
	window_cs.push_in(player_cs);
	player.position = player_cs.position - 2;

	//Shooting
	if (game.is_button_pressed(SDL_BUTTON_LEFT)) {
		player_move_type = PlayerMoveType::SHOOTING;
		fire.particle_layer[0].initial_direction = fire.position.direction_to(game.get_mouse_position()) * 30;
		fire.particle_layer[0].initial_velocity = velocity * delta * 20;
		fire.particle_layer[0].initial_velocity_min = 30;
	}
	else {
		fire.particle_layer[0].initial_direction = ss::Vector(0, -1);
		fire.particle_layer[0].initial_velocity_min = -10;
		fire.particle_layer[0].initial_velocity = ss::Vector(0);
	}

	//Player Animations
	switch (player_move_type) {
	case PlayerMoveType::IDLE:
		blink_timer -= delta / 1000;
		if (blink_timer < 0) {
			blink_timer = rng.randd_range(2.5, 8);
			if (player.frame == 0) {
				player.play(0, 4, 12, false);
				if (rng.randi(1)) {
					player.flip = SDL_FLIP_HORIZONTAL;
				}
				else {
					player.flip = SDL_FLIP_NONE;
				}
			}
			else if (player.frame == 4) {
				player.play(4, 0, 12, false);
			}
		}
		break;
	case PlayerMoveType::MOVING:
		if (ss::natural(direction.x) >= ss::natural(direction.y)) {
			player.frame = 4;
			if (direction.x > 0) {
				player.flip = SDL_FLIP_NONE;
			}
			else if (direction.x < 0) {
				player.flip = SDL_FLIP_HORIZONTAL;
			}
		}
		else {
			player.frame = 5;
			if (direction.y > 0) {
				player.flip = SDL_FLIP_VERTICAL;
			}
			else if (direction.y < 0) {
				player.flip = SDL_FLIP_NONE;
			}
		}
		break;
	case PlayerMoveType::SHOOTING:
		if (ss::natural(fire.particle_layer[0].initial_direction.x) > ss::natural(fire.particle_layer[0].initial_direction.y)) {
			player.frame = 4;
			if (fire.particle_layer[0].initial_direction.x > 0) {
				player.flip = SDL_FLIP_NONE;
			}
			else if (fire.particle_layer[0].initial_direction.x < 0) {
				player.flip = SDL_FLIP_HORIZONTAL;
			}
		}
		else {
			player.frame = 5;
			if (fire.particle_layer[0].initial_direction.y > 0) {
				player.flip = SDL_FLIP_VERTICAL;
			}
			else if (fire.particle_layer[0].initial_direction.y < 0) {
				player.flip = SDL_FLIP_NONE;
			}
		}
		break;
	default:
		break;
	}

	fire.position = player.position + player.get_size() / 2;
}


void init_part(ss::ParticleEmitter& ptem, SDL_Renderer* render) {
	//Creating the particle emitter and adding a particle to it
	SDL_Surface* fire1 = SDL_CreateRGBSurface(NULL, 2, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(fire1, NULL, SDL_MapRGB(fire1->format, 255, 255, 255));
	SDL_Texture* fire1_t = SDL_CreateTextureFromSurface(render, fire1);
	SDL_FreeSurface(fire1);
	ptem.add_particle_layer(350, fire1_t, 0.8);

	//Creating the color gradient to make it look like a fire
	ptem.particle_layer[0].add_color_to_gradient(255, 255, 10, 0);
	ptem.particle_layer[0].add_color_to_gradient(255, 120, 10, 0.2);
	ptem.particle_layer[0].add_color_to_gradient(255, 0, 0, 0.3);
	ptem.particle_layer[0].add_color_to_gradient(255, 0, 0, 0.45);
	ptem.particle_layer[0].add_color_to_gradient(255, 255, 0, 0.6);

	//Creating a scale curve, to make the fire look more dense in the middle
	ptem.particle_layer[0].add_scale_to_scale_curve(1, 0);
	ptem.particle_layer[0].add_scale_to_scale_curve(1, 0.15);
	ptem.particle_layer[0].add_scale_to_scale_curve(2, 0.3);
	ptem.particle_layer[0].add_scale_to_scale_curve(0.5, 0.5);

	//Settings that make the particles behave like a fire
	ptem.particle_layer[0].use_gravity = true;
	ptem.particle_layer[0].g_direction = ss::Vector(0, -1);
	ptem.particle_layer[0].initial_direction_randomness = 0.85;
	ptem.particle_layer[0].initial_direction = ss::Vector(0, -1);
	ptem.particle_layer[0].initial_velocity_min = -15;
	ptem.particle_layer[0].initial_velocity_max = 30;
	ptem.particle_layer[0].g_force = 80;
	ptem.particle_layer[0].velocity_damping = 0.1;
	ptem.particle_layer[0].lifetime_random = 0.3;
	ptem.emission_shape = ss::ParticleEmitter::EmissionShape::CIRCLE;
	ptem.emission_radius = 5;
}


int main(int argc, char* args[]) {
	rng.randomize();
	window_cs.size = game.resolution;
	game.resize_window(512, 288);
	ss::Text fps(game.get_window(), "", "basic.ttf", 9);
	fps.position = ss::Vector(5, 5);
	SDL_Color text_color = SDL_Color(), border_color = SDL_Color(), fill_color = SDL_Color();

	text_color.r = 255; text_color.g = 255; text_color.b = 255; text_color.a = 255;
	border_color.r = 120; border_color.g = 140; border_color.b = 160; border_color.a = 255;
	fill_color.r = 60; fill_color.g = 80; fill_color.b = 100; fill_color.a = 255;

	//Creating the player sprite
	const char* frames[6] = {
		"Sprites/Player/player_idle0000.png",
		"Sprites/Player/player_idle0001.png",
		"Sprites/Player/player_idle0002.png",
		"Sprites/Player/player_idle0003.png",
		"Sprites/Player/player_idle0004.png",
		"Sprites/Player/player_idle0005.png"
	};
	ss::Sprite player = ss::Sprite(game.get_window(), 6, frames);
	player.position = game.resolution / 2 - player.get_size() / 2;
	player_cs.size = player.get_size() - 4;

	//Creating the snowman/ememy sprite
	

	//Creating a particle emitter for the fire
	ss::ParticleEmitter ptem(game.get_window(), player.position);
	init_part(ptem, game.get_renderer());

	//Creating the ground
	ss::Vector ground_size(256, 144);
	ss::Texture gnd_tex(game.get_window(), ground_size);
	bool* ground_b = new bool[256 * 144];
	memset(ground_b, 1, sizeof(bool) * 256 * 144);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 144; j++) {
			int r = rng.randi_range(210, 230);
			int g = rng.randi_range(r, 230);
			gnd_tex.set_pixel(ss::Vector(i, j), r, g, 240);
		}
	}
	gnd_tex.update();

	//Enables drawing of CollisionShapes in debug mode
#if defined _DEBUG
	bool draw_debug = true;
#endif

	SDL_Rect window_rect = SDL_Rect();
	window_rect.x = 0;
	window_rect.y = 0;
	window_rect.w = window_cs.size.x;
	window_rect.h = window_cs.size.y;
	float _dt = 0.0f;
	float _rdt = 0.0f;
	int i = 0;

	//Main loop, runs every frame
	while (game.running(_dt, _rdt)) {
		game.update();
		game.clear_screen();
		gnd_tex.update();
		gnd_tex.draw();
		player_process(player, ptem, game, _dt);

		for (int i = 0; i < ptem.get_num_of_particles(); i++) {
			ss::Vector p_pos = ptem.get_particle_position(i);
			for (int x = p_pos.x - 1; x < p_pos.x + 1; x++) {
				for (int y = p_pos.y - 1; y < p_pos.y + 1; y++) {
					if (x >= 0 and x < (int)ground_size.x and y >= 0 and y < (int)ground_size.y) {
						if (ground_b[x + y * (int)ground_size.x]) {
							int r = rng.randi_range(0, 40);
							int g = rng.randi_range(210, 230);
							int b = rng.randi_range(0, 40);
							gnd_tex.set_pixel(ss::Vector(x, y), r, g, b);
							ground_b[x + y * (int)ground_size.x] = false;
						}
					}
				}
			}
		}

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
		player.draw(_dt);

		//Draws the CollisionShapes and other debug info to the screen
#if defined _DEBUG
		if (draw_debug) {
			show_fps(fps, game.get_fps(), i, _rdt);
		}
		if (game.is_key_just_pressed(SDL_SCANCODE_F3)) {
			draw_debug = !draw_debug;
		}
#endif
	}
	return 0;
}
