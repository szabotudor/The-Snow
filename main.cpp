#include<Snow.h>
#include"Extern/Enemy/Enemy.h"


ss::Snow game("The Snow", ss::Vector(320, 180), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 144);
ss::Vector ground_size(game.resolution * 3);
ss::Vector camera_offset(180, 90);
ss::Vector player_position(320, 180);
ss::Vector player_draw_center;

#if defined _DEBUG
string console_text = " ";
ss::Text console(game.get_window(), " ", "Pixel.ttf", 7);
long long frame, last_console_update_frame;
int console_lines;

void print_to_console(string text) {
	int text_len = strlen(text.c_str());
	for (int i = 0; i < text_len; i++) {
		if (text[i] == '\n') {
			console_lines++;
		}
	}
	console_text += text + '\n';
	console_lines++;

	if (console_text[0] == ' ') {
		if (strlen(console_text.c_str()) > 1) {
			console_text = console_text.substr(1);
		}
	}
	if (console_lines > 12) {
		int j = 0;
		for (j; console_text[j] != '\n'; j++);
		console_text = console_text.substr(j + 1);
	}
	if (frame != last_console_update_frame) {
		last_console_update_frame = frame;
		console.set_rich_text(console_text);
	}
}
#endif


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

int enemies = 0;
Enemy* enemy = new Enemy[64];


#if defined _DEBUG
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
#endif


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
	player_position += velocity;

	//Stop player from going outside the window
	player_cs.position = player_position - camera_offset + 2;
	window_cs.push_in(player_cs);
	if (player_cs.position.distance_to(player_position - camera_offset + 2) > 1) {
		player_position = camera_offset + player.position;
	}
	player.position = player_cs.position - 2;

	//Calculate collision between player and enemies
	for (int i = 0; i < enemies; i++) {
		enemy[i].collision.push_out(player_cs);
	}
	player.position = player_cs.position - 2;

	//Shooting
	if (game.is_button_pressed(SDL_BUTTON_LEFT)) {
		player_move_type = PlayerMoveType::SHOOTING;
		fire.particle_layer[0].initial_direction = lerp(fire.particle_layer[0].initial_direction, player.position.direction_to(game.get_mouse_position()) * 30, delta / 100).normalized() * 30;
		fire.particle_layer[0].initial_velocity = direction * 25 / (delta / 10);
		fire.particle_layer[0].initial_velocity_min = 30;
		camera_offset += fire.particle_layer[0].initial_direction * delta / 100;
	}
	else {
		fire.particle_layer[0].initial_direction = ss::Vector(0, -1);
		fire.particle_layer[0].initial_velocity_min = -10;
		fire.particle_layer[0].initial_velocity = ss::Vector(0);
	}

	//Make the camera follow the player
	ss::Vector player_size = player.get_size();
	if ((player.position - player_draw_center).lenght() > 0.1) {
		camera_offset = lerp(camera_offset, player_position - player_draw_center, delta / 100);
		if (camera_offset.x < 0) {
			camera_offset.x = 0;
		}
		if (camera_offset.y < 0) {
			camera_offset.y = 0;
		}

		if (camera_offset.x + game.resolution.x > ground_size.x) {
			camera_offset.x = ground_size.x - game.resolution.x;
		}
		if (camera_offset.y + game.resolution.y > ground_size.y) {
			camera_offset.y = ground_size.y - game.resolution.y;
		}
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

	fire.position = player_position + player_size / 2;
}


