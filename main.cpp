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
	ss::Snow game("The Snow", ss::Vector(512, 288), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 60);
	ss::Text fps(game.get_window(), "00", "basic.ttf", 18);
	ss::Text debug(game.get_window(), " ", "basic.ttf", 18);
	ss::Text inst(game.get_window(), "Press RETURN to change button type\nPress SPACE to lock/unlcok fps", "basic.ttf", 18);
	inst.set_rich_text("PRESS return TO CHANGE BUTTON TYPE\nPRESS space TO LOCK/UNLOCK FPS");
	inst.position = ss::Vector(10, 220);
	debug.position = ss::Vector(10, 50);
	SDL_Color text_color, border_color, fill_color;

	text_color.r = 255; text_color.g = 255; text_color.b = 255; text_color.a = 255;
	border_color.r = 120; border_color.g = 140; border_color.b = 160; border_color.a = 255;
	fill_color.r = 60; fill_color.g = 80; fill_color.b = 100; fill_color.a = 255;

	ss::Button button(game.get_window(), fill_color, border_color, text_color, 5, "BUTTON", "basic.ttf", 36);
	button.position = ss::Vector(10, 100);
	fps.position = ss::Vector(10, 10);
	float _dt = 0.0f;
	int i = 0;

	while (game.running(_dt)) {
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

		button.update();
		debug.set_text("type: " + to_string(button.get_toggle()) + ", hovered: " + to_string(button.hovered) + ", pressed: " + to_string(button.pressed) + '\0');
		button.draw();
		debug.draw();
		inst.draw();
		game.update();
		}
	return 0;
}
