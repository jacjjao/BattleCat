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
        SetDrawable(std::move(drawable));
        SetZIndex(zIndex);
        OriginalVisible = CopyImg;
        m_DragTrans.scale = glm::vec2(1.4f, 1.4f);
    };

    void MinifyAnime();

    void AmplifyAnime();

    void Dragging() override;

    void Unpressed() override;

    void Put_OFF() override;

    bool IsMouseHovering() override;

    unsigned int GetUnitNum(){ return m_unitnum;};

private:
    std::shared_ptr<GameObjectEx> m_unit;
    short m_FrameTimer = 0;
    Util::Transform m_DragTrans;
    bool OriginalVisible;
    bool m_minify = false;
    unsigned int m_unitnum = 0;
};

#endif // BATTLECAT_UNITCARD_HPP
