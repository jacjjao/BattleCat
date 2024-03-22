//
// Created by user on 2024/3/22.
//

#ifndef BATTLECAT_UNITCARD_HPP
#define BATTLECAT_UNITCARD_HPP

#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

enum class UnitCardState : size_t{
    UNPRESSED = 0,
    PUT_OFF,
    DRAGGING
};

class UnitCard : public GameObjectEx{
public:
    using GameObjectEx::GameObjectEx;

    void Dragging();

    [[nodiscard]]
    bool IsMouseHovering() const;
    [[nodiscard]]
    UnitCardState GetState() const;
private:
    UnitCardState m_State = UnitCardState::UNPRESSED;
    std::shared_ptr<GameObjectEx> m_DragImg =
        std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(RESOURCE_DIR"/cats/unit.png"),3);


};

#endif // BATTLECAT_UNITCARD_HPP
