#include "Game.h"
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

    void ensureFont(sf::Font& font, const std::string& fileName)
    {
        const auto path = resourcePath(fileName);
        if (!font.loadFromFile(path))
        {
            std::cerr << "Failed to load font \"" << path << "\". Try a standard TTF/OTF (non-variable). Ensure the file is present." << std::endl;
        }
    }
}

Game::Game() : enemySpawnTimer(110.f), enemySpawnTimerMax(110.f), maxEnemies(10), currLvl(0), bckgrndSprits(0), paused(true), start(true), moving(false), gameOver(false), key('x')
{
    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Space Pew Pew", sf::Style::Fullscreen);
    window->setFramerateLimit(120);

    ensureTexture(backgroundTexture, "Stars.png");
    sf::Sprite bckgrnd;
    bckgrnd.setTexture(backgroundTexture);
    bckgrnd.setColor(sf::Color(255, 255, 255, 125));
    background.push_back(bckgrnd);
    bckgrndSprits++;

    ensureFont(font, "Jersey10-Regular.ttf");
    text.setFont(font);
    text.setString("Level: 1\nHP: 100\nEnemies left: 20");
    text.setPosition(10.f, 10.f);
    text.setScale(2.f, 2.f);

    pauseMenu = new Menu(sf::Vector2f(window->getSize()), 0, 0, 0, 255);
    pauseMenu->addButton(Button(sf::Vector2f(500.f, 300.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/1.5f), 200, 50, 50, 50, 255, font, "Quit"));
    pauseMenu->addButton(Button(sf::Vector2f(800.f, 300.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/3.f), 200, 50, 50, 50, 255, font, "Resume"));

    startMenu = new Menu(sf::Vector2f(window->getSize()), 0, 0, 0, 255);
    startMenu->addButton(Button(sf::Vector2f(1900.f, 300.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/5.f), 220, 0, 0, 0, 0, font, "Space Pew Pew"));
    startMenu->addButton(Button(sf::Vector2f(500.f, 250.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/2.f), 150, 50, 50, 50, 255, font, "Play"));
    startMenu->addButton(Button(sf::Vector2f(500.f, 250.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/1.3f), 150, 50, 50, 50, 255, font, "Quit"));

    gameOverMenu = new Menu(sf::Vector2f(window->getSize()), 0, 0, 0, 255);
    gameOverMenu->addButton(Button(sf::Vector2f(1000.f, 300.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/5.f), 175, 0, 0, 0, 0, font, "Game Over!"));
    gameOverMenu->addButton(Button(sf::Vector2f(900.f, 300.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/2.5f), 175, 0, 0, 0, 0, font, "You Lost!"));
    gameOverMenu->addButton(Button(sf::Vector2f(400.f, 160.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/1.8f), 120, 50, 50, 50, 255, font, "Retry"));
    gameOverMenu->addButton(Button(sf::Vector2f(300.f, 160.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y)/1.3f), 120, 50, 50, 50, 255, font, "Quit"));

    player = new Player(sf::Color(175, 175, 175), sf::Vector2f(4.f, 4.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y) - 150.f));

    levels.emplace_back(20);
    levels[0].addEnemyCategories(EnemyArchetype::Minion);

    levels.emplace_back(30);
    levels[1].addEnemyCategories(EnemyArchetype::Minion);
    levels[1].addEnemyCategories(EnemyArchetype::Assasin);

    levels.emplace_back(40);
    levels[2].addEnemyCategories(EnemyArchetype::Minion);
    levels[2].addEnemyCategories(EnemyArchetype::Brute);
    levels[2].addEnemyCategories(EnemyArchetype::Assasin);

    levels.emplace_back(30);
    levels[3].addEnemyCategories(EnemyArchetype::Minion);
    levels[3].addEnemyCategories(EnemyArchetype::Brute);
    levels[3].addEnemyCategories(EnemyArchetype::Assasin);
    levels[3].addEnemyCategories(EnemyArchetype::Boss);
}

Game::~Game()
{
    delete window;
    delete pauseMenu;
    delete startMenu;
    delete player;
}

bool Game::running() const
{
    return window->isOpen();
}

