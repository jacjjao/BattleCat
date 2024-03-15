#include "BattleScene.hpp"
#include "Util/Time.hpp"
#include "DebugUtil/BattleLog.hpp"
#include <cassert>
#include <algorithm>

BattleScene::BattleScene() {
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/000/walk.png");
    m_EnemyImage.emplace_back(RESOURCE_DIR "/enemys/000/enemy_icon_000.png");

    // tmp
    const auto cat_attack = [this](Cat &cat) { CatAttack(cat); };
    auto &cat = m_Cats.emplace_back(CatType::CAT, 100, cat_attack);
    
    auto &doge = m_Enemies.emplace_back(EnemyType::DOGE, -100, [this](Enemy& e) { EnemyAttack(e); });

    m_CatBtn = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/YellowButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    m_CatBtn->SetPosition(0.0, -200.0);
    m_CatBtn->SetZIndex(0.5);
    m_CatBtn->AddButtonEvent([this, cat_attack] {
        m_Cats.emplace_back(CatType::CAT, 200, cat_attack);
    });
    m_Root.AddChild(m_CatBtn);
}

void BattleScene::Update() {
    m_CatBtn->Update();
    m_Root.Update();

    const auto dt = Util::Time::GetDeltaTime(); 

    CatStartAttack();
    for (auto &cat : m_Cats) {
        cat.Walk(dt);
    }

    EnemyStartAttack();
    for (auto &enemy : m_Enemies) {
        enemy.Walk(dt);
    }

    CatStartAttack();
    EnemyStartAttack();

    for (auto &cat : m_Cats) {
        cat.Update();
    }
    for (auto &enemy : m_Enemies) {
        enemy.Update();
    }

    for (auto &dmg_info : m_DmgInfos) {
        dmg_info.attacker->DealDamage(*dmg_info.victim);
    }
    m_DmgInfos.clear();

    const auto IsDead = [](auto &e) -> bool { return e.IsDead() && e.GetState() == EntityState::WALK; };
    const auto dead_cat_it = std::remove_if(m_Cats.begin(), m_Cats.end(), IsDead);
#ifdef ENABLE_BATTLE_LOG
    for (auto it = dead_cat_it; it != m_Cats.end(); ++it) {
        printBattleLog("{} is dead", it->GetName());
    }
#endif
    m_Cats.erase(dead_cat_it, m_Cats.end());

    const auto dead_enemy_it =
        std::remove_if(m_Enemies.begin(), m_Enemies.end(), IsDead);
#ifdef ENABLE_BATTLE_LOG
    for (auto it = dead_enemy_it; it != m_Enemies.end(); ++it) {
        printBattleLog("{} is dead", it->GetName());
    }
#endif
    m_Enemies.erase(dead_enemy_it, m_Enemies.end());

    Draw();
}

void BattleScene::CatStartAttack() {
    for (auto &cat : m_Cats) {
        if (cat.GetState() != EntityState::WALK) {
            continue;
        }
        if (std::any_of(m_Enemies.cbegin(), m_Enemies.cend(),
                        [&cat](auto &e) -> bool { return cat.IsInRange(e); })) {
            cat.StartAttack();
        }
    }
}

void BattleScene::EnemyStartAttack() {
    for (auto &enemy : m_Enemies) {
        if (enemy.GetState() != EntityState::WALK) {
            continue;
        }
        if (std::any_of(
                m_Cats.cbegin(), m_Cats.cend(),
                [&enemy](auto &cat) -> bool { return enemy.IsInRange(cat); })) {
            enemy.StartAttack();
        }
    }
}

void BattleScene::Draw() {
    for (const auto &cat : m_Cats) {
        cat.Draw(m_CatImage[static_cast<size_t>(cat.GetCatType())]);
    }
    for (const auto &enemy : m_Enemies) {
        enemy.Draw(m_EnemyImage[static_cast<size_t>(enemy.GetEnemyType())]);
    }
}

void BattleScene::CatAttack(Cat &cat) {
    ResolveAttack(cat, m_Enemies.data(), m_Enemies.size());
}

void BattleScene::EnemyAttack(Enemy &enemy) {
    ResolveAttack(enemy, m_Cats.data(), m_Cats.size());
}

void BattleScene::ResolveAttack(Entity &attacker, Entity *victims,
                                size_t count) {
    const auto IsInRange = [](HitBox a, float p) -> bool {
        return a.low <= p && p <= a.high;
    };
    const auto hitbox = attacker.GetHitBox();
    if (attacker.IsSingleTarget()) {
        Entity *target = nullptr;
        for (size_t i = 0; i < count; ++i) {
            if (!IsInRange(hitbox, victims[i].GetPosX())) {
                continue;
            }
            if (!target || target->GetPosX() < victims[i].GetPosX()) {
                target = std::addressof(victims[i]);
            }
        }
        if (target) {
            m_DmgInfos.push_back(DamageInfo{std::addressof(attacker), target});
        }
    } else {
        for (size_t i = 0; i < count; ++i) {
            if (IsInRange(hitbox, victims[i].GetPosX())) {
                m_DmgInfos.push_back(DamageInfo{std::addressof(attacker),
                                                std::addressof(victims[i])});
            }
        }
    }
}
