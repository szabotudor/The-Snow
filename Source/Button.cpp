#include "..\Include\Snow.h"

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

void ss::Button::set_text(char* text) {
	Button::text.setString(text);
}

string ss::Button::get_text() {
	return text.getString();
}

void ss::Button::set_font(char* font) {
	Button::font.loadFromFile(font);
}

void ss::Button::set_position(sf::Vector2f position) {
	Button::position = position;
	Button::text.setPosition(position);
	position.x -= border;
	position.y -= border;
	Button::rect.setPosition(position);
	position.x += border;
	position.y += border;
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