void Game::pollEvents()
{
    while(window->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
            {
                window->close();
                break;
            }

            case sf::Event::JoystickMoved:
            {
                if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -15.f && !paused)
                {
                    key = 'a';
                    moving = true;
                }
                else if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 15.f && !paused)
                {
                    key = 'd';
                    moving = true;
                }
                else if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > -15.f && sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < 15.f)
                    moving = false;
                break;
            }

            case sf::Event::JoystickButtonPressed:
            {
                if(sf::Joystick::isButtonPressed(0, 6))
                {
                    if(paused && !start && !gameOver)
                        paused = false;
                    else if(!paused && !start && !gameOver)
                        paused = true;
                    else
                        window->close();
                }
                else if(sf::Joystick::isButtonPressed(0, 2))
                {
                    if(!paused && !gameOver)
                        spawnBullet();
                }
                else if(sf::Joystick::isButtonPressed(0, 1))
                {
                    if(paused || gameOver)
                        window->close();
                }
                else if(sf::Joystick::isButtonPressed(0, 0) && paused)
                {
                    paused = false;
                    start = false;
                }
                else if(sf::Joystick::isButtonPressed(0, 0) && gameOver)
                {
                    restart();
                }
                break;
            }

            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    if(paused && !start&& !gameOver)
                        paused = false;
                    else if(!paused && !start&& !gameOver)
                        paused = true;
                    else
                        window->close();
                }
                else if(event.key.code == sf::Keyboard::A && !paused)
                {
                    key = 'a';
                    moving = true;
                }
                else if(event.key.code == sf::Keyboard::D && !paused)
                {
                    key = 'd';
                    moving = true;
                }
                else if(event.key.code == sf::Keyboard::M && !paused)
                    spawnBullet();
                break;
            }

            case sf::Event::KeyReleased:
            {
                if(event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
                    moving = false;
                break;
            }

            case sf::Event::LostFocus:
            {
                if(!paused)
                    paused = true;
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                if(event.key.code == sf::Mouse::Left)
                {
                    updateMousePos();
                    if(start && paused)
                        checkForMouseHit(startMenu);
                    else if(!start && paused)
                        checkForMouseHit(pauseMenu);
                    else if(gameOver)
                        checkForMouseHit(gameOverMenu);
                }
                break;
            }
            default:
                break;
        }
    }
}

void Game::updateMousePos()
{
    mousePos = sf::Mouse::getPosition(*window);
}

void Game::checkForMouseHit(const Menu* menu)
{
    std::vector<Button> buttons = menu->getButtons();
    for(auto it = buttons.begin(); it != buttons.end();)
    {
        if(it->getBackground().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            if(it->getText().getString() == "Quit")
            {
                window->close();
            }
            else if(it->getText().getString() == "Retry")
            {
                restart();
            }
            else if(it->getText().getString() != "Space Pew Pew")
            {
                paused = false;
                start = false;
            }
            break;
        }
        ++it;
    }
}

void Game::spawnEnemy()
{
    const unsigned int selection = levels[currLvl].getEnemyCatagories().size();
    unsigned int archetype = rand()% selection;
    if(levels[currLvl].getEnemyCatagories()[archetype] == EnemyArchetype::Boss && levels[currLvl].getEnemyCount() > 7)
        archetype--;
    const unsigned int winSize = window->getSize().x;
    auto* enemy = new Enemy((sf::Color
    (
        static_cast<sf::Uint8>(rand() % 206 + 50),
        static_cast<sf::Uint8>(rand() % 206 + 50),
        static_cast<sf::Uint8>(rand() % 206 + 50)
    )),
    sf::Vector2f(4.f, 4.f),
    levels[currLvl].getEnemyCatagories()[archetype],
    winSize);
    if(levels[currLvl].getEnemyCatagories()[archetype] != EnemyArchetype::Boss)
        enemy->getEnemySprite()->setPosition(sf::Vector2f(static_cast<float>(rand() % static_cast<unsigned int>(static_cast<float>(window->getSize().x) - 4.f*static_cast<float>(enemy->getEnemySprite()->getTexture()->getSize().x))), -4.f*static_cast<float>(enemy->getEnemySprite()->getTexture()->getSize().x)));
    else
        levels[currLvl].removeBoss();

    std::vector<Enemy*> enemies = levels[currLvl].getEnemies();
    for(const auto & e : enemies)
    {
        while(enemy->getEnemySprite()->getGlobalBounds().intersects(e->getEnemySprite()->getGlobalBounds()))
        {
            if(enemy->getEnemySprite()->getPosition().x < static_cast<float>(window->getPosition().x) - 4.f*static_cast<float>(enemy->getEnemySprite()->getTexture()->getSize().x))
                enemy->getEnemySprite()->move(1.f, 0.f);
            else if(enemy->getEnemySprite()->getPosition().x > 0.f)
                enemy->getEnemySprite()->move(-1.f, 0.f);
        }
    }

    levels[currLvl].addEnemy(enemy);
}