void init_part(ss::ParticleEmitter& ptem, SDL_Renderer* render) {
	//Creating the particle emitter and adding a particle to it
	SDL_Surface* fire1 = SDL_CreateRGBSurface(NULL, 2, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(fire1, NULL, SDL_MapRGB(fire1->format, 255, 255, 255));
	SDL_Texture* fire1_t = SDL_CreateTextureFromSurface(render, fire1);
	SDL_FreeSurface(fire1);
	ptem.add_particle_layer(250, fire1_t, 0.8);

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
	game.resize_window(640, 360);
	ss::Text fps(game.get_window(), "", "basic.ttf", 9);
	fps.color.b = 0;
	fps.color.g = 100;
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
	player_draw_center = game.resolution / 2 - player.get_size() / 2;

	//Creating a particle emitter for the fire
	ss::ParticleEmitter ptem(game.get_window(), player.position);
	init_part(ptem, game.get_renderer());

	//Creating the ground
	ss::Texture gnd_tex(game.get_window(), ground_size);
	bool** ground_b = new bool*[(int)ground_size.x];
	long long snow_pixels = ground_size.x * ground_size.y;
#if defined _DEBUG
	print_to_console(to_string(snow_pixels) + " pixels to fill");
#endif
	int r = rng.randi_range(235, 255);
	int g = rng.randi_range(ss::clamp(235, 255, r + 10), 255);
	int ip = 0, jp = 0;
	for (int i = 0; i < ground_size.x; i++) {
		ground_b[i] = new bool[(int)ground_size.y];
		for (int j = 0; j < ground_size.y; j++) {
#if defined _DEBUG
			if (i <= 1 or i >= ground_size.x - 2 or j <= 1 or j >= ground_size.y - 2) {
				r = 0;
				g = 0;
			}
#endif
			if (rng.randi() < 50) {
				r = rng.randi_range(235, 255);
				g = rng.randi_range(ss::clamp(235, 255, r + 10), 255);
			}
			else {
				jp = j;
				ip = i - 1;
				if (ip < 0) {
					jp--;
					ip++;
				}
				if (jp < 0) {
					jp++;
				}
				r = gnd_tex.get_pixel(ss::Vector(ip, jp)).r;
				g = gnd_tex.get_pixel(ss::Vector(ip, jp)).g;
			}
			gnd_tex.set_pixel(ss::Vector(i, j), r, g, 255);
			ground_b[i][j] = true;
		}
	}
	gnd_tex.update();

	//Enables drawing of CollisionShapes in debug mode
#if defined _DEBUG
	bool draw_debug = true;
	console.position.y = 64;
	console.color.r = 0;
	console.color.g = 100;
	console.color.b = 100;
	console.color.a = 160;
	player_cs.enable_draw(game.get_window());
	print_to_console("Start game...\n");
#endif

	SDL_Rect window_rect = SDL_Rect();
	window_rect.x = 0;
	window_rect.y = 0;
	window_rect.w = window_cs.size.x;
	window_rect.h = window_cs.size.y;
	float _dt = 0.0f;
	float _rdt = 0.0f;
	int i = 0;

	double spawn_timer = 2;
	bool first_frame = true;

	//Main loop, runs every frame
	while (game.running(_dt, _rdt)) {
		game.update();
		game.clear_screen();
		gnd_tex.update();
		gnd_tex.position -= camera_offset;
		gnd_tex.draw();
		gnd_tex.position += camera_offset;
		player_process(player, ptem, game, _dt);

		//Spawn an enemy at a random interval
		if (enemies < 64) {
#if defined _DEBUG
			if (spawn_timer < 0 or game.is_key_just_pressed(SDL_SCANCODE_F1)) {
#else
			if (spawn_timer < 0) {
#endif
				spawn_timer = rng.randd_range(2, 4);
				enemy[enemies] = Enemy(player.position + rng.randdir() * rng.randd_range(50, 100));
				window_cs.push_in(enemy[enemies].collision);
#if defined _DEBUG
				enemy[enemies].collision.enable_draw(game.get_window());
				print_to_console("Enemy " + to_string(enemies) + " spawned");
#endif
				for (int i = 0; i < enemies; i++) {
					enemy[i].collision.push_out(enemy[enemies].collision);
				}
				enemies++;
			}
			else {
				//spawn_timer -= _dt / 1000;
			}
//Spawn an enemy if there is room for it
#if defined _DEBUG
			if (game.is_key_pressed(SDL_SCANCODE_LSHIFT) and game.is_key_pressed(SDL_SCANCODE_LCTRL)) {
				if (game.is_key_just_pressed(SDL_SCANCODE_F1)) {
					for (enemies; enemies < 64; enemies++) {
						enemy[enemies] = Enemy(player.position + rng.randdir() * rng.randd_range(50, 100));
						window_cs.push_in(enemy[enemies].collision);
						enemy[enemies].collision.enable_draw(game.get_window());
						print_to_console("Enemy " + to_string(enemies) + " spawned");
						for (int i = 0; i < enemies; i++) {
							enemy[i].collision.push_out(enemy[enemies].collision);
						}
					}
					enemies--;
				}
			}
#endif
		}

		//Process enemies
		for (int i = 0; i < enemies; i++) {
			enemy[i].process(_dt);
			if (enemy[i].is_dead()) {
#if defined _DEBUG
				print_to_console("Enemy " + to_string(i) + " killed");
#endif
				for (int j = i; j < enemies - 1; j++) {
					enemy[j] = enemy[j + 1];
				}
				enemies--;
			}
		}

		//Verify colision of fire particles with snow on the ground and with snowmen
		int r = 0, g = 0, b = 0;
		for (int i = 0; i < ptem.get_num_of_particles(); i++) {
			ss::Vector p_pos = ptem.get_particle_position(i);
			//Set color of pixels on ground to green
			for (int x = p_pos.x - 10; x < p_pos.x + 10; x++) {
				for (int y = p_pos.y - 10; y < p_pos.y + 10; y++) {
					if (x >= 0 and x < ground_size.x and y >= 0 and y < ground_size.y) {
						if (ground_b[x][y]) {
							if (p_pos.distance_to(ss::Vector(x, y)) < 140) {
								if (rng.randi() < 50) {
									r = rng.randi_range(0, 40);
									g = rng.randi_range(180, 200);
									b = rng.randi_range(0, 40);
								}
								else {
									SDL_Color color = gnd_tex.get_pixel(ss::Vector(y - 1, x - rng.randi(1)));
									r = ss::clamp(0, 40, r);
									g = ss::clamp(180, 200, g);
									b = ss::clamp(0, 40, b);
								}
								gnd_tex.set_pixel(ss::Vector(x, y), r, g, b);
								ground_b[x][y] = false;
								snow_pixels--;
							}
						}
					}
				}
			}
			//Verify collision between fire particles and snowmen
			for (int j = 0; j < enemies; j++) {
				if (enemy[j].collision.is_colliding_with(p_pos)) {
#if defined _DEBUG
					if (!enemy[j].is_invulnerable()) {
						print_to_console("Enemy " + to_string(j) + " damaged (" + to_string(enemy[j].get_hp()) + " hp)");
					}
#endif
					enemy[j].damage();
				}
			}
		}

		ptem.draw_offset = ss::Vector() - camera_offset;
		ptem.update(_dt);
		ptem.draw();
		player.draw(_dt);

		//Draws the CollisionShapes and other debug info to the screen
#if defined _DEBUG
		frame++;
		if (draw_debug) {
			player_cs.draw();
			for (int i = 0; i < enemies; i++) {
				enemy[i].collision.draw();
			}
			console.draw();
			show_fps(fps, game.get_fps(), i, _rdt);
		}
		if (game.is_key_just_pressed(SDL_SCANCODE_F3)) {
			draw_debug = !draw_debug;
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_F2)) {
			if (game.target_fps == 144) {
				game.target_fps = 30;
			}
			else {
				game.target_fps = 144;
			}
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_F5)) {
			" ";
			print_to_console(to_string(snow_pixels) + " snow left");
		}

		if (snow_pixels == 0) {
			print_to_console("All snow cleared!");
			snow_pixels--;
		}

		if (first_frame) {
			first_frame = false;
			print_to_console("Game started");
		}
#endif
		//Process events (for example quit)
		for (int i = 0; i < game.get_num_events(); i++) {
			if (game.events[i].type == SDL_QUIT) {
				game.quit();
			}
			if (game.is_key_pressed(SDL_SCANCODE_LCTRL) and game.is_key_just_pressed(SDL_SCANCODE_F)) {
				game.set_fullscreen(!game.get_fullscreen());
			}
		}
	}
	return 0;
}
