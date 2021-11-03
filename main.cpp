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
	ss::Snow game("The Snow", ss::Vector(512, 288), SDL_WINDOW_SHOWN, 60);
	ss::Text fps(game.get_renderer(), "00", "basic.ttf", 18);
	SDL_Color text_color, border_color, fill_color;

	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
	text_color.a = 255;

	border_color.r = 120;
	border_color.g = 140;
	border_color.b = 160;
	border_color.a = 255;

	fill_color.r = 60;
	fill_color.g = 80;
	fill_color.b = 100;
	fill_color.a = 255;

	ss::Button button(game.get_renderer(), fill_color, border_color, text_color, 2, "Button");
	fps.position = ss::Vector(10, 10);
	float _dt = 0.0f;
	int i = 0;

	while (game.running(_dt)) {
		game.clear_screen();
		show_fps(fps, game.get_fps(), i);
		if (game.is_key_just_pressed(SDL_SCANCODE_SPACE)) {
			cout << "Space just pressed" << endl;
			if (game.get_target_framerate() == 60) {
				game.set_target_framerate(144);
			}
			else if (game.get_target_framerate() == 144) {
				game.set_target_framerate(60);
			}
		}
		else if (game.is_key_just_released(SDL_SCANCODE_SPACE)) {
			cout << "Space just released" << endl;
		}
		button.draw();
		game.update();
	}
	return 0;
}
