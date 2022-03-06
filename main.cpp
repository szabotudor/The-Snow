#include"Extern/Setup.h"
#include<Snow.h>
#include<SDL_mixer.h>
#include"Extern/Enemy/Enemy.h"
#include"Extern/Snowball/Snowball.h"


Mix_Chunk* snd_player_hit = NULL;
Mix_Chunk* snd_fire = NULL;
Mix_Chunk* snd_melt = NULL;
Mix_Chunk* snd_click = NULL;
Mix_Chunk* snd_coin = NULL;

Mix_Music* mus_intro = NULL;
Mix_Music* mus_body1 = NULL;
Mix_Music* mus_body2 = NULL;
Mix_Music* mus_outro = NULL;
Mix_Music* mus_win = NULL;

ss::Snow game("The Snow", ss::Vector(320, 180), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 144);
ss::Vector ground_size(game.resolution * 1.8);
ss::Vector camera_offset(0);
ss::Vector player_draw_center;
uint64_t score = 0, score_record = 0;

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


Diff difficulty = Diff::NORMAL;
Diff unlocked = Diff::NORMAL;
ss::RandomNumberGenerator rng;
int avg_fps[60];

ss::Vector velocity;
float invulnerability = 1;
double spawn_timer = 0.5;
double blink_timer = 1;
double player_visibility_timer = 0;
double insn_change_timer = 0;
double camera_shake = 0;
double natural_shake = 0;
bool player_visible = true;
bool player_dead = false;
bool in_menu = true;
bool select_screen = false;
bool first_boot = true;
bool mouse_pressed = false;
bool force_reset = false;
uint8_t life = 3;
PlayerMoveType player_move_type = PlayerMoveType::IDLE;
ss::CollisionShape player_cs(ss::Vector(12), ss::Vector(0));
ss::CollisionShape window_cs;
ss::CollisionShape ground_cs;

int enemies = 0;
uint16_t max_enemies = 6;
Enemy* enemy = nullptr;
int snowballs;
Snowball** snowball = new Snowball*[512];

uint32_t coins = 0;
uint32_t coins_to_draw = 0;
uint32_t max_coins_to_draw = 256;
ss::Vector* coin_positions = new ss::Vector[256];

uint32_t total_hearts = 1;
uint32_t heart_cost = 12;

ss::Vector tree_positions[8];
uint8_t tree_states[8];
float tree_timers[8];


#if defined _DEBUG
bool god_mode = false;
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


