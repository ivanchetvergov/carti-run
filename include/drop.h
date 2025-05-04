#pragma once


#include <SFML/Graphics.hpp>  // ← нужно для sf::*
#include <vector>   

struct Shot {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    int damage;
};

class Weapon {
    public:
        float fireRate = 6;              // выстрелов в секунду
        float fireCooldown = 0.1f;    // сколько времени до следующего выстрела
        int damage;

        float energy = 0.f;          // накопленная энергия
        float maxEnergy = 100.f;     // максимум для ульты
        bool canUseUltimate() const { return energy >= maxEnergy; }
    
        void update(float dt);
        void fire(sf::Vector2f position, bool facingRight, std::vector<Shot>& shots);
        void fireUltimate(sf::Vector2f position, bool facingRight, std::vector<Shot>& shots);
};
    