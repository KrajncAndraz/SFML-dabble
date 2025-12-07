#include "Menu.h"

Menu::Menu(const sf::Vector2f& windowSize, const unsigned int& r, const unsigned int& g, const unsigned int& b, const unsigned int& a)
{
    background.setSize(static_cast<sf::Vector2f>(windowSize));
    background.setFillColor(sf::Color(r, g, b, a));
}

void Menu::addButton(const Button& button)
{
    buttons.push_back(button);
}

sf::RectangleShape Menu::getBackground() const
{
    return background;
}

std::vector<Button> Menu::getButtons() const
{
    return buttons;
}

Button* Menu::getButton(const unsigned int& index)
{
    return &buttons[index];
}
