#ifndef STAGE_HPP
#define STAGE_HPP

#include "Enemy.hpp"
#include "Timer.hpp"
#include <vector>

class EnemyDispatcher {
public:
    std::optional<std::tuple<EnemyType, float, bool>>
    Update(double tower_health_percent, double total_time, double dt);

    static constexpr int s_Infinite = std::numeric_limits<int>::max(); // should be big enough to be consider as infinite

    void SetTimeOutDur(double time_out_dur);

    // yes public
    EnemyType type = EnemyType::DOGE;
    double initAppearTime = 0.0;
    double towerHealthPercent = 100.0;
    int limit = s_Infinite;
    float modifier = 1.0f;
    bool knockCats = false;

private:
    Timer m_Timer;
    bool m_FirstDispatch = false;
};

class Stage {
public:
    std::vector<EnemyDispatcher> dispatchers;
};

enum class Stages {
    LEVEL1
};

class StageFactory {
public:
    static Stage CreateStage(Stages which);

private:
    static Stage CreateLevel1();
};

#endif // STAGE_HPP