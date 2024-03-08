#include "BattleScene.hpp"
#include "Util/Time.hpp"

BattleScene::BattleScene() {
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/000/walk.png");
    m_EnemyImage.emplace_back(RESOURCE_DIR "/enemys/000/enemy_icon_000.png");

    // tmp
    Cat cat;
    cat.SetStats(CatStats::Cat);
    AddCat(std::move(cat));

    Enemy doge;
    doge.SetStats(EnemyStats::Doge);
    AddEnemy(std::move(doge));
}

void BattleScene::Update() {
    constexpr float dt = 1.0f / 60.0f; // tmp for now
    for (auto &cat : m_Cats) {
        cat.Update(dt);
    }
    for (auto &enemy : m_Enemies) {
        enemy.Update(dt);
    }

    StartAttack();

    Draw();
}

void BattleScene::AddCat(Cat cat) {
    m_Cats.push_back(std::move(cat));
}

void BattleScene::AddEnemy(Enemy enemy) {
    m_Enemies.push_back(std::move(enemy));
}

void BattleScene::StartAttack() {
    for (auto &cat : m_Cats) {
        for (const auto &enemy : m_Enemies) {
            if (cat.CanAttack(enemy.GetPosX())) {
                cat.StartAttack();
                break;
            }
        }
    }

    for (auto &enemy : m_Enemies) {
        for (const auto & cat: m_Cats) {
            if (enemy.CanAttack(cat.GetPosX())) {
                enemy.StartAttack();
                break;
            }
        }
    }
}

void BattleScene::Draw() {
    for (const auto &cat : m_Cats) {
        cat.Draw(m_CatImage[0]);
    }
    for (const auto &enemy : m_Enemies) {
        enemy.Draw(m_EnemyImage[0]);
    }
}
