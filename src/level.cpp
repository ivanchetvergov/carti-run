#include "../include/level.h"

#include <iostream>
#include "level.h"

// Устанавливаем текстуру для указанного типа плитки
void Level::setTexture(TileType type, const std::shared_ptr<sf::Texture>& texture) {
    textures[type] = texture;
}

std::shared_ptr<sf::Texture> Level::getTexture(TileType type) const {
    auto it = textures.find(type);
    return (it != textures.end()) ? it->second : nullptr;
}

void Level::setCloudTexture(const std::shared_ptr<sf::Texture>& texture) {
    cloudsTexture = texture; 
}

void Level::loadLevel(const std::vector<std::string>& data) {
    height = data.size();                 // Устанавливаем высоту уровня
    width = data.empty() ? 0 : data[0].size();  // Проверка на пустой уровень
    tiles.clear();
    tiles.resize(height);

    for (int y = 0; y < height; ++y) {
        tiles[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            Tile tile;
            sf::Vector2f pos(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize));

            // Определяем тип плитки
            TileType tileType = TileType::Empty;
            switch (data[y][x]) {
                case '0': tileType = TileType::Background; break;
                case '1': tileType = TileType::Wall; tile.isSolid = true; break;
                case '_': tileType = TileType::Empty; break;
                case '!': tileType = TileType::Spikes; tile.isKilling = true; break;
                case '*': tileType = TileType::Bouncy; tile.isSolid = true; tile.isBouncy = true; break;
                case '#': tileType = TileType::Hangable; tile.isSolid = true; tile.isHangable = true; break;
                case 'w': tileType = TileType::WeaponPickup; tile.isWeapon = true; break;
                default: tileType = TileType::Empty; break;
            }

            // Назначаем текстуру и создаём спрайт
            auto texture = getTexture(tileType);
            if (!texture) {
                std::cerr << "Ошибка: текстура для типа " << static_cast<int>(tileType) << " отсутствует!\n";
                continue;  // Пропускаем плитку, если текстура отсутствует
            }
            
            if (texture) {
                tile.tileSprite = std::make_unique<sf::Sprite>(*texture);
                tile.tileSprite->setPosition(pos);
                tile.tileSprite->setScale(sf::Vector2f(
                    tileSize / texture->getSize().x,
                    tileSize / texture->getSize().y
                ));
            } else {
                std::cerr << "Ошибка: текстура для типа " << static_cast<int>(tileType) << " отсутствует!\n";
            }

            tile.type = tileType;
            tiles[y].emplace_back(std::move(tile)); // Добавляем плитку в строку
        }
    }
}

/* Render the level onto the window. */
void Level::draw(sf::RenderWindow& window) {
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            window.draw(*tile.tileSprite); // draw the tile's sprite
        }
    }
}

void Level::setDimensions(int width, int height) {
    this->width = width;      // сохраняем ширину уровня
    this->height = height;    // сохраняем высоту уровня
    tiles.resize(height);     // изменяем высоту массива плиток
}

bool Level::isWall(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height) return false;
    return tiles[tileY][tileX].isSolid;  // Предполагаем, что стены обозначены '1'
}

bool Level::isBouncy(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height) return false;
    return tiles[tileY][tileX].isBouncy;
}

bool Level::isHangable(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height) return false;
    return tiles[tileY][tileX].isHangable;
}

bool Level::isTileSolid(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x].isSolid; // return solidity of the tile
    }
    return false; // if coordinates are out of bounds, consider it impassable
}

bool Level::isKilling(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x].isKilling; // return solidity of the tile
    }
    return false; // if coordinates are out of bounds, consider it impassable
}

bool Level::isWeaponOrb(int tileX, int tileY) const{
    if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height) {
        return tiles[tileY][tileX].isWeapon; // return solidity of the tile
    }
    return false; // if coordinates are out of bounds, consider it impassable
}

void Level::removeTile(int x, int y){
    auto texture = getTexture(TileType::Empty);
    tiles[y][x].tileSprite = std::make_unique<sf::Sprite>(*texture);
}
