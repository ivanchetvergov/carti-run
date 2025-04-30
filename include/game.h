/*#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "player.h"
#include "level.h"

enum class GameState{
  Playing,
  DeadMenu
};

class Game {
public:
  Game();
  void run();
  

private:
  std::shared_ptr<sf::Texture> wallTexture;
  std::shared_ptr<sf::Texture> emptyTexture;
  std::shared_ptr<sf::Texture> backgroundTexture;
  std::shared_ptr<sf::Texture> cloudsTexture;
  std::shared_ptr<sf::Texture> spikesTexture;

  void loadTexture(std::shared_ptr<sf::Texture>& texture, const std::string& path);

  void processEvents();
  void update(float deltaTime);
  void render();
  void reset();
  void handleEvents();
  #void updateCamera();


    sf::RenderWindow window;
    sf::View camera;
    sf::Sprite cloudsSprite;
    sf::Clock clock;

    sf::Font font;
    sf::Text deathText;

    sf::Music backgroundMusic;

    Player player;
    Level level;

    bool showDeathScreen = false;
};
*/
