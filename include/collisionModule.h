#pragma once

#include <SFML/System/Vector2.hpp>
#include "level.h"

class CollisionModule {
public:
    CollisionModule() = default;

    bool checkLeftCollision(const Level &level, float candidateX, const sf::Vector2f &pos, const sf::Vector2f &spriteSize) {
        float tileSize = level.getTileSize();
        float sampleY1 = pos.y + spriteSize.y * 0.05f;
        float sampleY2 = pos.y + spriteSize.y * 0.95f;
        int tileY1 = static_cast<int>(sampleY1 / tileSize);
        int tileY2 = static_cast<int>(sampleY2 / tileSize);
        int tileX = static_cast<int>(candidateX / tileSize);
        return level.isTileSolid(tileX, tileY1) || level.isTileSolid(tileX, tileY2);
    }

    bool checkRightCollision(const Level &level, float candidateX, const sf::Vector2f &pos, const sf::Vector2f &spriteSize) {
        float tileSize = level.getTileSize();
        float sampleY1 = pos.y + spriteSize.y * 0.05f;
        float sampleY2 = pos.y + spriteSize.y * 0.95f;
        int tileY1 = static_cast<int>(sampleY1 / tileSize);
        int tileY2 = static_cast<int>(sampleY2 / tileSize);
        int tileX = static_cast<int>((candidateX + spriteSize.x) / tileSize);
        return level.isTileSolid(tileX, tileY1) || level.isTileSolid(tileX, tileY2);
    }

    bool checkTopCollision(const Level &level, float &candidateY, const sf::Vector2f &pos, const sf::Vector2f &spriteSize, float &verticalSpeed) {
        float tileSize = level.getTileSize();
        float sampleX1 = pos.x + spriteSize.x * 0.1f;
        float sampleX2 = pos.x + spriteSize.x * 0.9f;
        int tileX1 = static_cast<int>(sampleX1 / tileSize);
        int tileX2 = static_cast<int>(sampleX2 / tileSize);
        int tileY = static_cast<int>(candidateY / tileSize);
        if (level.isTileSolid(tileX1, tileY) || level.isTileSolid(tileX2, tileY)) {
            candidateY = (tileY + 1) * tileSize;
            verticalSpeed = 0.f;
            return true;
        }
        return false;
    }

    bool checkBottomCollision(const Level &level, float &candidateY, const sf::Vector2f &pos, const sf::Vector2f &spriteSize, float &verticalSpeed, bool &isOnGround) {
        float tileSize = level.getTileSize();
        float sampleX1 = pos.x + spriteSize.x * 0.1f;
        float sampleX2 = pos.x + spriteSize.x * 0.9f;
        int tileX1 = static_cast<int>(sampleX1 / tileSize);
        int tileX2 = static_cast<int>(sampleX2 / tileSize);
        int tileY = static_cast<int>((candidateY + spriteSize.y) / tileSize);
        if (level.isTileSolid(tileX1, tileY) || level.isTileSolid(tileX2, tileY)) {
            candidateY = tileY * tileSize - spriteSize.y;
            verticalSpeed = 0.f;
            isOnGround = true;
            return true;
        }
        return false;
    }
};
