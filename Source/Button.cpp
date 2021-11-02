#include<Snow.h>



void ss::Button::draw(SDL_Window& window) {
	switch (background_type) {
	case ss::Button::BackgroundType::Rect:
		break;
	case ss::Button::BackgroundType::Empty:
		break;
	case ss::Button::BackgroundType::Texture:
		break;
	}
}

bool is_hovered(int x, int y, int bx, int by, int w, int h) {
	return x > bx and x < (bx + w) and y > by and y < (by + h);
}

void ss::Button::update(SDL_Window& window) {
	if (disabled) {
		return;
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	if (!hovered) {
		hovered = is_hovered(x, y, position.x, position.y, rect->w, rect->h);
		if (hovered) {
			/*
			text.setFillColor(text.getFillColor() + sf::Color(30, 30, 30, 0));
			text.setPosition(text.getPosition() - sf::Vector2f(0, border / 2));
			rect.setPosition(rect.getPosition() - sf::Vector2f(0, border / 2));
			*/
		}
	}
	else {
		hovered = is_hovered(x, y, position.x, position.y, rect->w, rect->h);
		if (!hovered) {
			/*
			text.setFillColor(text.getFillColor() - sf::Color(30, 30, 30, 0));
			text.setPosition(text.getPosition() + sf::Vector2f(0, border / 2));
			rect.setPosition(rect.getPosition() + sf::Vector2f(0, border / 2));
			*/
		}
	}
	
	if (just_released) {
		just_released = false;
	}
	//Check wether Left Click is pressed
	if (SDL_GetMouseState(&x, &y) == mouse_button) {
		//Checks wether the mouse cursor is inside the button
		if (hovered) {
			switch (type) {
			case ss::Button::Type::Press:
				if (!pressed) {
					pressed = true;
					just_pressed = true;
					state = State::ON;
				}
				else {
					just_pressed = false;
					state = State::OFF;
				}
			case ss::Button::Type::Toggle:
				if (!pressed) {
					pressed = true;
					if (state == State::OFF) {
						state = State::ON;
					}
					else if (state == State::ON) {
						state = State::OFF;
					}
				}
			}
		}
	}
	else if (pressed) {
		just_released = true;
		pressed = false;
	}
	else {

	}
}

void ss::Button::set_toggle(bool ON) {
	if (ON) {
		type = Button::Type::Toggle;
	}
	else {
		type = Button::Type::Press;
	}
}
