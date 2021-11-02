#include<Snow.h>


int main(int argc, char* args[]) {
	ss::Snow game("The Snow", ss::Vector(512, 288), SDL_WINDOW_SHOWN, -1);
	ss::Text text(game.get_renderer(), "Test String", "bulkypix.ttf", 20);
	float _dt = 0.0f;
	int avg_fps[60] = { 0 }, i = 0;

	while (game.running(_dt)) {
		game.clear_screen();
		if (i < 60) {
			avg_fps[i] = game.get_fps();
			i++;
		}
		else {
			i = 0;
			int sum = 0;
			for (int j = 0; j < 60; j++) {
				sum += avg_fps[j];
			}
			sum /= 60;

			//text.set_text("FPS: " + to_string(sum) + " | Aprox frame delta(ms): " + to_string(_dt));
		}
		text.set_text("FPS: " + to_string(game.get_fps()) + " | Aprox frame delta(ms): " + to_string(_dt));
		text.draw();
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
