#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

using namespace std;


void poll_events(sf::RenderWindow &window, sf::Event &event) {
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(256, 144), "The Snow", sf::Style::Fullscreen);
    sf::Event event;

    while (window.isOpen()) {
        poll_events(window, event);

        //Clears the window
        window.clear(sf::Color::Transparent);

    //Main loop
        
    //End of main loop

        //Displays the window
        window.display();
    }
    return 0;
}
