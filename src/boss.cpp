#include "../include/boss.h"

Boss::Boss(std::shared_ptr<sf::Texture> texture, sf::Vector2f position, float speed)
    : Mob(texture, position, speed), health(20), shootCooldown(1.5f) {}

void Boss::update(float deltaTime, Player& player, const Level& level) {
    Mob::update(deltaTime, player, level); // обычное поведение моба (движение и т.д.)

    if (shootTimer.getElapsedTime().asSeconds() > shootCooldown) {
        shootAt(player.getCenter()); // или getPosition()
        shootTimer.restart();
    }

    // обновление снарядов
    for (auto& p : projectiles)
        p.update(deltaTime);

    // можно добавить проверку на попадание по игроку
}

void Boss::draw(sf::RenderWindow& window) {
    Mob::draw(window);
    for (auto& p : projectiles)
        p.draw(window);
}

void Boss::shootAt(const sf::Vector2f& target) {
    sf::Vector2f origin = sprite->getPosition();
    sf::Vector2f dir = target - origin;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    dir /= length;

    projectiles.emplace_back(origin, dir * 300.f, 3); // скорость и урон
}

void Boss::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Boss::isDead() const {
    return health <= 0;
}
