#include "BattleScene.hpp"
#include "Util/Time.hpp"
#include "DebugUtil/BattleLog.hpp"
#include "App.hpp"
#include <cassert>
#include <algorithm>
#include "Sound.hpp"
#include "LoadingRectangle.hpp"

BattleScene::BattleScene(App &app)
    : m_App(app) {
    m_Cats.reserve(s_MaxEntityCount); // to prevent reallocation
    m_Enemies.reserve(s_MaxEntityCount);
    //------------------------------------------------
    m_ReturnButton = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    m_ReturnButton->SetPosition(-500, 300);
    m_ReturnButton->AddButtonEvent(
        [this] {
            m_App.SwitchScene(App::SceneType::CAT_BASE);
            m_App.SwitchBGM(App::BGMType::CAT_BASE);
    });
    m_Root.AddChild(m_ReturnButton);
    //-----------------------------------------------
    m_OKButton = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/OKbutton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_yellow.png",
             RESOURCE_DIR "/buttons/hover_purple.png"}));
    m_OKButton->SetPosition(0, -200);
    m_OKButton->SetZIndex(5.0f);
    m_OKButton->AddButtonEvent(
        [this] {
            m_App.SwitchScene(App::SceneType::CAT_BASE);
            m_App.SwitchBGM(App::BGMType::CAT_BASE);
        });
    m_OKButton->SetVisible(false);
    //--------------------------------------------
    m_Background.emplace(RESOURCE_DIR "/img/bg/bg_000.png");
    m_Background->SetScaleX(2.0f);
    //--------------------------------------------
    m_victory = std::make_shared<GameObjectEx>();
    m_victory->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/scene/victory.png"));
    m_victory->SetVisible(false);
    m_victory->SetZIndex(100.0f);
    m_Root.AddChild(m_victory);
    //-----------------------------------------------------
    m_defeat = std::make_shared<GameObjectEx>();
    m_defeat->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/scene/defeat.png"));
    m_defeat->SetVisible(false);
    m_defeat->SetZIndex(100.0f);
    m_Root.AddChild(m_defeat);

    DeployButton::Init();
}

