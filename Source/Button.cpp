#include "..\Include\Snow.h"

ss::Button::Button(Type type = Type::Press, const char* text = "Button", const char* font = "Pixel.ttf", BackgroundType background_type = BackgroundType::Rect) {
	Button::type = type;
	Button::text.setString(text);
	Button::font.loadFromFile(font);

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
