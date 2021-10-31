#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

using namespace std;

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
		//Sets the button text to the given parameter
		void set_text(char* text);
		//Returns the text set on the button
		string get_text();
		//Loads specified font and uses it on the button
		void set_font(char* font);
	};
}
