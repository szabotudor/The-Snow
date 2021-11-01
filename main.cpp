#include<Snow.h>

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
    sf::RenderWindow window(sf::VideoMode(256 * 2, 144 * 2), "The Snow");
    sf::Event event;
    ss::Button button(sf::Color(20, 40, 60), sf::Color(120, 150, 180), sf::Color(255, 255, 255), 5, "Button");
    button.set_position(40, 40);

    while (window.isOpen()) {
        poll_events(window, event);

        //Clears the window
        window.clear(sf::Color::Transparent);

    //Main loop
        button.draw(window);
    //End of main loop

        //Displays the window
        window.display();
    }
    return 0;
}