void Game::updateEnemies()
{
    if(levels[currLvl].getEnemies().size() < maxEnemies && levels[currLvl].getEnemies().size() < levels[currLvl].getEnemyCount())
    {
        if(enemySpawnTimer >= enemySpawnTimerMax)
        {
            spawnEnemy();
            enemySpawnTimer = 0.f;
        }
        else
            enemySpawnTimer += 1.f;
    }

    unsigned int size = levels[currLvl].getEnemies().size();
    for(int i=0; i<size;)
    {
        std::vector<Enemy*> enemies = levels[currLvl].getEnemies();
        enemies[i]->updateEnemy();
        if(enemies[i]->isAlive())
        {
            if(enemies[i]->getEnemySprite()->getPosition().y >= static_cast<float>(window->getSize().y))
            {
                player->updateHealth(enemies[i]->getDamage());
                levels[currLvl].removeEnemy(i);
                size--;
            }
            else
            {
                i++;
            }
        }
        else if(!enemies[i]->isAlive() && !enemies[i]->hasDissipated())
        {
            enemies[i]->dissipate();
            i++;
        }
        else if(enemies[i]->hasDissipated())
        {
            levels[currLvl].removeEnemy(i);
            size--;
        }
        else
        {
            i++;
        }
    }
}

void Game::renderEnemies()
{
    std::vector<Enemy*> enemies = levels[currLvl].getEnemies();
    for(const auto & e : enemies)
    {
        window->draw(*e->getEnemySprite());
    }
}

void Game::updateText()
{
    text.setString("Level: " + std::to_string(currLvl+1) + "\nHP: " + std::to_string(player->getHealth()) + "\nEnemies left: " + std::to_string(levels[currLvl].getEnemyCount()));
}

void Game::renderText() const
{
    window->draw(text);
}

void Game::renderMenu(const Menu* menu) const
{
    window->draw(menu->getBackground());
    const std::vector<Button> buttons = menu->getButtons();
    for(const auto & button : buttons)
    {
        window->draw(button.getBackground());
        window->draw(button.getText());
    }
}

void Game::updatePlayer()
{
    if(player->getHealth() == 0)
        gameOver = true;
    if(key == 'a' && moving && player->getPlayerSprite()->getPosition().x >= 5.f)
    {
        player->getPlayerSprite()->move(-15.f, 0.f);
    }
    else if(key == 'd' && moving && player->getPlayerSprite()->getPosition().x <= static_cast<float>(window->getSize().x) - 4.f*(static_cast<float>(player->getPlayerSprite()->getTexture()->getSize().x)+2.f))
    {
        player->getPlayerSprite()->move(15.f, 0.f);
    }
}

void Game::renderPlayer() const
{
    window->draw(*player->getPlayerSprite());
}

void Game::spawnBullet() const
{
    player->addBullet(sf::Vector2f(player->getPlayerSprite()->getPosition().x + player->getPlayerSprite()->getScale().x/2, player->getPlayerSprite()->getPosition().y),
                        sf::Vector2f(4.f, 4.f), sf::Color(255, 255, 100));
}

