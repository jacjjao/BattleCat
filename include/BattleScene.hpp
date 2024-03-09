#pragma once

#include "Scene.hpp"
#include "Cat.hpp"
#include "Enemy.hpp"
#include <vector>

class BattleScene : public Scene {
public:
    BattleScene();

    void Update() override;

private:
    void StartAttack();
    void Draw();
    void CatAttack(Cat &cat);
    void EnemyAttack(Enemy &enemy);

    std::vector<Util::Image> m_CatImage;
    std::vector<Util::Image> m_EnemyImage;

    size_t m_TotalTime = 0;
    std::vector<Cat> m_Cats;
    std::vector<Enemy> m_Enemies;
};
