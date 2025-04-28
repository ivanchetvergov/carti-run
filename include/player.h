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

    sf::Vector2f getPosition() const { return sprite->getPosition(); }


private:
    // Графические данные
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;

    bool isFacingRight = true;

    // Основные параметры движения
    float speed = 520.0f;
    float gravity = 2800.f;
    float jumpForce = -2280.f;

    // Текущее состояние физики
    float verticalSpeed = 1000.f;
    bool isJumping = false;
    bool isOnGround = false;

    // Анимация
    sf::IntRect currentFrame;                // Текущий кадр анимации
    float animationTimer = 0.f;              // Таймер для смены кадров
    float frameDuration = 0.1f;              // Длительность одного кадра
    int currentFrameIndex = 0;               // Индекс текущего кадра
    int numFrames = 8;                       // Количество кадров анимации
    const int frameWidth = 23;               // Ширина одного кадра (раскадровка)
    const int frameHeight = 35;              // Высота одного кадра (раскадровка)

    // Модули физики и столкновений
    std::unique_ptr<PhysicsModule> physicsModule;
    std::unique_ptr<CollisionModule> collisionModule;
};
