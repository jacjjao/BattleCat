#include "BattleScene.hpp"
#include "Util/Time.hpp"
#include <cassert>

BattleScene::BattleScene() {
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/000/walk.png");
    m_EnemyImage.emplace_back(RESOURCE_DIR "/enemys/000/enemy_icon_000.png");

    // tmp
    auto &cat = m_Cats.emplace_back([this](Cat &cat) { CatAttack(cat); });
    cat.SetStats(CatStats::Cat);
    cat.SetPosX(50.f);

    auto &doge = m_Enemies.emplace_back([this](Enemy& e) { EnemyAttack(e); });
    doge.SetStats(EnemyStats::Doge);
    doge.SetPosX(-50.f);
}

void BattleScene::Update() {
    StartAttack();

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

void BattleScene::StartAttack() {
    for (auto &cat : m_Cats) {
        if (cat.GetState() != EntityState::WALK) {
            continue;
        }
        if (std::any_of(m_Enemies.cbegin(), m_Enemies.cend(),
                        [&cat](auto &e) -> bool {
                            return cat.CanAttack(e);
                        })) {
            cat.StartAttack();
        }
    }
    for (auto &enemy : m_Enemies) {
        if (enemy.GetState() != EntityState::WALK) {
            continue;
        }
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

void BattleScene::CatAttack(Cat &cat) {
    const auto IsInRange = [](HitBox a, float p) -> bool {
        return a.low <= p && p <= a.high;
    };
    const auto hitbox = cat.GetHitBox();
    if (cat.IsSingleTarget()) {
        Enemy *target = nullptr;
        for (auto &enemy : m_Enemies) {
            if (!IsInRange(hitbox, enemy.GetPosX())) {
                continue;
            }
            if (!target || target->GetPosX() < enemy.GetPosX()) {
                target = std::addressof(enemy);
            }
        }
        if (target) {
            cat.DealDamage(*target);
        }
    } else {
        for (auto &enemy : m_Enemies) {
            if (IsInRange(hitbox, enemy.GetPosX())) {
                cat.DealDamage(enemy);
            }
        }
    }
}

void BattleScene::EnemyAttack(Enemy &enemy) {
    const auto IsInRange = [](HitBox a, float p) -> bool {
        return a.low <= p && p <= a.high;
    };
    const auto hitbox = enemy.GetHitBox();
    if (enemy.IsSingleTarget()) {
        Cat *target = nullptr;
        for (auto &cat : m_Cats) {
            if (!IsInRange(hitbox, cat.GetPosX())) {
                continue;
            }
            if (!target || target->GetPosX() < cat.GetPosX()) {
                target = std::addressof(cat);
            }
        }
        if (target) {
            enemy.DealDamage(*target);
        }
    } else {
        for (auto &cat : m_Cats) {
            if (IsInRange(hitbox, cat.GetPosX())) {
                enemy.DealDamage(cat);
            }
        }
    }
}
