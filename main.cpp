#include<Snow.h>


int avg_fps[60];


void show_fps(ss::Text& text, unsigned int fps, int &i) {
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

		text.set_text(to_string(sum));
	}
	text.draw();
}


int main(int argc, char* args[]) {
	ss::iVector mpos;
	ss::Snow game("The Snow", ss::iVector(256, 144), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 60);
	ss::Text fps(game.get_window(), "", "basic.ttf", 9);
	ss::Text debug(game.get_window(), "", "basic.ttf", 9);
	ss::Text inst(game.get_window(), "PRESS crtl + f TO TOGGLE FULLSCREEN\nPRESS return TO CHANGE BUTTON TYPE\nPRESS space TO LOCK/UNLOCK FPS", "basic.ttf", 9);
	ss::Text mousepos(game.get_window(), "", "basic.ttf", 9);
	mousepos.position = ss::Vector(100, 55);
	fps.position = ss::Vector(5, 5);
	debug.position = ss::Vector(5, 25);
	inst.position = ss::Vector(5, 100);
	SDL_Color text_color, border_color, fill_color;

	text_color.r = 255; text_color.g = 255; text_color.b = 255; text_color.a = 255;
	border_color.r = 120; border_color.g = 140; border_color.b = 160; border_color.a = 255;
	fill_color.r = 60; fill_color.g = 80; fill_color.b = 100; fill_color.a = 255;

	ss::Button button(game.get_window(), fill_color, border_color, text_color, 3, "BUTTON", "basic.ttf", 18);
	button.position = ss::Vector(5, 50);
	float _dt = 0.0f;
	int i = 0;

	SDL_Event* ev;

	while (game.running(_dt)) {
		game.update();
		game.clear_screen();

		show_fps(fps, game.get_fps(), i);

		if (game.is_key_just_pressed(SDL_SCANCODE_SPACE)) {
			if (game.get_target_framerate() == 60) {
				game.set_target_framerate(0);
			}
			else if (game.get_target_framerate() == 0) {
				game.set_target_framerate(60);
			}
		}

		if (game.is_key_just_pressed(SDL_SCANCODE_RETURN)) {
			button.set_toggle(!button.get_toggle());
		}

		for (int i = 0; i < game.get_num_events(); i++) {
			if (game.events[i].type == SDL_QUIT) {
				game.quit();
			}
			if (game.is_key_pressed(SDL_SCANCODE_LCTRL) and game.is_key_just_pressed(SDL_SCANCODE_F)) {
				game.set_fullscreen(!game.get_fullscreen());
			}
		}

		mpos = game.get_mouse_position();
		mousepos.set_text(to_string(mpos.x) + " " + to_string(mpos.y));

		button.update();
		debug.set_text("type: " + to_string(button.get_toggle()) + ", hovered: " + to_string(button.hovered) + ", pressed: " + to_string(button.pressed) + '\0');
		button.draw();
		debug.draw();
		inst.draw();
		mousepos.draw();
		}
	return 0;
}
