//
// Created by user on 2024/3/22.
//

#ifndef BATTLECAT_UNITCARD_HPP
#define BATTLECAT_UNITCARD_HPP

#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Draggable.hpp"

class UnitCard : public Draggable , public GameObjectEx{
public:
    UnitCard(std::unique_ptr<Core::Drawable> drawable,const float zIndex){
        auto shared_ptr_drawable = static_cast<std::shared_ptr<Core::Drawable>>(drawable.get());
        SetDrawable(std::move(drawable));
        SetZIndex(zIndex);
        m_DragImg = std::make_shared<GameObjectEx>();
        m_DragImg->SetDrawable(shared_ptr_drawable);
        m_DragImg->SetZIndex(zIndex+0.3f);
    };
    void Dragging() override;

    bool IsMouseHovering() override;

private:
    std::shared_ptr<GameObjectEx> m_DragImg;
};

#endif // BATTLECAT_UNITCARD_HPP
