#include<Snow.h>

ss::Button::Button(const char* text) {
	Button::text.setPosition(0, -border / 2);
	Button::text.setString(text);
	Button::font.loadFromFile("Pixel.ttf");
	Button::text.setFont(Button::font);

	Button::text.setFillColor(sf::Color::White - sf::Color(30, 30, 30, 0));

	Button::background_type = Button::BackgroundType::Empty;
}

ss::Button::Button(sf::Color bgd_color, sf::Color border_color, sf::Color text_color, int border_thickness, const char* text, const char* font) {
	Button::text.setPosition(0, -border / 2);
	Button::rect.setPosition(-border_thickness, -border_thickness);
	Button::text.setString(text);
	Button::font.loadFromFile(font);
	Button::text.setFont(Button::font);

	Button::rect.setSize(sf::Vector2f(Button::text.getCharacterSize() * (strlen(text) - 1) + border_thickness * 2, Button::text.getCharacterSize() + border_thickness * 2));
	Button::text.setFillColor(text_color - sf::Color(30, 30, 30, 0));
	Button::rect.setFillColor(bgd_color);
	Button::rect.setOutlineColor(border_color);
	Button::rect.setOutlineThickness(border_thickness);

	Button::background_type = Button::BackgroundType::Rect;
	Button::border = border_thickness;
}

ss::Button::Button(sf::Color text_color, const char* text, const char* font) {
	Button::text.setPosition(0, -border / 2);
	Button::text.setString(text);
	Button::font.loadFromFile(font);
	Button::text.setFont(Button::font);

	Button::text.setFillColor(text_color - sf::Color(30, 30, 30, 0));

	Button::background_type = Button::BackgroundType::Empty;
}
/*
ss::Button::Button(Type type, const char* text, const char* font, BackgroundType background_type) {
	Button::type = type;
	Button::text.setString(text);
	Button::text.setPosition(0, 0);
	Button::font.loadFromFile(font);
	Button::text.setFont(Button::font);

	switch (background_type) {
	case ss::Button::Rect:
		break;
	case ss::Button::Empty:
		break;
	case ss::Button::Texture:
		break;
	}
}
*/

void ss::Button::set_text(char* text) {
	Button::text.setString(text);
	Button::rect.setSize(sf::Vector2f(Button::text.getCharacterSize() * (strlen(text) - 1) + border * 2, Button::text.getCharacterSize() + border * 2));
}

string ss::Button::get_text() {
	return text.getString();
}

void ss::Button::set_font(char* font) {
	Button::font.loadFromFile(font);
}

void ss::Button::set_position(sf::Vector2f position) {
	sf::Vector2f pos(position.x, position.y - border / 2);
	Button::position = pos;
	Button::text.setPosition(pos);
	pos.x -= border;
	pos.y -= border;
	Button::rect.setPosition(pos);
}

void ss::Button::set_position(int x, int y) {
	sf::Vector2f pos(x, y - border / 2);
	Button::position = pos;
	Button::text.setPosition(pos);
	x -= border;
	y -= border;
	Button::rect.setPosition(x, y);
}

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
