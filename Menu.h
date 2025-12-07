#ifndef MENU_H
#define MENU_H

#include <vector>
#include "Button.h"

class Menu
{
    private:
        sf::RectangleShape background;
        std::vector<Button> buttons;

    public:
        Menu(const sf::Vector2f& windowSize, const unsigned int& r, const unsigned int& g, const unsigned int& b, const unsigned int& a);
        void addButton(const Button& button);
        sf::RectangleShape getBackground() const;
        std::vector<Button> getButtons() const;
        Button* getButton(const unsigned int& index);
};

#endif