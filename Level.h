#ifndef LEVEL_H
#define LEVEL_H


#include "Enemy.h"
#include <vector>

class Level
{
    private:
        unsigned int enemyCount;
        std::vector<EnemyArchetype> enemyCatagories;
        std::vector<Enemy*> enemies;

    public:
        explicit Level(const unsigned int& enemyCount);
        ~Level();
        void addEnemy(Enemy* enemy);
        void addEnemyCategories(const EnemyArchetype& category);
        void setEnemyCount(const unsigned int& newEnemyCount);
        void removeEnemy(const unsigned int& index);
        std::vector<Enemy*> getEnemies();
        std::vector<EnemyArchetype> getEnemyCatagories();
        unsigned int getEnemyCount() const;
        void removeBoss();
};

#endif
