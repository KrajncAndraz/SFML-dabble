#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>

class Player
{
    private:
        sf::Sprite playerSprite;
        std::vector<sf::Sprite> bullets;
        sf::Texture playerTexture, bulletTexture;
        unsigned int health;

    public:
        Player(const sf::Color& color, const sf::Vector2f& scale, const sf::Vector2f& pos);
        sf::Sprite* getPlayerSprite();
        std::vector<sf::Sprite> getBullets();
        void updateBullet(const unsigned int& index, const sf::Vector2f& offset);
        void addBullet(const sf::Vector2f& pos, const sf::Vector2f& scale, const sf::Color& color);
        void removeBullet(unsigned int index);
        unsigned int getHealth() const;
        void updateHealth(const unsigned int& damage);
        void resetHealth();
        void clearBullets();
};

#endif
