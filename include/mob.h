#pragma once

#include "player.h"

class Mob {
public:
    Mob(std::shared_ptr<sf::Texture> texture, sf::Vector2f position, float speed);
    virtual ~Mob() = default;

    virtual void update(float deltaTime, Player &player, const Level &level);
    virtual void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite->getGlobalBounds(); };

protected:
    std::unique_ptr<sf::Sprite> sprite;
    std::shared_ptr<sf::Texture> texture;
    float speed;
    bool movingRight;
};



