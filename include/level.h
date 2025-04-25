#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

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

    std::vector<std::vector<int>> tiles; // 1 — блок, 0 — пусто
};
