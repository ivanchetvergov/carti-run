#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <map>

enum class TileType {
    Wall,        // Стена
    Background,  // Фон
    Empty,       // Пустая плитка
    Spikes,      // Шипы (смертельная плитка)
    Bouncy,      // Отталкивающая плитка (для усиленного прыжка)
    Hangable,    // Плитка, за которую можно зацепиться (для подвисания)
    WeaponPickup // плитка с оружием
};

// Класс Tile, представляющий отдельную плитку
class Tile {
public:
    std::unique_ptr<sf::Sprite> tileSprite; // Спрайт плитки
    bool isSolid = false;                   // Является ли плитка непроходимой
    bool isKilling = false;
    bool isBouncy = false;   // Прыжковый блок
    bool isHangable = false;
    bool isWeapon = false;

    Tile() = default;
    TileType type = TileType::Empty;
};

// Класс Level, отвечающий за загрузку и отрисовку уровня
class Level {
public:
    // сеттеры
    void setTexture(TileType type, const std::shared_ptr<sf::Texture>& texture);
    void setDimensions(int width, int height);
    void setCloudTexture(const std::shared_ptr<sf::Texture> &texture);

    // геттеры
    float getTileSize() const {return tileSize; }
    const std::map<TileType, std::shared_ptr<sf::Texture>>& getAllTextures() const {return textures;}
    std::shared_ptr<sf::Texture> getCloudTexture() const { return cloudsTexture; }
    std::shared_ptr<sf::Texture> getTexture(TileType type) const;

    void loadLevel(const std::vector<std::string>& data); // загрузка из массива
    void draw(sf::RenderWindow& window); // отрисовка

    bool isTileSolid(int x, int y) const; // проверка стенки
    bool isKilling(int x, int y) const;
    bool isWall(int tileX, int tileY) const;
    bool isBouncy(int tileX, int tileY) const;
    bool isHangable(int tileX, int tileY) const;
    bool isWeaponOrb(int tileX, int tileY) const;

    TileType getTile(int x, int y) { return tiles[y][x].type; }
    TileType getTile(int x, int y) const { return tiles[y][x].type; }
    void setTile(int x, int y, TileType tileType) { tiles[y][x].type = tileType; }
    void removeTile(int x, int y);  

private:
    std::vector<std::vector<Tile>> tiles;           // Двумерный массив плиток
    int width = 0, height = 0;                      // Размеры уровня
    float tileSize = 64.0f;                         // Размер плитки (например, 32x32 пикселя)

    std::shared_ptr<sf::Texture> cloudsTexture;     // Текстура фона
    std::map<TileType, std::shared_ptr<sf::Texture>> textures;
};