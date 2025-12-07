#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button
{
    private:
        sf::RectangleShape background;
        sf::Text text;

    public:
        Button(const sf::Vector2f& size, const sf::Vector2f& pos, const unsigned int& charSize, const unsigned int& r, const unsigned int& g, const unsigned int& b,
            const unsigned int& a, const sf::Font& font, const std::string& str);
        sf::RectangleShape getBackground() const;
        sf::Text getText() const;
        void setString(const std::string& str);
};

#endif