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
    for (auto &cat : m_Cats) {
        cat.Update(1.0 / 60.0);
        cat.Draw(m_CatImage[0]);
    }
    for (auto &enemy : m_Enemies) {
        enemy.Update(1.0 / 60.0);
        enemy.Draw(m_EnemyImage[0]);
    }
}

void BattleScene::AddCat(Cat cat) {
    m_Cats.push_back(std::move(cat));
}

void BattleScene::AddEnemy(Enemy enemy) {
    m_Enemies.push_back(std::move(enemy));
}
