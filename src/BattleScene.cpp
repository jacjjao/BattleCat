#include "BattleScene.hpp"
#include "Util/Time.hpp"
#include "DebugUtil/BattleLog.hpp"
#include "App.hpp"
#include "Util/Input.hpp"
#include <cassert>
#include <algorithm>

BattleScene::BattleScene(App &app)
    : m_App(app) {
    m_Cats.reserve(s_MaxEntityCount); // to prevent reallocation
    m_Enemies.reserve(s_MaxEntityCount);

    m_CatImage.emplace_back(RESOURCE_DIR "/stages/ec000_tw.png");
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/000/walk.png");
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/001/walk.png");
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/002/walk.png");
    m_CatImage.emplace_back(RESOURCE_DIR "/cats/003/walk.png");

    m_EnemyImage.emplace_back(RESOURCE_DIR "/stages/ec000_tw.png");
    m_EnemyImage.emplace_back(RESOURCE_DIR "/enemys/000/enemy_icon_000.png");

    m_ReturnButton = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    m_ReturnButton->SetPosition(-500, 300);
    m_ReturnButton->AddButtonEvent(
        [this] { m_App.SwitchScene(App::SceneType::CAT_BASE);
    });
    m_Root.AddChild(m_ReturnButton);

    m_Background.emplace(RESOURCE_DIR "/img/bg/bg_000.png");
    m_Background->SetScaleX(2.0f);
}

void BattleScene::Update() {
    m_Cam.Update();
    m_Background->ConstraintCam(m_Cam);

    const auto dt = Util::Time::GetDeltaTime();
    m_TotalTime += dt;
    
    for (auto &btn : m_CatButton) {
        btn->Update(dt);
    }
    m_ReturnButton->Update();
    m_Root.Update();

    m_Wallet->Update(dt);
    
    const auto health_percent = m_EnemyTower->GetHealthPercent();
    for (auto &ed : m_Stage.dispatchers) {
        auto e = ed.Update(health_percent, m_TotalTime, dt);
        if (!e) {
            continue;
        }
        auto& [type, modifier] = *e;
        AddEnemy(type, modifier);
    }

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
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    }
    if (m_CatTower->IsDead())
    {
        GameOver(false);
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

void BattleScene::LoadStage(Stage &stage) {
    m_Cats.clear();
    m_Enemies.clear();

    assert(m_Cats.capacity() == s_MaxEntityCount);
    assert(m_Enemies.capacity() == s_MaxEntityCount);

    AddCat(CatType::CAT_TOWER, 1);
    m_CatTower = std::addressof(m_Cats[0]);
    m_CatTower->SetState(EntityState::IDLE);

    AddEnemy(EnemyType::ENEMY_TOWER, 3.0f);
    m_EnemyTower = std::addressof(m_Enemies[0]);
    m_EnemyTower->SetState(EntityState::IDLE);

    m_Stage = std::move(stage);
    m_TotalTime = 0.0;

    m_Wallet.emplace(3);
    m_Cam.Reset();

    CreateUnitButtons();
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
        cat.Draw(m_Cam.GetTransform(),
                 m_CatImage[static_cast<size_t>(cat.GetCatType())]);
        // cat.Draw(m_Cam.GetTransform(), m_CatImage[0]);
    }
    for (const auto &enemy : m_Enemies) {
        enemy.Draw(m_Cam.GetTransform(),
                   m_EnemyImage[static_cast<size_t>(enemy.GetEnemyType())]);
        // enemy.Draw(m_Cam.GetTransform(), m_EnemyImage[0]);
    }
    m_Wallet->Draw();

    for (auto &btn : m_CatButton) {
        btn->DrawCost();
    }

    m_Background->Draw(m_Cam);
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
    if (m_Cats.size() >= s_MaxEntityCount) {
        return;
    }
    auto& cat = m_Cats.emplace_back(type, level);
    cat.SetPosX(s_CatTowerPosX);
}

