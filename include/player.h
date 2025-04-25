#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Level; // forward declaration

class Player {
public:
    Player();
    Player(const std::string& texturePath);

    void handleInput(float deltaTime, const Level& level);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    float speed = 200.0f;
};
