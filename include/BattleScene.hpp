#pragma once

#include "Scene.hpp"
#include "Cat.hpp"
#include "Enemy.hpp"
#include "GameButton.hpp"
#include "Util/Root.hpp"
#include <vector>

struct DamageInfo {
    Entity *attacker;
    Entity *victim;
};

class BattleScene : public Scene {
public:
    BattleScene();

    void Update() override;

    BattleScene(const BattleScene &) = delete;
    BattleScene &operator=(const BattleScene &) = delete;

private:
    void CatStartAttack();
    void EnemyStartAttack();
    void Draw();
    void CatAttack(Cat &cat);
    void EnemyAttack(Enemy &enemy);

    std::vector<Util::Image> m_CatImage;
    std::vector<Util::Image> m_EnemyImage;

    size_t m_TotalTime = 0;
    std::vector<Cat> m_Cats;
    std::vector<Enemy> m_Enemies;
    std::vector<DamageInfo> m_DmgInfos;

    Util::Root m_Root;
    std::shared_ptr<GameButton> m_CatBtn; // tmp
    std::shared_ptr<GameButton> m_EBtn; // tmp
};
