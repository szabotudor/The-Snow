#pragma once
#include<thread>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

using namespace std;

namespace ss {
	//Funtion that runs in a background thread (not meant for use by user)
	void _process();
	//Stops all processes and background threads running in the helper library
	void quit();
	
	class Button {
	public:
		bool pressed = false;
		bool just_pressed = false;
		bool just_released = false;
		enum Type {
			Press,
			Toggle
		};
		enum BackgroundType {
			Rect,
			Empty,
			Texture
		};

	private:
		unsigned int border = 0;
		Type type;
		BackgroundType background_type;
		sf::Text text;
		sf::RectangleShape rect;
		sf::Texture texture;
		sf::Font font;
		sf::Vector2f position;
		
	public:
		Button(const char* text = "Button");
		Button(sf::Color bgd_color, sf::Color border_color, sf::Color text_color, int border_thickness, const char* text, const char* font = "Pixel.ttf");
		Button(sf::Color text_color, const char* text, const char* font = "Pixel.ttf");
		//Button(Type type = Type::Press, const char* text = "Button", const char* font = "Pixel.ttf", BackgroundType background_type = BackgroundType::Rect);
		
		//Sets the button text to the given parameter
		void set_text(char* text);
		//Returns the text set on the button
		string get_text();
		//Loads specified font and uses it on the button
		void set_font(char* font);
		//Sets the position of the button (by layer)
		void set_position(sf::Vector2f position);
		//Sets the position of the button (by layer)
		void set_position(int x, int y);
		//Sets the window the button should be drawn to
		void draw(sf::RenderWindow& window);
		//Updates the button state, based on mouse location and clicks
		void update(sf::RenderWindow& window);
	};
}
