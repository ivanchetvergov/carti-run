#include "level.h"
#include <iostream>

Level::Level(const std::string& texturePath, int width, int height)
    : width(width), height(height) {
    tileTexture = std::make_unique<sf::Texture>();
    if (!tileTexture->loadFromFile(texturePath)) {
        std::cerr << "failed to load texture: " << texturePath << std::endl;
    }

    tileSprite = std::make_unique<sf::Sprite>(*tileTexture);

    tiles.resize(height);
    for (int y = 0; y < height; ++y) {
        tiles[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            Tile tile;
            tile.tileSprite = std::make_unique<sf::Sprite>(*tileTexture);
            sf::Vector2f pos(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize));
            tile.tileSprite->setPosition(pos);
            tile.isSolid = (y >= height - 3); // земля снизу
            tiles[y].emplace_back(std::move(tile));
        }
    }
}

void Level::draw(sf::RenderWindow& window) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tiles[y][x].tileSprite) {
                window.draw(*tiles[y][x].tileSprite);
            }
        }
    }
}

bool Level::isBlockedAt(float x, float y) const {
    int tx = static_cast<int>(x) / tileSize;
    int ty = static_cast<int>(y) / tileSize;

    if (tx < 0 || tx >= width || ty < 0 || ty >= height)
        return true;

    return tiles[ty][tx].isSolid;
}

void Level::loadLevel(const std::vector<std::string>& data) {
    height = data.size();
    width = data[0].size();
    tiles.clear();
    tiles.resize(height);

    for (int y = 0; y < height; ++y) {
        tiles[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            Tile tile;
            tile.tileSprite = std::make_unique<sf::Sprite>(*tileTexture);
            sf::Vector2f pos(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize));
            tile.tileSprite->setPosition(pos);
            tile.isSolid = (data[y][x] == '1');
            tiles[y].emplace_back(std::move(tile));
        }
    }
}
