#pragma once 

#include <SFML/System/Vector2.hpp>

class PhysicsModule {
public:
    PhysicsModule(float gravity, float jumpForce)
        : gravity(gravity), jumpForce(jumpForce) {}

    // Интегрируем вертикальную скорость с учетом гравитации.
    // horizontalSpeed передаётся отдельно (устанавливается из ввода).
    sf::Vector2f integrate(float dt, float horizontalSpeed, float &verticalSpeed) {
        // Вертикальную скорость обновляем с гравитацией
        verticalSpeed += gravity * dt;
        return sf::Vector2f(horizontalSpeed, verticalSpeed);
    }

    // При прыжке вертикальная скорость устанавливается в jumpForce (обычно отрицательное значение)
    float getJumpForce() const {
        return jumpForce;
    }
    
private:
    float gravity;
    float jumpForce;
};

