#pragma once

#include <SFML/System/Vector2.hpp>

class PhysicsModule {
public:
    PhysicsModule(float gravity, float jumpForce)
        : gravity(gravity), jumpForce(jumpForce) {}

    // интеграция вертикальной скорости
    sf::Vector2f integrate(float dt, float horizontalSpeed, float& verticalSpeed) {
        verticalSpeed += gravity * dt;
        return sf::Vector2f(horizontalSpeed, verticalSpeed);
    }

    // получить силу прыжка
    float getJumpForce() const {
        return jumpForce;
    }
    
    float getGravity() const { return gravity; }


private:
    float gravity;
    float jumpForce;
};
