#include "BattleScene.hpp"
#include "Util/Time.hpp"

BattleScene::BattleScene() {
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/000/walk.png");
    m_EnemyImage.emplace_back(RESOURCE_DIR "/enemys/000/enemy_icon_000.png");

    // tmp
    Cat cat;
    cat.SetStats(CatStats::Cat);
    cat.SetPosX(50.f);
    AddCat(std::move(cat));

    Enemy doge;
    doge.SetStats(EnemyStats::Doge);
    doge.SetPosX(-50.f);
    AddEnemy(std::move(doge));
}

void BattleScene::Update() {
    const auto dt = Util::Time::GetDeltaTime(); 
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
        if (std::any_of(m_Enemies.cbegin(), m_Enemies.cend(),
                        [&cat](auto &e) -> bool {
                            return cat.CanAttack(e);
                        })) {
            cat.StartAttack();
        }
    }

    for (auto &enemy : m_Enemies) {
        if (std::any_of(m_Cats.cbegin(), m_Cats.cend(),
                        [&enemy](auto &cat) -> bool {
                            return enemy.CanAttack(cat);
                        })) {
            enemy.StartAttack();
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
