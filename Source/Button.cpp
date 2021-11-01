#include<Snow.h>

ss::Button::Button(const char* text) {
	Button::text.setPosition(0, 0);
	Button::text.setString(text);
	Button::font.loadFromFile("Pixel.ttf");
	Button::text.setFont(Button::font);

	Button::text.setFillColor(sf::Color::White);

	Button::background_type = Button::Empty;
}

ss::Button::Button(sf::Color bgd_color, sf::Color border_color, sf::Color text_color, int border_thickness, const char* text, const char* font) {
	Button::text.setPosition(0, 0);
	Button::rect.setPosition(-border_thickness, -border_thickness);
	Button::text.setString(text);
	Button::font.loadFromFile(font);
	Button::text.setFont(Button::font);

	Button::rect.setSize(sf::Vector2f(Button::text.getCharacterSize() * (strlen(text) - 1) + border_thickness * 2, Button::text.getCharacterSize() + border_thickness * 2));
	Button::text.setFillColor(text_color);
	Button::rect.setFillColor(bgd_color);
	Button::rect.setOutlineColor(border_color);
	Button::rect.setOutlineThickness(border_thickness);

	Button::background_type = Button::Rect;
	Button::border = border_thickness;
}

ss::Button::Button(sf::Color text_color, const char* text, const char* font) {
	Button::text.setPosition(0, 0);
	Button::text.setString(text);
	Button::font.loadFromFile(font);
	Button::text.setFont(Button::font);

	Button::text.setFillColor(text_color);

	Button::background_type = Button::Empty;
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
	sf::Vector2f pos(position.x, position.y);
	Button::position = pos;
	Button::text.setPosition(pos);
	pos.x -= border;
	pos.y -= border;
	Button::rect.setPosition(pos);
}

void ss::Button::set_position(int x, int y) {
	sf::Vector2f pos(x, y);
	Button::position = pos;
	Button::text.setPosition(pos);
	x -= border;
	y -= border;
	Button::rect.setPosition(x, y);
}

void ss::Button::draw(sf::RenderWindow& window) {
	switch (background_type) {
	case ss::Button::Rect:
		window.draw(rect);
		window.draw(text);
	case ss::Button::Empty:
		window.draw(text);
	case ss::Button::Texture:
		break;
		//window.draw(texture);
	}
}

void ss::Button::update(sf::RenderWindow& window) {
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	
	if (just_released) {
		just_released = false;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (pos.x > position.x and pos.x < position.x + rect.getSize().x and pos.y > position.y and pos.y < position.y + rect.getSize().y) {
			switch (type) {
			case ss::Button::Press:
				if (!pressed) {
					pressed = true;
					just_pressed = true;
				}
				else {
					just_pressed = false;
				}
			case ss::Button::Toggle:
				break;
			}
		}
	}
	else if (pressed) {
		just_released = true;
		pressed = false;
	}
}
