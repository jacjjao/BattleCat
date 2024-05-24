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

// https://mygamatoto.com/stageinfo/300000/Korea
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
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(8.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300002/China
Stage StageFactory::CreateLevel2(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(6.5);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300005/Philippines
Stage StageFactory::CreateLevel3(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(6.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 40.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(10.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300012/Dubai
Stage StageFactory::CreateLevel4(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(7.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(7.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 40.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(7.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.88;
        ed.SetTimeOutDur(3.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.88;
        ed.SetTimeOutDur(10.0);
        ed.limit = 1;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.9;
        ed.SetTimeOutDur(3.0);
        ed.limit = 6;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.9;
        ed.SetTimeOutDur(10.0);
        ed.limit = 1;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300014/Kenya
Stage StageFactory::CreateLevel5(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(15.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 40.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(30.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 60.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 80.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 133.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(50.0);
        ed.limit = 1;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 280.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(50.0);
        ed.limit = 1;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 0.99;
        ed.SetTimeOutDur(1.0);
        ed.limit = 8;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300015/Madagascar
Stage StageFactory::CreateLevel6(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(15.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(15.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.85;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::GORY;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 0.85;
        ed.SetTimeOutDur(1.5);
        ed.limit = 12;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.6;
        ed.SetTimeOutDur(60.0);
        ed.limit = 1;
        ed.type = EnemyType::GORY;
        ed.knockCats = true;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300016/South%20Africa
Stage StageFactory::CreateLevel7(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(50.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 100.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 102.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 133.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(50.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 133.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(50.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.2;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300020/Turkey
Stage StageFactory::CreateLevel8(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 60.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(90.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 60.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::BAABAA;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 80.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(90.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 80.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(90.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 133.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(90.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::GORY;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300023/Italy
Stage StageFactory::CreateLevel9(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(25.0);
        ed.limit = 1;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 60.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(8.0);
        ed.limit = 1;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 120.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(0.0);
        ed.limit = 1;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 180.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(90.0);
        ed.limit = 1;
        ed.type = EnemyType::GORY;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.9;
        ed.SetTimeOutDur(25.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.9;
        ed.SetTimeOutDur(25.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.9;
        ed.SetTimeOutDur(25.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.9;
        ed.SetTimeOutDur(25.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::GORY;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

// https://mygamatoto.com/stageinfo/300046/Hawaii
Stage StageFactory::CreateLevel10(){
    Stage stage;
    stage.dispatchers.reserve(2);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(2.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::THOSE_GUYS;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(2.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(2.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 60.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(21.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::GORY;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 80.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(40.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SIRSEAL;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 100.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(76.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::LEBOIN;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 120.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(40.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::KANGROO;
        stage.dispatchers.push_back(ed);
    }
    {
        EnemyDispatcher ed;
        ed.initAppearTime = 140.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(77.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::ONEHORN;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

Stage StageFactory::CreateLevelTEST(){
    Stage stage;
    stage.dispatchers.reserve(12);

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::DOGE;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(5.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SNACHE;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(15.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::HIPPOE;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(15.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::PIGGE;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 5.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(15.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::JackiePeng;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 20.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(30.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::GORY;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 10.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(20.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::BAABAA;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 10.0;
        ed.towerHealthPercent = 1.0;
        ed.SetTimeOutDur(30.0);
        ed.limit = EnemyDispatcher::s_Infinite;
        ed.type = EnemyType::SIRSEAL;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.99;
        ed.SetTimeOutDur(60.0);
        ed.limit = 1;
        ed.type = EnemyType::LEBOIN;
        ed.knockCats = true;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.99;
        ed.SetTimeOutDur(60.0);
        ed.limit = 10;
        ed.type = EnemyType::KANGROO;
        stage.dispatchers.push_back(ed);
    }

    {
        EnemyDispatcher ed;
        ed.initAppearTime = 0.0;
        ed.towerHealthPercent = 0.99;
        ed.SetTimeOutDur(60.0);
        ed.limit = 10;
        ed.type = EnemyType::ONEHORN;
        stage.dispatchers.push_back(ed);
    }

    return stage;
}

