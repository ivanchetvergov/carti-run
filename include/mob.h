#pragma once

#include "player.h"



class Mob {
public:
    Mob(std::shared_ptr<sf::Texture> texture, sf::Vector2f position, float speed);
    void update(float deltaTime, Player &player, const Level &level);
    void update(float deltaTime, Player &player); // Передаем игрока
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite->getGlobalBounds(); };

private:
    std::unique_ptr<sf::Sprite> sprite;
    std::shared_ptr<sf::Texture> texture;
    float speed;
    bool movingRight;
};