void make_ground(bool**& ground_b, ss::Texture& gnd_tex, long long& snow_pixels) {
	snow_pixels = ground_size.x * ground_size.y;
	int r = rng.randi_range(235, 255);
	int g = rng.randi_range(ss::clamp(235, 255, r + 10), 255);
	int ip = 0, jp = 0;
	for (int i = 0; i < ground_size.x; i++) {
		ground_b[i] = new bool[(int)ground_size.y];
		for (int j = 0; j < ground_size.y; j++) {
			if (rng.randi() < 50) {
				r = rng.randi_range(235, 255);
				g = rng.randi_range(ss::clamp(235, 255, r + 10), 255);
			}
			else if (i > 0 and j > 0) {
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

	for (int i = 0; i < 8; i++) {
		tree_states[i] = 4;
	}
}


void prepare_game(bool**& ground_b, ss::Texture& gnd_tex, long long& snow_pixels, Diff game_difficulty, ss::Sprite& heart) {
	life = 3 * total_hearts;
	first_boot = false;
	select_screen = false;
	in_menu = false;
	difficulty = game_difficulty;
	if (snow_pixels < 200 or player_dead or force_reset) {
		make_ground(ground_b, gnd_tex, snow_pixels);
		score = 0;
		coins_to_draw = 0;
	}
	spawn_timer = 0.5;
	player_dead = false;
	player_move_type = PlayerMoveType::IDLE;
	velocity = 0;
	Mix_FadeInChannel(CH_FIRE, snd_fire, 1, 1000);
}


void do_unlock(ss::Button& hard, ss::Button& impos, ss::Button& insn) {
	switch (unlocked) {
	case Diff::INSANE:
		insn.set_text((char*)">s{:1?=u");
	case Diff::IMPOSSIBLE:
		impos.set_text((char*)"IMPOSSIBLE");
	case Diff::HARD:
		hard.set_text((char*)"HARD");
		break;
	case Diff::NORMAL:
		break;
	default:
		break;
	}
}


void save_data() {
	ofstream fout("game.data", ios::binary);
	fout << (uint16_t)unlocked << " " << score_record << " " << coins << " " << total_hearts << " " << heart_cost;
	fout.close();
}


void process_menu(double delta, ss::Button& playbtn, ss::Button& quitbtn, ss::Button& norm, ss::Button& hard, ss::Button& impos, ss::Button& insn, ss::Button& back) {
	playbtn.update();
	quitbtn.update();
	norm.update();
	hard.update();
	impos.update();
	back.update();

	if (unlocked == Diff::INSANE) {
		if (insn_change_timer <= 0) {
			char* txt = new char[9](0);
			for (int i = 0; i < 8; i++) {
				txt[i] = rng.randi_range(32, 126);
			}
			insn.set_text(txt);
			insn_change_timer = 0.03;
			delete[] txt;
		}
		else {
			insn_change_timer -= delta / 1000;
		}
	}
	insn.update();

	if (!select_screen) {
		if (playbtn.hovered) playbtn.draw_offset = ss::lerp(playbtn.draw_offset, ss::Vector(0, -3), delta / 40);
		else playbtn.draw_offset = ss::lerp(playbtn.draw_offset, ss::Vector(0, 0), delta / 40);

		if (quitbtn.hovered) quitbtn.draw_offset = ss::lerp(quitbtn.draw_offset, ss::Vector(0, -3), delta / 40);
		else quitbtn.draw_offset = ss::lerp(quitbtn.draw_offset, ss::Vector(0, 0), delta / 40);
	}
	else {
		if (norm.hovered) norm.draw_offset = ss::lerp(norm.draw_offset, ss::Vector(0, -3), delta / 40);
		else norm.draw_offset = ss::lerp(norm.draw_offset, ss::Vector(0, 0), delta / 40);

		if (hard.hovered) hard.draw_offset = ss::lerp(hard.draw_offset, ss::Vector(0, -3), delta / 40);
		else hard.draw_offset = ss::lerp(hard.draw_offset, ss::Vector(0, 0), delta / 40);

		if (impos.hovered) impos.draw_offset = ss::lerp(impos.draw_offset, ss::Vector(0, -3), delta / 40);
		else impos.draw_offset = ss::lerp(impos.draw_offset, ss::Vector(0, 0), delta / 40);

		if (insn.hovered) insn.draw_offset = ss::lerp(insn.draw_offset, ss::Vector(0, -3), delta / 40);
		else insn.draw_offset = ss::lerp(insn.draw_offset, ss::Vector(0, 0), delta / 40);

		if (back.hovered) back.draw_offset = ss::lerp(back.draw_offset, ss::Vector(0, -3), delta / 40);
		else back.draw_offset = ss::lerp(back.draw_offset, ss::Vector(0, 0), delta / 40);
	}
}


void damage_player(ss::Sprite& player, ss::ParticleEmitter& fire, ss::Snow& game, ss::Vector direction, float delta, ss::Sprite& heart) {
#if defined _DEBUG
	if (god_mode or in_menu) {
		return;
	}
#endif
	if (in_menu) return;

	if (invulnerability < 0) {
		life--;
		Mix_PlayChannel(CH_PLAYER_HIT, snd_player_hit, 0);
		velocity += direction * delta / 4;
		invulnerability = 1;
#if defined _DEBUG
		print_to_console("Player damaged (" + to_string(life) + " life left)");
#endif
	}
	camera_shake = 2;
}


void player_process(ss::Sprite& player, ss::ParticleEmitter& fire, ss::Snow &game, float delta) {
	//Flicker the player sprite when invulnerable
	if (invulnerability > 0) {
		invulnerability -= delta / 1000;
		if (player_visibility_timer > 0.03) {
			player_visible = !player_visible;
			player_visibility_timer = 0;
		}
		else {
			player_visibility_timer += delta / 1000;
		}
	}
	else {
		player_visible = true;
	}

	ss::Vector direction;

	//Calculate the velocity with which the player should pe moving, and move him with that velocity
	if (!(player_dead or in_menu)) {
		direction.y = game.is_key_pressed(SDL_SCANCODE_S) - game.is_key_pressed(SDL_SCANCODE_W);
		direction.x = game.is_key_pressed(SDL_SCANCODE_D) - game.is_key_pressed(SDL_SCANCODE_A);
		if (direction.lenght() > 0) {
			direction.normalize();
			direction *= delta / 10;
			player_move_type = PlayerMoveType::MOVING;
		}
		else {
			if (player_move_type != PlayerMoveType::IDLE and player_move_type != PlayerMoveType::DEAD) {
				player.frame = 0;
			}
			player_move_type = PlayerMoveType::IDLE;
		}
		velocity = ss::lerp(velocity, direction, delta / 125);
		player_cs.position += velocity;
	}

	//Stop player from going outside the window
	ground_cs.push_in(player_cs);

	//Calculate collision between player and enemies
	for (int i = 0; i < enemies; i++) {
		enemy[i].collision.push_out(player_cs);
	}
	player.position = player_cs.position - camera_offset - 2;

	//Shooting
	if (game.is_button_pressed(SDL_BUTTON_LEFT) and !in_menu) {
		player_move_type = PlayerMoveType::SHOOTING;
		fire.particle_layer[0].initial_direction = lerp(fire.particle_layer[0].initial_direction, player.position.direction_to(game.get_mouse_position()), delta / 60).normalized();
		fire.particle_layer[0].initial_velocity = velocity / (delta / 1000);
		fire.particle_layer[0].initial_velocity_min = 180;
		fire.particle_layer[0].initial_velocity_max = 200;
		fire.particle_layer[0].initial_direction_randomness = 0.2;
		//camera_offset += fire.particle_layer[0].initial_direction * delta / 1000;
	}
	else {
		fire.particle_layer[0].initial_direction = ss::Vector(0, -1);
		fire.particle_layer[0].initial_velocity_min = -15;
		fire.particle_layer[0].initial_velocity_max = 15;
		fire.particle_layer[0].initial_direction_randomness = 0.85;
		fire.particle_layer[0].initial_velocity = ss::Vector(0);
	}

	//Make the camera follow the player
	ss::Vector player_size = player.get_size();
	if ((player.position - player_draw_center).lenght() > 0.1) {
		camera_offset = lerp(camera_offset, player_cs.position - player_draw_center, delta / 100);
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

	//Check if player has died
	if (life == 0) {
		fire.set_draw_ammount(0);
		player_move_type = PlayerMoveType::DEAD;
	}
	else {
		fire.set_draw_ammount(300);
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
	case PlayerMoveType::DEAD:
		if (!player_dead) {
#if defined _DEBUG
			print_to_console("Player killed");
#endif
			player.play(0, 2, 2, false);
			player_dead = true;
			in_menu = true;
			enemies = 0;
		}
		break;
	default:
		break;
	}

	fire.position = player_cs.position + player_size / 4;
}


void init_part(ss::ParticleEmitter& ptem, SDL_Renderer* render) {
	//Creating the particle emitter and adding a particle to it
	SDL_Surface* fire1 = SDL_CreateRGBSurface(NULL, 2, 2, 32, 0, 0, 0, 0);
	SDL_FillRect(fire1, NULL, SDL_MapRGB(fire1->format, 255, 255, 255));
	SDL_Texture* fire1_t = SDL_CreateTextureFromSurface(render, fire1);
	SDL_FreeSurface(fire1);
	ptem.add_particle_layer(300, fire1_t, 0.8, 0);

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


void add_to_score(ss::Text& score_text, uint32_t add) {
	score += add;
	score_text.set_text(to_string(score));
	if (score > score_record) {
		score_record = score;
	}
	score_text.scale = 1.2;
	score_text.position.x = 310 - score_text.get_text().length() * 8;
}


int main(int argc, char* args[]) {
	Mix_Init(MIX_INIT_MP3);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw runtime_error("Could not start audio straming");
	}
	Mix_AllocateChannels(10);
	snd_player_hit = Mix_LoadWAV("Sounds/Effects/hit.wav");
	snd_fire = Mix_LoadWAV("Sounds/Effects/fire.wav");
	snd_melt = Mix_LoadWAV("Sounds/Effects/melt.wav");
	snd_click = Mix_LoadWAV("Sounds/Effects/click.wav");
	snd_coin = Mix_LoadWAV("Sounds/Effects/coin.wav");

	Mix_VolumeChunk(snd_player_hit, 44);
	Mix_VolumeChunk(snd_fire, 72);
	Mix_VolumeChunk(snd_melt, 64);
	Mix_VolumeChunk(snd_click, 32);
	Mix_VolumeChunk(snd_coin, 32);
	float melt_volume = 0;

	mus_intro = Mix_LoadMUS("Sounds/Music/music-intro.mp3");
	mus_body1 = Mix_LoadMUS("Sounds/Music/music-body-1.mp3");
	mus_body2 = Mix_LoadMUS("Sounds/Music/music-body-2.mp3");
	mus_outro = Mix_LoadMUS("Sounds/Music/music-outro.mp3");
	mus_win = Mix_LoadMUS("Sounds/Music/music-win.mp3");

	enemy = new Enemy[max_enemies];
	rng.randomize();
	window_cs.size = game.resolution;
	game.resize_window(640, 360);
	ss::Text fps(game.get_window(), "", "stfont.ttf", 8);
	fps.color.b = 0;
	fps.color.g = 100;
	fps.position = ss::Vector(5, 5);
	SDL_Color text_color = SDL_Color(), fill_color = SDL_Color();

	text_color.r = 60; text_color.g = 80; text_color.b = 255; text_color.a = 255;
	fill_color.r = 60; fill_color.g = 80; fill_color.b = 100; fill_color.a = 255;

	ss::Text title_text{ game.get_window(), "Reign of The\nEvil Snowmen", "stfont.ttf", 16 };
	title_text.position = ss::Vector(70, 25);
	title_text.color = fill_color;

	//Play button
	ss::Button playbtn = ss::Button(game.get_window(), text_color, "PLAY", "stfont.ttf", 16);
	playbtn.update();
	playbtn.position = ss::Vector(game.resolution / 2 - playbtn.bounding_box.size / 2 - ss::Vector(0, 20)) + ss::Vector(0, 15);

	//Quit button
	ss::Button quitbtn = ss::Button(game.get_window(), text_color, "QUIT", "stfont.ttf", 16);
	quitbtn.update();
	quitbtn.position = ss::Vector(game.resolution / 2 - quitbtn.bounding_box.size / 2 + ss::Vector(0, 20)) + ss::Vector(0, 15);

	//Normal difficulty selector button
	ss::Button normbtn = ss::Button(game.get_window(), text_color, "NORMAL", "stfont.ttf", 16);
	normbtn.update();
	normbtn.position = ss::Vector(game.resolution / 2 - normbtn.bounding_box.size / 2 + ss::Vector(0, -60)) + ss::Vector(0, 0.5);

	//Hard difficulty selector button
	ss::Button hardbtn = ss::Button(game.get_window(), text_color, "????", "stfont.ttf", 16);
	hardbtn.update();
	hardbtn.position = ss::Vector(game.resolution / 2 - hardbtn.bounding_box.size / 2 + ss::Vector(0, -20)) + ss::Vector(0, 0.5);

	//Impossible difficulty selector button
	ss::Button imposbtn = ss::Button(game.get_window(), text_color, "??????????", "stfont.ttf", 16);
	imposbtn.update();
	imposbtn.position = ss::Vector(game.resolution / 2 - imposbtn.bounding_box.size / 2 + ss::Vector(0, 20)) + ss::Vector(0, 0.5);

	//Impossible difficulty selector button
	ss::Button insnbtn = ss::Button(game.get_window(), text_color, "????????", "stfont.ttf", 16);
	insnbtn.update();
	insnbtn.position = ss::Vector(game.resolution / 2 - insnbtn.bounding_box.size / 2 + ss::Vector(0, 60)) + ss::Vector(0, 0.5);

	//Back button
	ss::Button backbtn = ss::Button(game.get_window(), text_color, "BACK", "stfont.ttf", 8);
	backbtn.update();
	backbtn.position = ss::Vector(5, game.resolution.y - 13);

	//Buy life button
	ss::Button buy_life{ game.get_window(), text_color, "BUY", "stfont.ttf", 8 };
	buy_life.update();
	buy_life.position = ss::Vector(24, 148);
	ss::Text buy_life_text{ game.get_window(), "COST:12", "stfont.ttf", 8 };
	buy_life_text.color = text_color;
	buy_life_text.position = ss::Vector(24, 166);

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
	player_cs.position = player.position;
	camera_offset = player_cs.position - game.resolution / 2;
	player_cs.size = player.get_size() - 4;
	player_draw_center = game.resolution / 2 - player.get_size() / 2;
	
	//Load hearts
	const char* heart_frames[4] = {
		"Sprites/Heart/heart0000.png",
		"Sprites/Heart/heart0001.png",
		"Sprites/Heart/heart0002.png",
		"Sprites/Heart/heart0003.png"
	};
	ss::Sprite heart{game.get_window(), 4, heart_frames};
	heart.position = ss::Vector(8, 8);
	float heart_timer = 1;

	//Load trees
	const char* tree_frames[5] = {
		"Sprites/Tree/tree0000.png",
		"Sprites/Tree/tree0001.png",
		"Sprites/Tree/tree0002.png",
		"Sprites/Tree/tree0003.png",
		"Sprites/Tree/tree0004.png",
	};
	ss::Sprite tree{ game.get_window(), 5, tree_frames };
	ss::CollisionShape tree_cs{ tree.get_size() };
#if defined _DEBUG
	tree_cs.enable_draw(game.get_window());
#endif

	//Creating a particle emitter for the fire
	ss::ParticleEmitter ptem(game.get_window(), player.position);
	init_part(ptem, game.get_renderer());

	//Creating the ground
	ss::Texture gnd_tex(game.get_window(), ground_size);
	bool** ground_b = new bool*[(int)ground_size.x];
	long long snow_pixels = ground_size.x * ground_size.y;
	long long max_snow_pixels = snow_pixels;
	ground_cs.size = ground_size;
#if defined _DEBUG
	print_to_console(to_string(snow_pixels) + " pixels to fill");
#endif
	make_ground(ground_b, gnd_tex, snow_pixels);

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

	double snowball_timer = 0.8;
	bool first_frame = true;

	init_enemy(game);
	init_snowballs(game);

	ss::Text highscore(game.get_window(), "High Score: 0", "stfont.ttf", 8);
	highscore.color = text_color;
	highscore.position.y = 10;
	highscore.position.x = 310 - highscore.get_text().length() * 8;
	highscore.scale = 1.2;

	//Loading
	ifstream fin("game.data", ios::binary);
	if (fin.is_open()) {
		int unlkd = 0;

		fin >> unlkd >> score_record >> coins >> total_hearts >> heart_cost;

		life = 3 * total_hearts;
		unlocked = (Diff)unlkd;
		do_unlock(hardbtn, imposbtn, insnbtn);
		highscore.set_text("High Score: " + to_string(score_record));
		highscore.position.x = 310 - highscore.get_text().length() * 8;
		buy_life_text.set_text("COST:" + to_string(heart_cost));

		fin.close();
	}

	//Place trees
	for (int i = 0; i < 8; i++) {
		tree_positions[i] = rng.randv_range(ss::Vector(0), ground_size);
		tree_cs.position = tree_positions[i];
		ground_cs.push_in(tree_cs);
		tree_positions[i] = tree_cs.position;
		for (int j = 0; j < j; j++) {
			ss::CollisionShape temp_tree_cs{ tree.get_size(), tree_positions[j] };
			tree_cs.push_out(temp_tree_cs);
			tree_positions[j] = temp_tree_cs.position;
		}
		tree_states[i] = 4;
	}

	//Coins
	const char* coin_textures[6] = {
		"Sprites/Coin/coin0000.png",
		"Sprites/Coin/coin0001.png",
		"Sprites/Coin/coin0002.png",
		"Sprites/Coin/coin0003.png",
		"Sprites/Coin/coin0004.png",
		"Sprites/Coin/coin0005.png"
	};
	ss::Sprite coin_sprite{ game.get_window(), 6, coin_textures };
	coin_sprite.play(0, 5, 12, true);
	coin_sprite.position = ss::Vector(6, 24);
	string temp_coin_text0 = "X" + to_string(coins);
	ss::Text coin_text{ game.get_window(), temp_coin_text0.c_str(), "stfont.ttf", 8};
	coin_text.color = text_color;
	coin_text.position = ss::Vector(24, 28);

	uint32_t score_to_add = 0;
	double add_score_timer = 0;
	uint8_t force_spawn_enemies = rng.randi(3);
#if defined _DEBUG
	print_to_console("Force spawning " + to_string(force_spawn_enemies) + " enemies");
#endif

	uint64_t mus_pos = 0;

	//Main loop, runs every frame
	while (game.running(_dt, _rdt)) {
		if (!player_dead and first_boot) {
			if (!Mix_Playing(CH_FIRE)) Mix_PlayChannel(CH_FIRE, snd_fire, 1);
		}
		else {
			first_boot = false;
			Mix_Volume(CH_FIRE, 0);
		}
		if (!Mix_Playing(CH_MELT)) Mix_PlayChannel(CH_MELT, snd_melt, 1);
		melt_volume = ss::clamp(0, 16, melt_volume);
		if (melt_volume > 0) melt_volume = ss::lerp(melt_volume, 0, _dt / 100);
		else melt_volume = 0;
		Mix_Volume(CH_MELT, (int)melt_volume);

		game.update();
		game.clear_screen();
		gnd_tex.update();
		gnd_tex.position -= camera_offset;
		gnd_tex.draw();
		gnd_tex.position += camera_offset;

		//Trees
		for (int i = 0; i < 8; i++) {
			tree.position = tree_positions[i] - camera_offset;
			tree.frame = tree_states[i];
			tree_cs.position = tree_positions[i];
			tree_cs.push_out(player_cs);
			for (int j = 0; j < enemies; j++) {
				tree_cs.push_out(enemy[j].collision);
			}
			for (int j = 0; j < snowballs; j++) {
				if (tree_cs.is_colliding_with(snowball[j]->collision)) {
					snowball[j]->force_melt();
					if (tree_timers[i] <= 0) {
						if (tree_states[i] < 4) {
							tree_states[i]++;
							tree_timers[i] = 0.35;
						}
					}
				}
			}
			tree.draw(0);

			if (tree_timers[i] > 0) {
				tree_timers[i] -= _dt / 1000;
			}
		}

		player_process(player, ptem, game, _dt);

		//Spawn an enemy at a random interval
		if (enemies < max_enemies and snow_pixels > 50 and !in_menu or force_spawn_enemies) {
			if (spawn_timer < 0 or force_spawn_enemies) {
				if (force_spawn_enemies) force_spawn_enemies--;

				double time_change = 0.0, st_delay = 2;
				switch (difficulty) {
				case Diff::INSANE:
					time_change = 1.0 - (double)snow_pixels / max_snow_pixels;
					st_delay = 0;
					break;
				case Diff::IMPOSSIBLE:
					time_change = (1.0 - (double)snow_pixels / max_snow_pixels) * 5;
					st_delay = 1;
					break;
				case Diff::HARD:
					time_change = (2.0 - (double)snow_pixels / max_snow_pixels) * 7;
					st_delay = 1.5;
					break;
				case Diff::NORMAL:
					time_change = (3.0 - (double)snow_pixels / max_snow_pixels) * 9.5;
					break;
				default:
					break;
				}
				spawn_timer = rng.randd_range((double)difficulty / 1.5, time_change);
				ss::Vector spawn_position = player_cs.position + rng.randdir() * rng.randd_range(50, 100);
				spawn_position.x = ss::clamp(0, ground_size.x, spawn_position.x);
				spawn_position.y = ss::clamp(0, ground_size.y, spawn_position.y);

				enemy[enemies] = Enemy(spawn_position);
				if (difficulty == Diff::INSANE)
					enemy[enemies].move_speed = 100;
				enemy[enemies].snowball_throw_delay = st_delay;
				ground_cs.push_in(enemy[enemies].collision);
#if defined _DEBUG
				enemy[enemies].collision.enable_draw(game.get_window());
				print_to_console("Enemy " + to_string(enemies) + " spawned");
#endif
				enemies++;
			}
			else {
				spawn_timer -= _dt / 1000;
			}
		}
		else if (snow_pixels < 200 and !in_menu) {
			//Placeholder for win screen
			coins += ss::clamp(0, 23, heart_cost / 2);
			coin_text.set_text("X" + to_string(coins));
			force_reset = true;
			in_menu = true;
			enemies = 0;
			if (unlocked > Diff::INSANE) {
				switch (difficulty) {
				case Diff::INSANE:
					break;
				case Diff::IMPOSSIBLE:
					if (unlocked >= Diff::IMPOSSIBLE)
						unlocked = Diff::INSANE;
					break;
				case Diff::HARD:
					if (unlocked >= Diff::HARD)
						unlocked = Diff::IMPOSSIBLE;
					break;
				case Diff::NORMAL:
					if (unlocked >= Diff::NORMAL)
						unlocked = Diff::HARD;
					break;
				default:
					break;
				}
				do_unlock(hardbtn, imposbtn, insnbtn);
				save_data();
			}
		}

		//Process enemies
		int r = 0, g = 0, b = 0;
		ss::Vector p_pos;
		for (int i = 0; i < enemies; i++) {
			for (int j = 0; j < enemies; j++) {
				if (j != i) enemy[j].collision.push_out(enemy[i].collision);
			}
			enemy[i].attacking = !in_menu;
			enemy[i].target = player_cs.get_center();
			enemy[i].process(_dt, snowballs, game, snowball);
			ground_cs.push_in(enemy[i].collision);
			enemy[i].draw(camera_offset);
			p_pos = enemy[i].collision.get_center() + enemy[i].aoe_offset;
			for (int x = p_pos.x - 13; x < p_pos.x + 13; x++) {
				for (int y = p_pos.y - 13; y < p_pos.y + 13; y++) {
					if (x >= 0 and x < ground_size.x and y >= 0 and y < ground_size.y) {
						if (!ground_b[x][y]) {
							if (p_pos.distance_to(ss::Vector(x, y)) < enemy[i].aoe) {
								r = rng.randi_range(235, 255);
								g = rng.randi_range(ss::clamp(235, 255, r + 10), 255);
								gnd_tex.set_pixel(ss::Vector(x, y), r, g, 255);
								ground_b[x][y] = true;
								snow_pixels++;
							}
						}
					}
				}
			}
			if (enemy[i].is_dead()) {
#if defined _DEBUG
				print_to_console("Enemy " + to_string(i) + " killed");
#endif
				if (coins_to_draw < max_coins_to_draw) {
					coin_positions[coins_to_draw] = enemy[i].get_position() + ss::Vector(4, 4);
					coins_to_draw++;
				}
				for (int j = i; j < enemies - 1; j++) {
					enemy[j] = enemy[j + 1];
				}
				enemies--;
			}
		}

		//Verify colision of fire particles with snow on the ground and with snowmen
		for (int i = 0; i < ptem.get_num_of_particles(); i++) {
			p_pos = ptem.get_particle_position(i);
			//Set color of pixels on ground to green
			for (int x = p_pos.x - 10; x < p_pos.x + 10; x++) {
				for (int y = p_pos.y - 10; y < p_pos.y + 10; y++) {
					if (x >= 0 and x < ground_size.x and y >= 0 and y < ground_size.y) {
						if (ground_b[x][y]) {
							if (p_pos.distance_to(ss::Vector(x, y)) < 12.5) {
								melt_volume += 0.005;
								if (rng.randi() < 50) {
									r = rng.randi_range(0, 40);
									g = rng.randi_range(180, 200);
									b = rng.randi_range(0, 40);
								}
								else {
									r = ss::clamp(0, 40, r);
									g = ss::clamp(180, 200, g);
									b = ss::clamp(0, 40, b);
								}
								gnd_tex.set_pixel(ss::Vector(x, y), r, g, b);
								ground_b[x][y] = false;
								snow_pixels--;
								if (rng.randi() < 7 and !in_menu) {
									score_to_add++;
								}
							}
						}
					}
				}
			}
			//Verify collision between fire particles and snowmen
			for (int j = 0; j < enemies; j++) {
				if (enemy[j].collision.is_colliding_with(p_pos) and !in_menu) {
#if defined _DEBUG
					if (!enemy[j].is_invulnerable()) {
						print_to_console("Enemy " + to_string(j) + " damaged (" + to_string(enemy[j].get_hp()) + " hp)");
					}
#endif
					score_to_add += ss::clamp(80, 200, 200 - enemy[j].get_lifetime());
					enemy[j].damage();
				}
			}

			//Verify collision with trees
			for (int j = 0; j < 8; j++) {
				tree_cs.position = tree_positions[j];
				if (tree_cs.is_colliding_with(p_pos)) {
					if (tree_timers[j] <= 0) {
						if (tree_states[j] > 0) {
							tree_states[j]--;
							tree_timers[j] = 0.35;
						}
					}
				}
			}
		}

		if (camera_shake > 0.1) {
			camera_shake = ss::lerp(camera_shake, natural_shake, _dt / 300);
			camera_offset += rng.randdir() * camera_shake;
		}
		ptem.draw_offset = ss::Vector() - camera_offset;
		ptem.update(_dt);
		ptem.draw();
		if (player_visible and !in_menu) {
			player.draw(_dt);
		}

		for (int i = 0; i < total_hearts; i++) {
			if (life / 3 > i) {
				heart.frame = 0;
				heart.draw(_dt);
				heart.position.x += 14;
			}
			else if (life / 3 == i) {
				heart.frame = 3 - life % 3;
				if (heart.frame != 3) {
					heart.draw(_dt);
					heart.position.x += 14;
				}
			}
		}
		heart.position.x = 8;
		if (heart_timer > 0) {
			heart_timer -= _dt / 1000 * (heart.frame * 2 + 1);
		}
		else if (heart.frame != 3) {
			heart_timer = 1;
			if (heart.position.y > 7) {
				heart.position.y = 7;
			}
			else {
				heart.position.y = 8;
			}
		}
		else {
			heart.position.y = 8;
		}

		if (!select_screen) highscore.draw();

		//Coin code
		coin_sprite.draw(_dt);
		coin_text.draw();
		for (int i = 0; i < coins_to_draw; i++) {
			coin_sprite.position = coin_positions[i] - camera_offset;
			if (window_cs.is_colliding_with(coin_sprite.position)) {
				coin_sprite.draw(0);

				if (player_cs.position.distance_to(coin_positions[i]) < 15) {
					coins++;
					coin_text.set_text("X" + to_string(coins));
					for (int j = i; j < coins_to_draw; j++) {
						coin_positions[j] = coin_positions[j + 1];
					}
					coins_to_draw--;
					Mix_PlayChannel(CH_COIN, snd_coin, 0);
				}
			}
		}
		coin_sprite.position = ss::Vector(6, 24);

		if (in_menu) {
			highscore.scale = 1.0;
			process_menu(_dt, playbtn, quitbtn, normbtn, hardbtn, imposbtn, insnbtn, backbtn);
			if (!select_screen) {
				playbtn.draw();
				quitbtn.draw();
				title_text.draw();
				if (!mouse_pressed) {
					if (quitbtn.just_pressed) {
						Mix_PlayChannel(CH_UI, snd_click, 0);
						game.quit();
					}
					if (playbtn.just_pressed) {
						select_screen = true;
						Mix_PlayChannel(CH_UI, snd_click, 0);
					}
				}

				uint8_t heart_prev_frame = heart.frame;
				heart.frame = 0;
				if (buy_life.is_hovered()) {
					heart.position += ss::Vector(0, 135);
					buy_life.draw_offset = ss::Vector(0, -2);
				}
				else {
					heart.position += ss::Vector(0, 137);
					buy_life.draw_offset = ss::Vector(0);
				}
				coin_sprite.position += ss::Vector(-1, 137);
				heart.draw(0);
				buy_life.draw();
				buy_life_text.draw();
				coin_sprite.draw(0);
				heart.position -= ss::Vector(0, 135 + (!buy_life.is_hovered()) * 2);
				coin_sprite.position -= ss::Vector(-1, 137);
				heart.frame = heart_prev_frame;

				buy_life.update();
				if (buy_life.just_pressed) {
					if (coins >= heart_cost) {
						coins -= heart_cost;
						heart_cost *= 2.2;
						buy_life_text.set_text("COST:" + to_string(heart_cost));
						total_hearts++;
						life += 3;
						coin_text.set_text("X" + to_string(coins));
						save_data();
						Mix_PlayChannel(CH_UI, snd_coin, 0);
					}
					else {
						Mix_PlayChannel(CH_UI, snd_click, 0);
					}
				}
			}
			else {
				normbtn.draw();
				hardbtn.draw();
				imposbtn.draw();
				insnbtn.draw();
				backbtn.draw();
				if (!mouse_pressed) {
					if (normbtn.just_pressed) {
						prepare_game(ground_b, gnd_tex, snow_pixels, Diff::NORMAL, heart);	
						Mix_PlayChannel(CH_UI, snd_click, 0);
						delete[] enemy;
						enemy = new Enemy[max_enemies];
						enemies = 0;
					}
					if (hardbtn.just_pressed and unlocked <= Diff::HARD) {
						prepare_game(ground_b, gnd_tex, snow_pixels, Diff::HARD, heart);
						Mix_PlayChannel(CH_UI, snd_click, 0);
						max_enemies = 10;
						delete[] enemy;
						enemy = new Enemy[max_enemies];
						enemies = 0;
					}
					if (imposbtn.just_pressed and unlocked <= Diff::IMPOSSIBLE) {
						prepare_game(ground_b, gnd_tex, snow_pixels, Diff::IMPOSSIBLE, heart);
						Mix_PlayChannel(CH_UI, snd_click, 0);
						max_enemies = 20;
						delete[] enemy;
						enemy = new Enemy[max_enemies];
						enemies = 0;
					}
					if (insnbtn.just_pressed and unlocked <= Diff::INSANE) {
						prepare_game(ground_b, gnd_tex, snow_pixels, Diff::INSANE, heart);
						Mix_PlayChannel(CH_UI, snd_click, 0);
						max_enemies = 64;
						delete[] enemy;
						enemy = new Enemy[max_enemies];
						enemies = 0;
					}
					if (backbtn.just_pressed) {
						select_screen = false;
						Mix_PlayChannel(CH_UI, snd_click, 0);
					}
				}
			}

			if (Mix_PlayingMusic() and mus_pos != 3) {
				if (player_dead) {
					Mix_PlayMusic(mus_outro, 0);
				}
				else {
					Mix_PlayMusic(mus_win, 0);
				}
				mus_pos = 3;
			}
		}
		else {
			if (game.is_key_just_pressed(SDL_SCANCODE_ESCAPE)) {
				in_menu = true;
				mus_pos = 3;
			}
			if (add_score_timer <= 0 and score_to_add) {
				add_to_score(highscore, score_to_add);
				score_to_add = 0;
				add_score_timer = 0.05;
			}
			else {
				add_score_timer -= _dt / 1000;
			}
			highscore.scale = ss::lerp(highscore.scale, 1.0, _dt / 25);

			if (!Mix_PlayingMusic()) {
				switch (mus_pos) {
				case 0:
					Mix_PlayMusic(mus_intro, 0);
					mus_pos = 1;
					break;
				case 1:
					Mix_PlayMusic(mus_body1, 0);
					mus_pos = 2;
					break;
				case 2:
					Mix_PlayMusic(mus_body2, 0);
					mus_pos = 1;
					break;
				case 3:
					mus_pos = 0;
					break;
				default:
					break;
				}
			}
		}

		mouse_pressed = game.is_button_pressed(SDL_BUTTON_LEFT);

		//Update and draw snowballs
		for (int i = 0; i < snowballs; i++) {
			bool hit_player = false;
			snowball[i]->update(_dt, gnd_tex, ground_b, snow_pixels, player_cs, ground_size, hit_player);
			snowball[i]->draw(camera_offset);
			if (snowball[i]->is_melted()) {
				snowball[i]->free();
				delete[] snowball[i];
				for (int j = i; j < snowballs - 1; j++) {
					snowball[j] = snowball[j + 1];
				}
				snowballs--;
			}
			if (hit_player) {
				damage_player(player, ptem, game, snowball[i]->get_direction(), _dt, heart);
			}
		}

		//Draws the CollisionShapes and other debug info to the screen
#if defined _DEBUG
		frame++;
		if (draw_debug) {
			player_cs.position -= camera_offset;
			player_cs.draw();
			player_cs.position += camera_offset;

			for (int i = 0; i < enemies; i++) {
				enemy[i].collision.position -= camera_offset;
				enemy[i].collision.draw();
				enemy[i].collision.position += camera_offset;
			}

			for (int i = 0; i < snowballs; i++) {
				snowball[i]->collision.position -= camera_offset;
				snowball[i]->collision.draw();
				snowball[i]->collision.position += camera_offset;
			}

			for (int i = 0; i < 8; i++) {
				tree_cs.position = tree_positions[i] - camera_offset;
				tree_cs.draw();
			}

			console.draw();
			show_fps(fps, game.get_fps(), i, _rdt);
		}
		if (game.is_key_just_pressed(SDL_SCANCODE_F3)) {
			draw_debug = !draw_debug;
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_F2)) {
			if (game.target_fps == 144) {
				game.target_fps = 40;
			}
			else {
				game.target_fps = 144;
			}
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_F5)) {
			print_to_console(to_string(snow_pixels) + " snow left");
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_F10)) {
			god_mode = !god_mode;
			if (god_mode) {
				print_to_console("Toggled god mode (ON)");
			}
			else {
				print_to_console("Toggled god mode (OFF)");
			}
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
	save_data();
	return 0;
}
