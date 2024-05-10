#include "Stage.hpp"

EnemyDispatcher::EnemyDispatcher() {
    SetTimeOutDur(s_Infinite);
}

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
    case Stages::LEVEL2:
        return CreateLevel2();
    case Stages::LEVEL3:
        return CreateLevel3();
    case Stages::LEVEL4:
        return CreateLevel4();
    case Stages::LEVEL5:
        return CreateLevel5();
    case Stages::LEVEL6:
        return CreateLevel6();
    case Stages::LEVEL7:
        return CreateLevel7();
    case Stages::LEVEL8:
        return CreateLevel8();
    case Stages::LEVEL9:
        return CreateLevel9();
    case Stages::LEVEL10:
        return CreateLevel10();
    case Stages::TEST:
        return CreateLevelTEST();
    
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
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    
    {
        EnemyDispatcher test;
        test.initAppearTime = EnemyDispatcher::s_Infinite;
        test.towerHealthPercent = 1.0;
        test.SetTimeOutDur(3.0);
        test.limit = EnemyDispatcher::s_Infinite;
        test.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(test);
    }

    return stage;
}

Stage StageFactory::CreateLevel2(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel3(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel4(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel5(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel6(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel7(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel8(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel9(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevel10(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevelTEST(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

