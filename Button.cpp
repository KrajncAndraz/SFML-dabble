#include "Button.h"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& pos, const unsigned int& charSize, const unsigned int& r, const unsigned int& g, const unsigned int& b,
    const unsigned int& a, const sf::Font& font, const std::string& str)
{
    background.setSize(size);
    background.setOrigin(size.x/2, size.y/2);
    background.setPosition(pos);
    background.setFillColor(sf::Color(r, g, b, a));
    text.setFont(font);
    text.setString(str);
    text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f + text.getLocalBounds().top);
    text.setCharacterSize(charSize);
    text.setPosition(background.getGlobalBounds().left + background.getGlobalBounds().width / 8.f, background.getGlobalBounds().top + background.getGlobalBounds().height / 30.f);

}

sf::RectangleShape Button::getBackground() const
{
    return background;
}

sf::Text Button::getText() const
{
    return text;
}

void Button::setString(const std::string &str)
{
    text.setString(str);
}
