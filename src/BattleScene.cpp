#include "BattleScene.hpp"
#include "Util/Time.hpp"
#include "DebugUtil/BattleLog.hpp"
#include "App.hpp"
#include <cassert>
#include <algorithm>

BattleScene::BattleScene(App &app)
    : m_App(app) {
    m_Cats.reserve(s_MaxEntityCount); // to prevent reallocation
    m_Enemies.reserve(s_MaxEntityCount);
    
    Reset();

    m_CatImage.emplace_back(RESOURCE_DIR "/cats/000/walk.png");
    m_EnemyImage.emplace_back(RESOURCE_DIR "/enemys/000/enemy_icon_000.png");

    m_CatBtn = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/YellowButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    m_CatBtn->SetPosition(200.0, -200.0);
    m_CatBtn->SetZIndex(0.5);
    m_CatBtn->AddButtonEvent([this] {
        AddCat(CatType::AXE_CAT, 1);
    });
    m_Root.AddChild(m_CatBtn);

    m_EBtn = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/YellowButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    m_EBtn->SetPosition(-200.0, -200.0);
    m_EBtn->SetZIndex(0.5);
    m_EBtn->AddButtonEvent([this] {
        AddEnemy(EnemyType::DOGE, 1.0f);
    });
    m_Root.AddChild(m_EBtn);
}

void BattleScene::Update() {
    m_CatBtn->Update();
    m_EBtn->Update();
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
        cat.UpdateTimer(dt);
        if (cat.OnAttack()) {
            CatAttack(cat);
        }
    }
    for (auto &enemy : m_Enemies) {
        enemy.UpdateTimer(dt);
        if (enemy.OnAttack()) {
            EnemyAttack(enemy);
        }
    }

    for (auto &dmg_info : m_DmgInfos) {
        assert(dmg_info.attacker);
        assert(dmg_info.victim);
        dmg_info.attacker->DealDamage(*dmg_info.victim);
    }
    m_DmgInfos.clear();


    if (m_EnemyTower->IsDead())
    {
        GameOver(true);
        Reset();
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    }
    if (m_CatTower->IsDead())
    {
        GameOver(false);
        Reset();
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    }

    const auto IsDead = [](auto &e) -> bool {
        return e.IsDead() && e.GetState() != EntityState::HITBACK;
    };
    const auto dead_cat_it =
        std::remove_if(m_Cats.begin(), m_Cats.end(), IsDead);
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

void BattleScene::Reset() {
    m_Cats.clear();
    m_Enemies.clear();
    
    assert(m_Cats.capacity() == s_MaxEntityCount);
    assert(m_Enemies.capacity() == s_MaxEntityCount);

    AddCat(CatType::CAT_TOWER, 1);
    m_CatTower = std::addressof(m_Cats[0]);

    AddEnemy(EnemyType::ENEMY_TOWER, 3.0f);
    m_EnemyTower = std::addressof(m_Enemies[0]);
}

void BattleScene::GameOver(const bool cat_won) {
    if (cat_won) {
        LOG_DEBUG("You Win!");
    } else {
        LOG_DEBUG("You Lose!");
    }
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
        // cat.Draw(m_CatImage[static_cast<size_t>(cat.GetCatType())]);
        cat.Draw(m_CatImage[0]);
    }
    for (const auto &enemy : m_Enemies) {
        // enemy.Draw(m_EnemyImage[static_cast<size_t>(enemy.GetEnemyType())]);
        enemy.Draw(m_EnemyImage[0]);
    }
}

void BattleScene::CatAttack(Cat &cat) {
    const auto IsInRange = [](HitBox a, float p) -> bool {
        return a.low <= p && p <= a.high;
    };
    const auto hitbox = cat.GetHitBox();
    if (cat.IsSingleTarget()) {
        Entity *target = nullptr;
        for (auto& e : m_Enemies) {
            if (!IsInRange(hitbox, e.GetPosX())) {
                continue;
            }
            if (!target || target->GetPosX() < e.GetPosX()) {
                target = std::addressof(e);
            }
        }
        if (target) {
            m_DmgInfos.push_back(DamageInfo{std::addressof(cat), target});
        }
    } else {
        for (auto &e : m_Enemies) {
            if (IsInRange(hitbox, e.GetPosX())) {
                m_DmgInfos.push_back(DamageInfo{std::addressof(cat),
                                                std::addressof(e)});
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
        Entity *target = nullptr;
        for (auto& cat : m_Cats) {
            if (!IsInRange(hitbox, cat.GetPosX())) {
                continue;
            }
            if (!target || target->GetPosX() < cat.GetPosX()) {
                target = std::addressof(cat);
            }
        }
        if (target) {
            m_DmgInfos.push_back(DamageInfo{std::addressof(enemy), target});
        }
    } else {
        for (auto &cat : m_Cats) {
            if (IsInRange(hitbox, cat.GetPosX())) {
                m_DmgInfos.push_back(DamageInfo{std::addressof(enemy), std::addressof(cat)});
            }
        }
    }
}

void BattleScene::AddCat(const CatType type, const int level) {
    assert(m_Cats.size() <= s_MaxEntityCount);
    if (m_Cats.size() == s_MaxEntityCount) {
        return;
    }
    auto& cat = m_Cats.emplace_back(type, level);
    cat.SetPosX(s_CatTowerPosX);
}

void BattleScene::AddEnemy(const EnemyType type, const float modifier) {
    assert(m_Cats.size() <= s_MaxEntityCount);
    if (m_Enemies.size() == s_MaxEntityCount) {
        return;
    }
    auto& e = m_Enemies.emplace_back(type);
    e.SetStatsModifier(modifier);
    e.SetPosX(s_EnemiesTowerPosX);
}
