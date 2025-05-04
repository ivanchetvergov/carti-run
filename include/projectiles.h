#pragma once
#include <SFML/Graphics.hpp>

struct Projectile {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    int damage;

    Projectile(sf::Vector2f size, sf::Color color, sf::Vector2f pos, sf::Vector2f vel, int dmg)
        : velocity(vel), damage(dmg)
    {
        shape.setSize(size);
        shape.setFillColor(color);
        shape.setPosition(pos);
    }

    Projectile(sf::Vector2f pos, sf::Vector2f vel, int dmg)
    : velocity(vel), damage(dmg)
    {
    shape.setSize({12.f, 6.f});
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(pos);
    }

    void update(float dt) {
        shape.move(velocity * dt);
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
    
};
