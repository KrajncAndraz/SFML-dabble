#include "Level.h"

Level::Level(const unsigned int &enemyCount) : enemyCount(enemyCount){}

Level::~Level()
{
    for(const auto & e : enemies)
    {
        delete e;
    }
    enemies.clear();
}

void Level::addEnemy(Enemy* enemy)
{
    enemies.push_back(enemy);
}

void Level::addEnemyCategories(const EnemyArchetype &category)
{
    bool included = false;
    for(auto & enemyCatagorie : enemyCatagories)
    {
        if(category == enemyCatagorie)
            included = true;
    }
    if(!included)
        enemyCatagories.push_back(category);
}

void Level::setEnemyCount(const unsigned int& newEnemyCount)
{
    enemyCount = newEnemyCount;
}

void Level::removeEnemy(const unsigned int& index)
{
    enemies[index];
    auto it = enemies.begin() + index;
    enemies.erase(it);
    enemyCount--;
}

std::vector<Enemy*> Level::getEnemies()
{
    return enemies;
}

std::vector<EnemyArchetype> Level::getEnemyCatagories()
{
    return enemyCatagories;
}

unsigned int Level::getEnemyCount() const
{
    return enemyCount;
}

void Level::removeBoss()
{
    for(int i=0; i<enemyCatagories.size(); i++)
    {
        auto it = enemyCatagories.begin()+i;
        if(enemyCatagories[i] == EnemyArchetype::Boss)
            enemyCatagories.erase(it);
    }
}
