#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

struct Tile {
    std::unique_ptr<sf::Sprite> tileSprite;
    bool isSolid = false;

    Tile() = default;

    // запрет копирования
    Tile(const Tile&) = delete;
    Tile& operator=(const Tile&) = delete;

    // разрешение перемещения
    Tile(Tile&&) noexcept = default;
    Tile& operator=(Tile&&) noexcept = default;
};

class Level {
public:
    Level(const std::string& texturePath, int width, int height);

    void draw(sf::RenderWindow& window);
    bool isBlockedAt(float x, float y) const; // проверка на коллизию
    void loadLevel(const std::vector<std::string>& data);


private:
    std::unique_ptr<sf::Texture> tileTexture;
    std::unique_ptr<sf::Sprite> tileSprite;
    int width, height;
    int tileSize = 32;

    std::vector<std::vector<Tile>> tiles; // 1 — блок, 0 — пусто
};