void BattleScene::Update() {
    m_Root.Update();
    if(IsGameOver){
        Util::Transform t;
        t.translation = glm::vec2(0,-200.0f);
        m_OKButton->Draw(t,5.0f);
        m_OKButton->Update();
        Draw();
        return;
    }

    m_Cam.Update();
    m_Background->ConstraintCam(m_Cam);

    const auto dt = Util::Time::GetDeltaTime();
    m_TotalTime += dt;

    for (auto &btn : m_CatButton) {
        btn->Update(dt);
    }
    m_ReturnButton->Update();

    m_Wallet->Update(dt);
    m_Work->Update();

    static double velocity_y = 0.0;

    const auto health_percent = m_EnemyTower->GetHealthPercent();
    for (auto &ed : m_Stage.dispatchers) {
        auto e = ed.Update(health_percent, m_TotalTime, dt);
        if (!e) {
            continue;
        }
        auto &[type, modifier, knock] = *e;
        AddEnemy(type, modifier);
        if (knock && !m_OnBossAppear) {
            m_OnBossAppear = true;
            m_CatY = 0.0;
            velocity_y = 200.0;
            for (size_t i = 1; i < m_Cats.size(); ++i) {
                m_Cats[i].ResetState();
            }
            Sounds::BossWave->Play();
        }
    }

    if (m_OnBossAppear) {
        constexpr double gravity = -200;
        constexpr double velocity_x = 100.0;
        velocity_y += gravity * dt;
        m_CatY += velocity_y * dt;
        if (m_CatY <= 0.0) {
            m_CatY = 0.0;
            m_OnBossAppear = false;
        }
        for (size_t i = 1; i < m_Cats.size(); ++i) {
            m_Cats[i].Move(velocity_x * dt);
        }
    }

    if (!m_OnBossAppear) {
        CatStartAttack();
        for (auto &cat : m_Cats) {
            cat.Walk(dt);
        }
    }

    if (!m_OnBossAppear) {
        EnemyStartAttack();
    }
    for (auto &enemy : m_Enemies) {
        enemy.Walk(dt);
    }

    if (!m_OnBossAppear) {
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
    }

    if (m_EnemyTower->IsDead()) {
        GameOver(true);
        Sounds::Victory->Play();
        return;
    }
    if (m_CatTower->IsDead()) {
        GameOver(false);
        Sounds::Defeat->Play();
        return;
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
    m_victory->SetVisible(false);
    m_defeat->SetVisible(false);
    IsGameOver = false;
    m_ReturnButton->SetVisible(true);
    m_OKButton->SetVisible(false);

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

    m_Wallet.emplace(1);
    m_Work.emplace(*m_Wallet);

    m_Cam.Reset();

    CreateUnitButtons();
    m_OnBossAppear = false;
}

void BattleScene::GameOver(const bool cat_won) {
    IsGameOver = true;
    m_OKButton->SetVisible(true);
    m_ReturnButton->SetVisible(false);
    m_App.PauseBGM();
    if (cat_won) {
        LOG_DEBUG("You Win!");
        m_victory->SetVisible(true);
        if (m_Enemies.size() > 1) {
            m_Enemies.erase(m_Enemies.begin()+1, m_Enemies.end()); // Keep tower.
        }
    }
    else {
        LOG_DEBUG("You Lose!");
        m_defeat->SetVisible(true);
        if(m_Cats.size() > 1){
            m_Cats.erase(m_Cats.begin()+1,m_Cats.end());//Keep tower.
        }
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
    if (!IsGameOver) {
        DeployButton::DrawStates();
        m_Wallet->Draw();
        m_Work->Draw();

        for (auto &btn : m_CatButton) {
            btn->Draw();
        }
    }

    {
        auto t = m_Cam.GetTransform();
        m_Cats[0].Draw(t);
        t.translation.y += m_CatY;
        for (size_t i = 1; i < m_Cats.size(); ++i) {
            auto gt = m_Cam.GetTransform();
            gt.translation.y += m_CatY;
            m_Cats[i].Draw(
                gt);
        }
    }
    for (auto &enemy : m_Enemies) {
        enemy.Draw(m_Cam.GetTransform());
    }

    m_Background->Draw(m_Cam);
    {
        auto currHP = static_cast<unsigned int>(m_EnemyTower->GetFullHealth()*m_EnemyTower->GetHealthPercent());
        glm::vec2 pos = glm::vec2(m_Cam.GetTransform().translation.x - 450.0f, 50);
        NumberSystem::Display(currHP,pos, 3.0f, 14,NumberSystem::SmallNumber);
    }
    {
        auto currHP = static_cast<unsigned int>(m_CatTower->GetFullHealth()*m_CatTower->GetHealthPercent());
        glm::vec2 pos = glm::vec2(m_Cam.GetTransform().translation.x + 500.0f, 50);
        NumberSystem::Display( currHP,pos,3.0f,14,NumberSystem::SmallNumber);
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
    if (m_Cats.size() >= s_MaxEntityCount) {
        return;
    }
    auto& cat = m_Cats.emplace_back(type, level);
    constexpr float y_low = -145.0f;
    constexpr float y_high = -125.0f;
    cat.SetY(y_low, y_high);
    cat.SetPosX(s_CatTowerPosX);
}

void BattleScene::AddEnemy(const EnemyType type, const float modifier) {
    assert(m_Cats.size() <= s_MaxEntityCount);
    if (m_Enemies.size() >= s_MaxEntityCount) {
        return;
    }
    auto& e = m_Enemies.emplace_back(type);
    constexpr float y_low = -145.0f;
    constexpr float y_high = -125.0f;
    e.SetY(y_low, y_high);
    e.SetStatsModifier(modifier);
    e.SetPosX(s_EnemiesTowerPosX);
    LOG_DEBUG("Add Enemy at time {}", m_TotalTime);
}

void BattleScene::CreateUnitButtons() {
    constexpr int unit_img_width = 110;
    constexpr int margin_x = 10;
    constexpr int start_x = -(unit_img_width * 2 + margin_x * 2);
    constexpr int start_y = -220;

    int x = start_x;
    int y = start_y;

    m_CatButton.clear();
    m_CatButton.resize(10);
    for (int i = 0; i < 10; ++i) {
        m_CatButton[i] =
            std::make_shared<DeployButton>(RESOURCE_DIR "/img/uni/uni.png");
    }

    auto equiplist = EquipList::GetEquipList();

    for (int i = 0; i < EquipList::Size ; ++i) {
        auto unitnum = equiplist.at(i)->GetUnitNum();
        auto cat_type = equiplist.at(i)->GetCatType();
        auto cat_lvl = equiplist.at(i)->GetUnitLVL();
        std::stringstream uni_img;
        if(static_cast<int>(cat_type) <= 24){
            uni_img << RESOURCE_DIR"/img/uni/f/" << "uni" << std::string(3 - std::to_string(unitnum).length(), '0') << unitnum << "_f00.png";
        }
        else{
            uni_img << RESOURCE_DIR"/img/uni/c/" << "uni" << std::string(3 - std::to_string(unitnum).length(), '0') << unitnum << "_c00.png";
        }
        m_CatButton[i] = std::make_shared<DeployButton>(uni_img.str());

        const auto cost =
            BaseCatStats::Stats[static_cast<size_t>(cat_type)].cost;
        m_CatButton[i]->SetCost(cost);
        m_CatButton[i]->SetCoolDownTime(
            BaseCatStats::Stats[static_cast<size_t>(cat_type)].recharge_time);

        m_CatButton[i]->AddButtonEvent([this, cost, cat_type, i,cat_lvl] {
            if (m_Wallet->CanDeploy(cost)) {
                AddCat(cat_type, static_cast<int>(cat_lvl));
                //printf("%d",static_cast<int>(cat_lvl));
                m_Wallet->Spend(cost);
                m_CatButton[i]->StartCoolDown();
                Sounds::Deploy->Play();
            }
            else{
                Sounds::Blocked->Play();
            }
        });
        m_CatButton[i]->AddButtonEvent([this]{
            Sounds::Scrolling->Play();
        });
    }

    constexpr float row_margin_y = 10.0f;
    for (int row = 0; row < 2; ++row) {
        for (int i = 0; i < 5; ++i) {
            const int idx = row * 5 + i;
            m_CatButton[idx]->SetPosition(x, y);
            x += m_CatButton[idx]->GetScaledSize().x + margin_x;
        }
        y -= m_CatButton[0]->GetScaledSize().y;
        y -= row_margin_y;
        x = start_x;
    }
}
