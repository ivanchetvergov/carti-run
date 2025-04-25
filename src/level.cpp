#include "level.h"
#include <iostream>

Level::Level(const std::string& texturePath, int width, int height)
    : width(width), height(height) {
    tileTexture = std::make_unique<sf::Texture>();
    if (!tileTexture->loadFromFile(texturePath)) {
        std::cerr << "failed to load texture: " << texturePath << std::endl;
    }

    tileSprite = std::make_unique<sf::Sprite>(*tileTexture);

    // простая генерация тайлов
    tiles.resize(height, std::vector<int>(width, 0));
    for (int y = height - 3; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles[y][x] = 1; // земля
        }
    }
}

void Level::draw(sf::RenderWindow& window) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tiles[y][x] == 1) {
                tileSprite->setPosition({static_cast<float>(x * tileSize), static_cast<float>(y * tileSize)});
                window.draw(*tileSprite);
            }
        }
    }
}

bool Level::isBlockedAt(float x, float y) const {
    int tx = static_cast<int>(x) / tileSize;
    int ty = static_cast<int>(y) / tileSize;

    if (tx < 0 || tx >= width || ty < 0 || ty >= height)
        return true; // считаем что за пределами мира — блоки

    return tiles[ty][tx] == 1;
}

void Level::loadLevel(const std::vector<std::string>& data) {
  height = data.size();
  width = data[0].size();
  tiles.resize(height, std::vector<int>(width, 0));

  for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
          tiles[y][x] = (data[y][x] == '1') ? 1 : 0;
      }
  }
}
