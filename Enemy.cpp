#include "Enemy.h"
#include <filesystem>
#include <iostream>

namespace
{
    // Returns "<exeDir>/assets/<filename>"
    std::string resourcePath(const std::string& fileName)
    {
        try
        {
            const auto exePath = std::filesystem::current_path(); // VS runs with working dir set to the .vcxproj output by default
            const auto path = exePath / "assets" / fileName;
            return path.string();
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Failed to build resource path for " << fileName << ": " << ex.what() << std::endl;
            return fileName; // fallback
        }
    }

    void ensureTexture(sf::Texture& tex, const std::string& fileName)
    {
        const auto path = resourcePath(fileName);
        if (!tex.loadFromFile(path))
        {
            std::cerr << "Failed to load image \"" << path << "\". Check that the file exists and the working directory is correct." << std::endl;
        }
    }
}

Enemy::Enemy(const sf::Color& color, const sf::Vector2f& scale, const EnemyArchetype& archetype, const unsigned int& windowSizeX)
: alive(true), dissipated(false)
{
    switch(archetype)
    {
        case EnemyArchetype::Brute:
            ensureTexture(texture, "Brute.png");
            hp = 5;
            damage = 10;
            speed = 2.f;
            dissipationRate = 4;
            break;

        case EnemyArchetype::Assasin:
            ensureTexture(texture, "Assasin.png");
            hp = 1;
            damage = 7;
            speed = 7.f;
            dissipationRate = 11;
            break;

        case EnemyArchetype::Boss:
            ensureTexture(texture, "Boss.png");
            hp = 35;
            damage = 100;
            speed = 0.5f;
            enemySprite.setPosition(sf::Vector2f((static_cast<float>(windowSizeX)/2.f)-2.f*static_cast<float>(texture.getSize().x), -4.f*static_cast<float>(texture.getSize().x)));
            dissipationRate = 1;
            break;

        case EnemyArchetype::Minion:
        default:
            ensureTexture(texture, "Minion.png");
            hp = 2;
            damage = 5;
            speed = 4.f;
            dissipationRate = 5;
            break;
    }
    enemySprite.setTexture(texture);
    enemySprite.setColor(color);
    enemySprite.setScale(scale);
}

sf::Sprite* Enemy::getEnemySprite()
{
    return &enemySprite;
}

sf::Uint8 Enemy::getHp() const
{
    return hp;
}

sf::Uint8 Enemy::getDamage() const
{
    return damage;
}

float Enemy::getSpeed() const
{
    return speed;
}

bool Enemy::isAlive() const
{
    return alive;
}

bool Enemy::hasDissipated()
{
    if(enemySprite.getColor().a < 10)
        dissipated = true;

    return dissipated;
}

void Enemy::killEnemy()
{
    alive = false;
}

void Enemy::dissipate()
{
    const sf::Color color = enemySprite.getColor();
    enemySprite.setColor(sf::Color(color.r, color.g, color.b, color.a - dissipationRate));
}

void Enemy::updateEnemy()
{
    enemySprite.move(0.f, speed);
}

void Enemy::updateHealth()
{
    hp--;
}
