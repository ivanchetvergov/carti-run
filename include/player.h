#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "physicsModule.h"
#include "collisionModule.h"
#include "inputManager.h"

class Level;
class PhysicsModule;
class CollisionModule;  

enum class PlayerState {
    Idle,
    Walking,
    Jumping,
    Falling
};


class Player {
public:
    Player();
    Player(const std::string& texturePath);

    void update(float deltaTime, const Level& level);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return sprite->getPosition(); };

    void kill() { isDead = true; };
    bool get_isDead() { return isDead; }
    void respawn();

    // обновление анимации
    void updateAnimation(float deltaTime);

private:
    // графика
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;

    bool isDead = false;

    // ориентация
    bool isFacingRight = true;

    // движение
    float speed = 540.0f;
    float gravity = 2800.f;
    float jumpForce = -1080.f;

    // физика
    float verticalSpeed = 1000.f;
    bool isJumping = false;
    bool isOnGround = false;

    // анимация
    sf::IntRect currentFrame;
    float animationTimer = 0.f;
    float frameDuration = 0.1f;
    int currentFrameIndex = 0;
    int walkFrameCount = 8; // количество кадров в анимации ходьбы

    static constexpr int frameWidth = 23;
    static constexpr int frameHeight = 32;

    // кадры анимации
    int jumpFrameX =  frameWidth * 8; 
    int jumpFrameY = 0; 

    int fallingFrameX = frameWidth * 9;
    int fallingFrameY = 0; 


    // текущее состояние
    PlayerState state = PlayerState::Idle;

    // модули
    std::unique_ptr<PhysicsModule> physicsModule;
    std::unique_ptr<CollisionModule> collisionModule;

};
