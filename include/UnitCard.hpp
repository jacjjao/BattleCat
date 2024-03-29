//
// Created by user on 2024/3/22.
//

#ifndef BATTLECAT_UNITCARD_HPP
#define BATTLECAT_UNITCARD_HPP

#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Draggable.hpp"
#include "GameButton.hpp"

class UnitCard : public Draggable , public GameObjectEx{
public:
    UnitCard(std::unique_ptr<Core::Drawable> drawable,const float zIndex,bool CopyImg = true){
        std::shared_ptr<Core::Drawable> drawable_shared = std::move(drawable);
        SetDrawable(drawable_shared);
        SetZIndex(zIndex);
        m_DragImg = std::make_shared<GameObjectEx>();
        m_DragImg->SetDrawable(drawable_shared);
        m_DragImg->SetZIndex(zIndex+0.3f);
        m_DragImg->SetScale(1.4f,1.4f);
        OriginalVisible = CopyImg;
    };

    void SetDragImgScale(float x,float y);

    void Dragging() override;

    void Unpressed() override;

    void Put_OFF() override;

    bool IsMouseHovering() override;

private:
    std::shared_ptr<GameObjectEx> m_DragImg;
    bool OriginalVisible;
};

#endif // BATTLECAT_UNITCARD_HPP
