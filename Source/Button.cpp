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
