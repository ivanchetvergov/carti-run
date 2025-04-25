#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

// Класс Tile, представляющий отдельную плитку
class Tile {
public:
    std::unique_ptr<sf::Sprite> tileSprite; // Спрайт плитки
    bool isSolid = false;                   // Является ли плитка непроходимой
};

// Класс Level, отвечающий за загрузку и отрисовку уровня
class Level {
private:
    std::vector<std::vector<Tile>> tiles;         // Двумерный массив плиток
    int width = 0, height = 0;                      // Размеры уровня
    float tileSize = 64.0f;                         // Размер плитки (например, 32x32 пикселя)
    std::shared_ptr<sf::Texture> wallTexture;       // Текстура стены
    std::shared_ptr<sf::Texture> backgroundTexture; // Текстура фона

public:
    // Сеттеры
    void setWallTexture(const std::shared_ptr<sf::Texture>& texture);
    void setBackgroundTexture(const std::shared_ptr<sf::Texture>& texture);
    void setDimensions(int width, int height);

    // геттеры
    float getTileSize() const {return tileSize; }    

    void loadLevel(const std::vector<std::string>& data); // загрузка из массива
    void draw(sf::RenderWindow& window); // отрисовка
    bool isTileSolid(int x, int y) const; // проверка стенки

};