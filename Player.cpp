#include "Player.h"
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

Player::Player(const sf::Color& color, const sf::Vector2f& scale, const sf::Vector2f& pos) : health(100)
{
    playerSprite.setColor(color);
    playerSprite.setScale(scale);
    playerSprite.setPosition(pos);
    ensureTexture(playerTexture, "SS1.png");
    playerSprite.setTexture(playerTexture);
    ensureTexture(bulletTexture, "Bullets.png");
}

sf::Sprite* Player::getPlayerSprite()
{
    return &playerSprite;
}

std::vector<sf::Sprite> Player::getBullets()
{
    return bullets;
}

void Player::updateBullet(const unsigned int& index, const sf::Vector2f& offset)
{
    bullets[index].move(offset);
}

void Player::addBullet(const sf::Vector2f& pos, const sf::Vector2f& scale, const sf::Color& color)
{
    sf::Sprite bullet;
    bullet.setTexture(bulletTexture);
    bullet.setPosition(pos);
    bullet.setScale(scale);
    bullet.setColor(color);
    bullets.push_back(bullet);
}

void Player::removeBullet(unsigned int index)
{
    const auto it = bullets.begin() + index;
    bullets.erase(it);
}

unsigned int Player::getHealth() const
{
    return health;
}

void Player::updateHealth(const unsigned int& damage)
{
    if(damage <= health)
        health -= damage;
    else
        health = 0;
}

void Player::resetHealth()
{
    health = 100;
}

void Player::clearBullets()
{
    bullets.clear();
}