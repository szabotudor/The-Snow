#include<Snow.h>



void ss::Button::draw(sf::RenderWindow& window) {
	switch (background_type) {
	case ss::Button::BackgroundType::Rect:
		window.draw(rect);
		window.draw(text);
	case ss::Button::BackgroundType::Empty:
		window.draw(text);
	case ss::Button::BackgroundType::Texture:
		break;
		//window.draw(texture);
	}
}

bool is_hovered(int x, int y, int bx, int by, int w, int h) {
	return x > bx and x < (bx + w) and y > by and y < (by + h);
}

void ss::Button::update(sf::RenderWindow& window) {
	if (disabled) {
		return;
	}
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	if (!hovered) {
		hovered = is_hovered(pos.x, pos.y, position.x, position.y, rect.getSize().x, rect.getSize().y);
		if (hovered) {
			text.setFillColor(text.getFillColor() + sf::Color(30, 30, 30, 0));
			text.setPosition(text.getPosition() - sf::Vector2f(0, border / 2));
			rect.setPosition(rect.getPosition() - sf::Vector2f(0, border / 2));
		}
	}
	else {
		hovered = is_hovered(pos.x, pos.y, position.x, position.y, rect.getSize().x, rect.getSize().y);
		if (!hovered) {
			text.setFillColor(text.getFillColor() - sf::Color(30, 30, 30, 0));
			text.setPosition(text.getPosition() + sf::Vector2f(0, border / 2));
			rect.setPosition(rect.getPosition() + sf::Vector2f(0, border / 2));
		}
	}
	
	if (just_released) {
		just_released = false;
	}
	//Check wether Left Click is pressed
	if (sf::Mouse::isButtonPressed(button)) {
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
