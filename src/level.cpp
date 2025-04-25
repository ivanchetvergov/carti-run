#include "level.h"

/* Set the wall texture. */
void Level::setWallTexture(const std::shared_ptr<sf::Texture>& texture) {
    wallTexture = texture; // assign wall texture
}

/* Set the background texture. */
void Level::setBackgroundTexture(const std::shared_ptr<sf::Texture>& texture) {
    backgroundTexture = texture; // assign background texture
}

/* Load the level from an array of strings. */
// Each string represents a row; '1' is a wall, '0' is a background.
void Level::loadLevel(const std::vector<std::string>& data) {
    height = data.size();                 // set level height
    width = data[0].size();                 // set level width
    tiles.clear();                        // clear any existing tiles
    tiles.resize(height);                 // resize tiles container

    for (int y = 0; y < height; ++y) {
        tiles[y].reserve(width);          // reserve space for row
        for (int x = 0; x < width; ++x) {
            Tile tile;
            sf::Vector2f pos(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize));

            // Set background texture by default and position it.
            tile.tileSprite = std::make_unique<sf::Sprite>(*backgroundTexture);
            tile.tileSprite->setPosition(pos); // Устанавливаем позицию плитки
            tile.tileSprite->setScale(sf::Vector2f(
                tileSize / backgroundTexture->getSize().x,
                tileSize / backgroundTexture->getSize().y
            )); // Масштабируем текстуру
            
            // For a wall ('1'), replace background with wall texture and mark as solid.
            if (data[y][x] == '1') {
                tile.tileSprite = std::make_unique<sf::Sprite>(*wallTexture);
                tile.tileSprite->setPosition(pos); // Устанавливаем позицию плитки
                tile.tileSprite->setScale(sf::Vector2f(
                    tileSize / wallTexture->getSize().x,
                    tileSize / wallTexture->getSize().y
                )); // Масштабируем текстуру
                
                tile.isSolid = true;               // mark tile as impassable
            } else {
                tile.isSolid = false;              // background tile is passable
            }
            
            tiles[y].emplace_back(std::move(tile)); // add the tile to the row
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

void Level::setDimensions(int width, int height) {
    this->width = width;      // сохраняем ширину уровня
    this->height = height;    // сохраняем высоту уровня
    tiles.resize(height);     // изменяем высоту массива плиток
}
