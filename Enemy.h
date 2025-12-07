#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

enum class EnemyArchetype {Minion, Brute, Assasin, Boss};

class Enemy
{
    private:
        sf::Sprite enemySprite;
        sf::Texture texture;
        bool alive, dissipated;
        sf::Uint8 dissipationRate, hp, damage;
        float speed;

    public:
        Enemy(const sf::Color& color, const sf::Vector2f& scale, const EnemyArchetype& archetype, const unsigned int& windowSizeX);
        sf::Sprite* getEnemySprite();
        sf::Uint8 getHp() const;
        sf::Uint8 getDamage() const;
        float getSpeed() const;
        bool isAlive() const;
        bool hasDissipated();
        void killEnemy();
        void dissipate();
        void updateEnemy();
        void updateHealth();


};

#endif
