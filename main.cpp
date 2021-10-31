#include<SFML/Graphics.hpp>

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    sf::RectangleShape rect(sf::Vector2f(20, 80));
    shape.setFillColor(sf::Color::Green);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(3);
    rect.setFillColor(sf::Color(30, 100, 240));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(rect);
        window.display();
    }

    return 0;
}
