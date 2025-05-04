#pragma once

#include "mob.h"
#include "projectiles.h"

class Boss : public Mob {
public:
    Boss(std::shared_ptr<sf::Texture> texture, sf::Vector2f position, float speed);

    void update(float deltaTime, Player& player, const Level& level) override;
    void draw(sf::RenderWindow& window) override;

    void takeDamage(int amount);
    bool isDead() const;

private:
    int health;
    float shootCooldown;
    sf::Clock shootTimer;
    std::vector<Projectile> projectiles;

    void shootAt(const sf::Vector2f& target);
};
