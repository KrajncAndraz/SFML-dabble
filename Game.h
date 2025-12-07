#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "Menu.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"

class Game
{
    private:
        sf::RenderWindow* window;
        sf::Event event{};
        sf::Vector2i mousePos;
        float enemySpawnTimer, enemySpawnTimerMax;
        unsigned int maxEnemies, currLvl, bckgrndSprits;
        sf::Font font;
        sf::Text text;
        Menu* pauseMenu;
        Menu* startMenu;
        Menu* gameOverMenu;
        bool paused, start, moving, gameOver;
        Player* player;
        char key;
        std::vector<Level> levels;
        sf::Texture backgroundTexture;
        std::vector<sf::Sprite> background;

    public:
        Game();
        ~Game();
        bool running() const;
        void pollEvents();
        void updateMousePos();
        void checkForMouseHit(const Menu* menu);
        void spawnEnemy();
        void updateEnemies();
        void renderEnemies();
        void updateText();
        void renderText() const;
        void renderMenu(const Menu* menu) const;
        void updatePlayer();
        void renderPlayer() const;
        void spawnBullet() const;
        void updateBullets();
        void renderBullets() const;
        void updateLvls();
        void updateBackground();
        void renderBackground();

        void restart();
        void update();
        void render();
};

#endif