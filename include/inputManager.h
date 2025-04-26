#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

class InputManager {
public:
    InputManager() = default;
    
    // Получаем горизонтальное направление [-1, 1]
    float getHorizontalInput() const {
        float input = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            input -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            input += 1.f;
        return input;
    }
    
    // Возвращает true, если нажаты клавиши для прыжка
    bool isJumpPressed() const {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    }
};
