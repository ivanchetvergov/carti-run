#pragma once

#include <SFML/System/Vector2.hpp>
#include "level.h"

class CollisionModule {
public:
    CollisionModule() = default;

    // Проверка столкновения с левой стороны.
    // candidateX – новое значение позиции по X при движении влево (должно быть меньше текущего pos.x)
    // pos – текущая позиция объекта,
    // spriteSize – размер объекта.
    bool checkLeftCollision(const Level &level, float candidateX, 
                            const sf::Vector2f &pos, const sf::Vector2f &spriteSize) {
        float tileSize = level.getTileSize();
        // Выбираем две внутренние точки по вертикали (на 25% и 75% высоты)
        float sampleY1 = pos.y + spriteSize.y * 0.05f;
        float sampleY2 = pos.y + spriteSize.y * 0.95f;
        int tileY1 = static_cast<int>(sampleY1 / tileSize);
        int tileY2 = static_cast<int>(sampleY2 / tileSize);
        // Для движения влево мы проверяем плитку по candidateX
        int tileX = static_cast<int>(candidateX / tileSize);
        return level.isTileSolid(tileX, tileY1) || level.isTileSolid(tileX, tileY2);
    }

    // Проверка столкновения с правой стороны.
    // candidateX – новое значение позиции по X при движении вправо (candidateX > pos.x)
    bool checkRightCollision(const Level &level, float candidateX, 
                             const sf::Vector2f &pos, const sf::Vector2f &spriteSize) {
        float tileSize = level.getTileSize();
        float sampleY1 = pos.y + spriteSize.y * 0.05f;
        float sampleY2 = pos.y + spriteSize.y * 0.95f;
        int tileY1 = static_cast<int>(sampleY1 / tileSize);
        int tileY2 = static_cast<int>(sampleY2 / tileSize);
        // Для движения вправо проверяем край объекта: candidateX + ширина спрайта
        int tileX = static_cast<int>((candidateX + spriteSize.x) / tileSize);
        return level.isTileSolid(tileX, tileY1) || level.isTileSolid(tileX, tileY2);
    }

    // Проверка столкновения сверху (при движении вверх).
    // candidateY передаётся по ссылке для возможности корректировки положения.
    // Используются горизонтальные образцы: выбираем точки на 25% и 75% ширины.
    bool checkTopCollision(const Level &level, float &candidateY,
                           const sf::Vector2f &pos, const sf::Vector2f &spriteSize, float &verticalSpeed) {
        float tileSize = level.getTileSize();
        float sampleX1 = pos.x + spriteSize.x * 0.05f;
        float sampleX2 = pos.x + spriteSize.x * 0.95f;
        int tileX1 = static_cast<int>(sampleX1 / tileSize);
        int tileX2 = static_cast<int>(sampleX2 / tileSize);
        // Для движения вверх проверяем плитку в точке candidateY
        int tileY = static_cast<int>(candidateY / tileSize);
        if (level.isTileSolid(tileX1, tileY) || level.isTileSolid(tileX2, tileY)) {
            // При столкновении сверху с потолком корректируем позицию вниз (опционально)
            candidateY = (tileY + 1) * tileSize;
            verticalSpeed = 0.f;
            return true;
        }
        return false;
    }

    // Проверка столкновения снизу (при движении вниз).
    // candidateY передаётся по ссылке для корректировки положения так, чтобы нижняя грань объекта совпадала с верхом плитки.
    bool checkBottomCollision(const Level &level, float &candidateY,
                              const sf::Vector2f &pos, const sf::Vector2f &spriteSize, 
                              float &verticalSpeed, bool &isOnGround) {
        float tileSize = level.getTileSize();
        float sampleX1 = pos.x + spriteSize.x * 0.05f;
        float sampleX2 = pos.x + spriteSize.x * 0.95f;
        int tileX1 = static_cast<int>(sampleX1 / tileSize);
        int tileX2 = static_cast<int>(sampleX2 / tileSize);
        // Для движения вниз проверяем нижнюю границу: candidateY + высота спрайта
        int tileY = static_cast<int>((candidateY + spriteSize.y) / tileSize);
        if (level.isTileSolid(tileX1, tileY) || level.isTileSolid(tileX2, tileY)) {
            // Корректируем положение так, чтобы нижняя грань объекта прилегала к верхней границе плитки.
            candidateY = tileY * tileSize - spriteSize.y;
            verticalSpeed = 0.f;
            isOnGround = true;
            return true;
        }
        return false;
    }
};
