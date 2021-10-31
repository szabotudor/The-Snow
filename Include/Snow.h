#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>


namespace ss {
	class Button {
		enum Type {
			Press,
			Toggle
		};
		enum BackgroundType {
			Rect,
			Empty,
			Texture
		};

		Type type;
		BackgroundType background_type;
		sf::Text text;
		sf::Font font;
		sf::RenderWindow window;

	public:
		Button(Type type = Type::Press, const char* text = "Button", const char* font = "Pixel.ttf", BackgroundType background_type = BackgroundType::Rect);
	};
}
