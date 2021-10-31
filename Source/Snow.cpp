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
