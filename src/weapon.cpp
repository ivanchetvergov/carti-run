#include "../include/drop.h"

void Weapon::update(float dt) {
    if (fireCooldown > 0.f)
        fireCooldown -= dt;
}

void Weapon::fire(sf::Vector2f position, bool facingRight, std::vector<Shot>& shots) {
    if (fireCooldown <= 0.f) {
        fireCooldown = 1.f / fireRate;

        sf::RectangleShape shape(sf::Vector2f(12.f, 4.f));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);

        sf::Vector2f velocity = facingRight ? sf::Vector2f(450.f, 0.f) : sf::Vector2f(-450.f, 0.f);

        shots.push_back({ shape, velocity, damage });

        energy += 10.f;
        if (energy > maxEnergy) energy = maxEnergy;
    }
}

void Weapon::fireUltimate(sf::Vector2f position, bool facingRight, std::vector<Shot>& shots) {
    if (canUseUltimate()) {
        energy = 0.f;

        sf::RectangleShape shape(sf::Vector2f(20.f, 8.f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);

        sf::Vector2f velocity = facingRight ? sf::Vector2f(300.f, 0.f) : sf::Vector2f(-300.f, 0.f);

        shots.push_back({ shape, velocity, damage * 5 });
    }
}
