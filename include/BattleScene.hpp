#ifndef BATTLESCENE_HPP
#define BATTLESCENE_HPP

#pragma once

#include "Scene.hpp"
#include "Cat.hpp"
#include "Enemy.hpp"
#include "GameButton.hpp"
#include "Util/Root.hpp"
#include "Stage.hpp"
#include "Wallet.hpp"
#include "DeployButton.hpp"
#include "Camera.hpp"
#include "BattleBackground.hpp"
#include "Workaholic.hpp"
#include <vector>

struct DamageInfo {
    Entity *attacker;
    Entity *victim;
};

class BattleScene : public Scene {
public:
    BattleScene(App &app);

    void Update() override;
    void LoadStage(Stage &stage);

    BattleScene(const BattleScene &) = delete;
    BattleScene &operator=(const BattleScene &) = delete;

private:
    static constexpr size_t s_MaxEntityCount = 41; // +1 for tower
    static constexpr float s_CatTowerPosX = 500.0f;
    static constexpr float s_EnemiesTowerPosX = -500.0f;

    void GameOver(bool cat_won);
    void CatStartAttack();
    void EnemyStartAttack();
    void Draw();
    void CatAttack(Cat &cat);
    void EnemyAttack(Enemy &enemy);
    void AddCat(CatType type, int level);
    void AddEnemy(EnemyType type, float modifier);
    void CreateUnitButtons();

    std::vector<Util::Image> m_CatImage;
    std::vector<Util::Image> m_EnemyImage;

    std::vector<Cat> m_Cats;
    std::vector<Enemy> m_Enemies;
    std::vector<DamageInfo> m_DmgInfos;

    std::vector<std::shared_ptr<DeployButton>> m_CatButton;
    std::shared_ptr<GameButton> m_ReturnButton;

    std::optional<BattleBackground> m_Background;

    Cat *m_CatTower = nullptr;
    Enemy *m_EnemyTower = nullptr;

    Util::Root m_Root;

    Stage m_Stage;
    std::optional<Wallet> m_Wallet;

    double m_TotalTime = 0.0;

    Camera m_Cam;

    App &m_App;

    std::optional<Workholic> m_Work;
};
#endif //BATTLESCENE_HPP
