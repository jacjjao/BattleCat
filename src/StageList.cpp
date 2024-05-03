#include "StageList.hpp"
#include <cassert>

StageList::StageList() {
    m_Cards.reserve(STAGE_COUNT);
    for (int i = 0; i < STAGE_COUNT; ++i) {
        m_Cards.emplace_back("");
    }
}

void StageList::Draw() {
    assert(0 <= m_CurStage && m_CurStage < m_Cards.size());
    StageCard *prev_card =
        (m_CurStage == 0) ? nullptr : std::addressof(m_Cards[m_CurStage - 1]);
    StageCard *next_card =
        (m_CurStage == m_Cards.size() - 1) ? nullptr : std::addressof(m_Cards[m_CurStage + 1]);


}
