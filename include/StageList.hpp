#ifndef STAGE_LIST_HPP
#define STAGE_LIST_HPP

#include <vector>
#include "StageCard.hpp"

class StageList {
public:
    explicit StageList();

    void Draw();

private:
    static constexpr int STAGE_COUNT = 10;

    int m_CurStage = 0;
    
    std::vector<StageCard> m_Cards;
};

#endif // STAGE_LIST_HPP
