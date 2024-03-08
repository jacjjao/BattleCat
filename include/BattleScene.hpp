#pragma once

#include "Scene.hpp"
#include "Cat.hpp"
#include "Enemy.hpp"
#include <vector>

class BattleScene : public Scene {
public:
    BattleScene();

    void Update() override;

    void AddCat(Cat cat);
    void AddEnemy(Enemy enemy);

private:
    void StartAttack();
    void Draw();
    void CatAttack(Cat &cat);

    std::vector<Util::Image> m_CatImage;
    std::vector<Util::Image> m_EnemyImage;

    size_t m_TotalTime = 0;
    std::vector<Cat> m_Cats;
    std::vector<Enemy> m_Enemies;
};
