#include "Stage.hpp"

std::optional<std::tuple<EnemyType, float, bool>>
EnemyDispatcher::Update(double tower_health_percent, double total_time,
                        double dt) {
    if (tower_health_percent > towerHealthPercent) {
        return std::nullopt;
    } 
    if ((initAppearTime != s_Infinite && total_time < initAppearTime) ||
        limit <= 0) {
        return std::nullopt;
    }
    if (!m_FirstDispatch) {
        m_Timer.Start();
        m_FirstDispatch = true;
        --limit;
    } else {
        m_Timer.Update(dt);
        if (!m_Timer.IsTimeOut()) {
            return std::nullopt;
        } 
        --limit;
        m_Timer.Start();
    }
    return {{type, modifier, knockCats}};
}

void EnemyDispatcher::SetTimeOutDur(double time_out_dur) {
    m_Timer.SetTimeOutDur(time_out_dur);
}

Stage StageFactory::CreateStage(Stages which) {
    switch (which) {
    case Stages::LEVEL1:
        return CreateLevel1();
    
    default:
        throw std::invalid_argument("Invalid Stages enum");
    }
}

Stage StageFactory::CreateLevel1() {
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.limit = 1;
        stage.dispatchers.push_back(std::move(ed));
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(std::move(ed));
    }
    {
        EnemyDispatcher test;
        test.initAppearTime = EnemyDispatcher::s_Infinite;
        test.towerHealthPercent = 0.95;
        test.limit = 1;
        test.knockCats = true;
        stage.dispatchers.push_back(std::move(test));
    }

    return stage;
}
