#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "physicsModule.h"
#include "collisionModule.h"
#include "inputManager.h"

class Level; // forward declaration для уровня
class PhysicsModule;    // forward declaration для физического модуля
class CollisionModule;  // forward declaration для модуля столкновений


class Player {
public:
    Player();
    Player(const std::string& texturePath);

    // Метод, обновляющий состояние игрока (сбор ввода, физика и проверки столкновений)
    void update(float deltaTime, const Level& level);
    void draw(sf::RenderWindow& window);

private:
    // Графические данные
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;

    // Основные параметры движения
    float speed = 400.0f;      
    float gravity = 2800.f;            
    float jumpForce = -2280.f; 

    // Текущее состояние физики
    float verticalSpeed = 10.f;  
    bool isJumping = false;          
    bool isOnGround = false;

    // Модули, отвечающие за интеграцию физики и проверку столкновений
    std::unique_ptr<PhysicsModule> physicsModule;
    std::unique_ptr<CollisionModule> collisionModule;
};
