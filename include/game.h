#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../include/player.h"
#include "../include/level.h"

enum class GameState{
  Playing,
  DeadMenu,
  WinMenu
};

class Game {
public:
  Game();
  void run();

private:
  sf::Texture deathWindowTexture;
  sf::Sprite deathWindowSprite;
  bool showDeathScreen = false;

  sf::Texture winWindowTexture;
  sf::Sprite winWindowSprite;
  bool showWinScreen = false;   

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
  void updateCamera();

  sf::RenderWindow window;
  sf::View camera;
  sf::Sprite cloudsSprite;
  sf::Clock clock;

  sf::Music backgroundMusic;

  Player player;
  Level level;

  sf::Font font;
  sf::Text timerText;
  sf::Clock gameTimer;
  void updateTimer();
};

