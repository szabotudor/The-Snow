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
	ss::Text fps(game.get_renderer(), "00", "color basic.ttf", 18);
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
		game.update();
	}
	return 0;
}
