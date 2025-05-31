#include "MyMainTest.h"

int myMainTest() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML Test");
    sf::Texture texture;
    if (!texture.loadFromFile("resources/testSprite.png")) {
        std::cerr << "Failed to load texture\n";
        return 1;
    }
    std::cout << "Texture size: " << texture.getSize().x << "x" << texture.getSize().y << "\n";
    sf::Sprite sprite(texture);
    sprite.setPosition({ 100, 100 });
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    return 0;
}