void BattleScene::AddEnemy(const EnemyType type, const float modifier) {
    assert(m_Cats.size() <= s_MaxEntityCount);
    if (m_Enemies.size() >= s_MaxEntityCount) {
        return;
    }
    auto& e = m_Enemies.emplace_back(type);
    e.SetStatsModifier(modifier);
    e.SetPosX(s_EnemiesTowerPosX);
    LOG_DEBUG("Add Enemy at time {}", m_TotalTime);
}

void BattleScene::CreateUnitButtons() {

    constexpr int unit_img_width = 110;
    constexpr int margin_x = 10;
    constexpr int start_x = -(unit_img_width * 2 + margin_x * 2);
    constexpr int start_y = -200;

    int x = start_x;
    int y = start_y;

    for (auto &btn : m_CatButton) {
        m_Root.RemoveChild(btn);
    }
    m_CatButton.clear();
    m_CatButton.resize(10);
    for (int i = 0; i < 10; ++i) {
        m_CatButton[i] =
            std::make_shared<DeployButton>(RESOURCE_DIR "/img/uni/f/uni_f.png");
    }

    // Everything is hard-coded for now just for midterm demonstration
    // later in development, we will pass some data to construct the buttons
    
    // cat
    {
        m_CatButton[0] = std::make_shared<DeployButton>(
            RESOURCE_DIR "/img/uni/f/uni000_f00.png");
        const auto cost =
            BaseCatStats::Stats[static_cast<size_t>(CatType::CAT)].cost;
        m_CatButton[0]->SetCost(cost);
        m_CatButton[0]->AddButtonEvent([this, cost] {
            if (m_Wallet->CanDeploy(cost)) {
                AddCat(CatType::CAT, 10);
                m_Wallet->Spend(cost);
                m_CatButton[0]->StartCoolDown();
            }
        });
    }

    // tank cat
    {
        m_CatButton[1] = std::make_shared<DeployButton>(
            RESOURCE_DIR "/img/uni/f/uni001_f00.png");
        const auto cost =
            BaseCatStats::Stats[static_cast<size_t>(CatType::TANK_CAT)].cost;
        m_CatButton[1]->SetCost(cost);
        m_CatButton[1]->AddButtonEvent([this, cost] {
            if (m_Wallet->CanDeploy(cost)) {
                AddCat(CatType::TANK_CAT, 10);
                m_Wallet->Spend(cost);
                m_CatButton[1]->StartCoolDown();
            }
        });
    }

    {
        m_CatButton[2] = std::make_shared<DeployButton>(
            RESOURCE_DIR "/img/uni/f/uni002_f00.png");
        const auto cost =
            BaseCatStats::Stats[static_cast<size_t>(CatType::AXE_CAT)].cost;
        m_CatButton[2]->SetCost(cost);
        m_CatButton[2]->AddButtonEvent([this, cost] {
            if (m_Wallet->CanDeploy(cost)) {
                AddCat(CatType::AXE_CAT, 10);
                m_Wallet->Spend(cost);
                m_CatButton[2]->StartCoolDown();
            }
        });
    }

    {
        m_CatButton[3] = std::make_shared<DeployButton>(
            RESOURCE_DIR "/img/uni/f/uni003_f00.png");
        const auto cost =
            BaseCatStats::Stats[static_cast<size_t>(CatType::CRAZED_GROSS_CAT)].cost;
        m_CatButton[3]->SetCost(cost);
        m_CatButton[3]->AddButtonEvent([this, cost] {
            if (m_Wallet->CanDeploy(cost)) {
                AddCat(CatType::CRAZED_GROSS_CAT, 10);
                m_Wallet->Spend(cost);
            }
        });
    }

    for (int row = 0; row < 2; ++row) {
        for (int i = 0; i < 5; ++i) {
            const int idx = row * 5 + i;
            m_CatButton[idx]->SetPosition(x, y);
            m_CatButton[idx]->SetCoolDownTime(2.0);
            m_Root.AddChild(m_CatButton[idx]);
            x += m_CatButton[idx]->GetScaledSize().x + margin_x;
        }
        y -= m_CatButton[0]->GetScaledSize().y;
        x = start_x;
    }
}