void Game::updateBullets()
{
    //const std::vector<sf::Sprite> bullets = player->getBullets();
    for(unsigned int i = 0; i < player->getBullets().size(); i++)
    {
        const std::vector<sf::Sprite> bullets = player->getBullets();
        player->updateBullet(i, sf::Vector2f(0.f, -10.f));
        if(bullets[i].getPosition().y <= 0.f - bullets[i].getScale().y*20)
            player->removeBullet(i);

        std::vector<Enemy*> enemies = levels[currLvl].getEnemies();
        for(const auto & e : enemies)
        {
            if(bullets[i].getGlobalBounds().intersects(e->getEnemySprite()->getGlobalBounds()) && e->isAlive())
            {
                e->updateHealth();
                if(e->getHp() <= 0)
                    e->killEnemy();
                player->removeBullet(i);
            }
        }
    }
}

void Game::renderBullets() const
{
    const std::vector<sf::Sprite> bullets = player->getBullets();
    for(const auto& i : bullets)
    {
        window->draw(i);
    }
}

void Game::updateLvls()
{
    if(levels[currLvl].getEnemyCount() == 0 && currLvl < levels.size()-1)
    {
        currLvl++;
    }
    else if(levels[currLvl].getEnemyCount() == 0 && currLvl == levels.size()-1)
    {
        gameOver = true;
        gameOverMenu->getButton(1)->setString("You Won!");
    }
}

void Game::updateBackground()
{
    if(bckgrndSprits < 2)
    {
        sf::Sprite bckgrnd;
        bckgrnd.setTexture(backgroundTexture);
        bckgrnd.setPosition(0.f, static_cast<float>(window->getSize().y)*-1);
        bckgrnd.setColor(sf::Color(255, 255, 255, 125));
        background.push_back(bckgrnd);
        bckgrndSprits++;
    }
    for (std::size_t idx = background.size(); idx > 0; --idx)
    {
        auto& spr = background[idx - 1];
        spr.move(0.f, 0.2f);
        if (spr.getPosition().y >= static_cast<float>(window->getSize().y) + 1.f)
        {
            background.erase(background.begin() + static_cast<std::ptrdiff_t>(idx - 1));
            bckgrndSprits--;
        }
    }
}

void Game::renderBackground()
{
    for(const auto & i : background)
    {
        window->draw(i);
    }
}

void Game::restart()
{
    currLvl = 0;
    gameOver = false;
    gameOverMenu->getButton(1)->setString("You Lost!");
    player->resetHealth();
    player = new Player(sf::Color(175, 175, 175), sf::Vector2f(4.f, 4.f), sf::Vector2f(static_cast<float>(window->getSize().x)/2.f, static_cast<float>(window->getSize().y) - 150.f));
    background.clear();
    ensureTexture(backgroundTexture, "Stars.png");
    sf::Sprite bckgrnd;
    bckgrnd.setTexture(backgroundTexture);
    bckgrnd.setColor(sf::Color(255, 255, 255, 125));
    background.push_back(bckgrnd);
    bckgrndSprits = 1;

    levels.clear();
    levels.emplace_back(20);
    levels[0].addEnemyCategories(EnemyArchetype::Minion);

    levels.emplace_back(30);
    levels[1].addEnemyCategories(EnemyArchetype::Minion);
    levels[1].addEnemyCategories(EnemyArchetype::Assasin);

    levels.emplace_back(40);
    levels[2].addEnemyCategories(EnemyArchetype::Minion);
    levels[2].addEnemyCategories(EnemyArchetype::Brute);
    levels[2].addEnemyCategories(EnemyArchetype::Assasin);

    levels.emplace_back(30);
    levels[3].addEnemyCategories(EnemyArchetype::Minion);
    levels[3].addEnemyCategories(EnemyArchetype::Brute);
    levels[3].addEnemyCategories(EnemyArchetype::Assasin);
    levels[3].addEnemyCategories(EnemyArchetype::Boss);
}

void Game::update()
{
    pollEvents();
    if(!paused && !start && !gameOver)
    {
        updateBackground();
        updatePlayer();
        updateBullets();
        updateEnemies();
        updateLvls();
        updateText();
        window->setMouseCursorVisible(false);
    }
    else
        window->setMouseCursorVisible(true);
}

void Game::render()
{
    window->clear();

    if(paused && !start)
        renderMenu(pauseMenu);

    else if(start && paused)
        renderMenu(startMenu);

    else if(gameOver)
        renderMenu(gameOverMenu);

    else
    {
        renderBackground();
        renderPlayer();
        renderEnemies();
        renderBullets();
        renderText();
    }

    window->display();
}