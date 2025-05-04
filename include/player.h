#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "physicsModule.h"
#include "collisionModule.h"
#include "inputManager.h"
#include "drop.h"

class Level;
class PhysicsModule;
class CollisionModule;  
struct shots;

enum class PlayerState {
    Idle,
    Walking,
    Jumping,
    Falling, 
    Crouch
};


class Player {
public:
    Player();
    Player(const std::string& texturePath);

    void update(float deltaTime, Level& level);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return sprite->getPosition(); }
    sf::Vector2f getCenter() const;

    bool isHanging = false; // Флаг зависания на плитке

    bool isWeapon = false;
    void pickupWeapon() { hasWeaponPickup = true; }
    bool hasWeapon() const { return hasWeaponPickup; }

    void win() { isWin = true; };
    bool get_isWIn(){ return isWin; };
    void kill() { isDead = true; };
    bool get_isDead() { return isDead; };
    void respawn();

    // обновление анимации
    void updateAnimation(float deltaTime);

    float getDashCooldownRemaining() const; // Возвращает оставшееся время кулдауна рывка
    sf::FloatRect getBounds() const { return sprite->getGlobalBounds(); }
    
private:
    // графика
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;

    sf::Vector2f spawnPoint = sf::Vector2f(242.f, 64.f);

    bool isDead = false;
    bool isWin = false;

    // ориентация
    bool isFacingRight = true;
    bool hasWeaponPickup = false;
    
    bool isDashing = false;   // Флаг рывка
    float dashSpeed = 2000.f;  // Скорость рывка
    float dashTime = 0.1f;    // Длительность рывка в секундах
    float dashCooldown = 1.f; // Время перезарядки рывка
    sf::Clock dashClock;      // Таймер для отслеживания времени рывка
    sf::Clock cooldownClock;  // Таймер для отслеживания перезарядки

    // движение
    float speed = 540.0f;
    float gravity = 2800.f;
    float jumpForce = -1080.f;

    // физика
    float verticalSpeed = 1000.f;
    bool isJumping = false;
    bool isOnGround = false;

    float wallJumpTimer = 0.f;
    bool canWallJump = false;

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

    sf::Texture smokeTexture;
    std::unique_ptr<sf::Sprite> smoke;
    bool showSmoke = false;
    sf::Clock smokeTimer;

    Weapon weapon;
    std::vector<Shot> shots; // Храним активные выстрелы
};
