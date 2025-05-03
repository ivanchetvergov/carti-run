#pragma once

#include "player.h"

class Mob {
public:
    Mob(const std::string& texturePath, sf::Vector2f position, float speed);
    void update(float deltaTime, Player &player, const Level &level);
    void update(float deltaTime, Player &player); // Передаем игрока
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite->getGlobalBounds(); };
    
private:
    std::unique_ptr<sf::Sprite> sprite;
    sf::Texture texture;
    float speed;
    bool movingRight;
};

