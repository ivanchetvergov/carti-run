#include "level.h"

/* Set the wall texture. */
void Level::setWallTexture(const std::shared_ptr<sf::Texture>& texture) {
    wallTexture = texture; // assign wall texture
}

/* Set the background texture. */
void Level::setBackgroundTexture(const std::shared_ptr<sf::Texture>& texture) {
    backgroundTexture = texture; // assign background texture
}

void Level::setCloudTexture(const std::shared_ptr<sf::Texture>& texture) {
    cloudsTexture = texture; // assign wall texture
}

void Level::setEmptyTexture(const std::shared_ptr<sf::Texture>& texture) {
    emptyTexture = texture; // assign wall texture
}

/* Load the level from an array of strings. */
// Each string represents a row; '1' is a wall, '0' is a background.
void Level::loadLevel(const std::vector<std::string>& data) {
    height = data.size();                 // Устанавливаем высоту уровня
    width = data[0].size();               // Устанавливаем ширину уровня
    tiles.clear();                        // Очищаем текущие данные плиток
    tiles.resize(height);                 // Изменяем размер массива плиток

    for (int y = 0; y < height; ++y) {
        tiles[y].reserve(width);          // Резервируем место для строк
        for (int x = 0; x < width; ++x) {
            Tile tile;
            sf::Vector2f pos(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize));

            if (data[y][x] == '0') {
                // Устанавливаем фон и позицию
                tile.tileSprite = std::make_unique<sf::Sprite>(*backgroundTexture);
                tile.tileSprite->setPosition(pos);
                tile.tileSprite->setScale(sf::Vector2f(
                    tileSize / backgroundTexture->getSize().x,
                    tileSize / backgroundTexture->getSize().y
                ));
                tile.isSolid = false; // Фон пропускаем
            } else if (data[y][x] == '1') {
                // Устанавливаем стену и позицию
                tile.tileSprite = std::make_unique<sf::Sprite>(*wallTexture);
                tile.tileSprite->setPosition(pos);
                tile.tileSprite->setScale(sf::Vector2f(
                    tileSize / wallTexture->getSize().x,
                    tileSize / wallTexture->getSize().y
                ));
                tile.isSolid = true; // Стена непроходимая
            } else if (data[y][x] == '_'){
                tile.tileSprite = std::make_unique<sf::Sprite>(*emptyTexture);
                tile.tileSprite->setPosition(pos);
                tile.tileSprite->setScale(sf::Vector2f(
                    tileSize / wallTexture->getSize().x,
                    tileSize / wallTexture->getSize().y
                ));
                tile.isSolid = false; // Стена непроходимая
            } else if (data[y][x] == '!'){
                tile.tileSprite = std::make_unique<sf::Sprite>(*emptyTexture);
                tile.tileSprite->setPosition(pos);
                tile.tileSprite->setScale(sf::Vector2f(
                    tileSize / wallTexture->getSize().x,
                    tileSize / wallTexture->getSize().y
                ));
                tile.isSolid = false; // Стена непроходимая
                tile.isKilling = true;
            }

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

/* Check if the tile at (x, y) is solid (impassable). */
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

void Level::setDimensions(int width, int height) {
    this->width = width;      // сохраняем ширину уровня
    this->height = height;    // сохраняем высоту уровня
    tiles.resize(height);     // изменяем высоту массива плиток
}
