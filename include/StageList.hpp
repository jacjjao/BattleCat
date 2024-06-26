#ifndef STAGE_LIST_HPP
#define STAGE_LIST_HPP

#include <vector>
#include "StageCard.hpp"

class StageList {
public:
    explicit StageList();

    void Update();

    void Draw();

    [[nodiscard]]
    int GetCurStage() const{return m_cur_stage;};

    void Reset();

private:
    static constexpr int STAGE_COUNT = 10;
    static constexpr float CARD_MARGIN = 20.0f;

    float m_MaxCursorX = 0.0f;
    float m_MinCursorX = 0.0f;
    float m_CursorX = 0.0f;
    int m_cur_stage = 0;

    bool m_MouseDown = false;
    glm::vec2 m_PrevMouse;
    
    std::vector<StageCard> m_Cards;
};

#endif // STAGE_LIST_HPP
