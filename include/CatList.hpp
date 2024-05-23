#ifndef BATTLECAT_CATLIST_HPP
#define BATTLECAT_CATLIST_HPP

#include "App.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Draggable.hpp"
#include "GameButton.hpp"
//#include "EquipList.hpp"

#include <sstream>
#include <iostream>

class EquipCard;
class UnitCard : public Draggable , public GameObjectEx{
public:
    UnitCard(unsigned int unitnum,float zIndex);

    void Transform();

    void MinifyAnime();

    void AmplifyAnime();

    void SetVisible(bool b);

    bool Addlvl();

    //void SetScale(float x, float y);
    [[nodiscard]]
    unsigned int GetUnitNum() const{ return m_UnitNum;};

    [[nodiscard]]
    bool Getform() const{ return m_form;};

    [[nodiscard]]
    unsigned short int Getlvl() const{ return m_lvl;};

    [[nodiscard]]
    bool Inuse() const {
        if(m_EquipCard.lock()) return true;
        return false;
    };

   std::weak_ptr<EquipCard> m_EquipCard;

private:
    //void Unpressed() override;
    void Dragging() override;
    void Put_OFF() override;
    //void PickUp() override;
    bool IsMouseHovering() override;

private:
    std::shared_ptr<GameObjectEx> m_udi1;
    std::shared_ptr<GameObjectEx> m_udi2;
    std::shared_ptr<GameObjectEx> m_currudi;
    short m_FrameTimer = 0;
    Util::Transform m_DragTrans;
    bool m_minify = false;
    unsigned int m_UnitNum = 0;
    bool m_form = false;
    unsigned short int m_lvl = 1;
    Util::Transform m_NumTrans;
};
//--------------------------------------------------------------------------------------
class CatList{

public:
    virtual ~CatList() = default;
    CatList();

private:
    static std::vector<std::shared_ptr<UnitCard>> Init(){
        std::vector<std::shared_ptr<UnitCard>> catlist;
        catlist.reserve(MAXUNITS);
        for(unsigned int i=0;i<MAXUNITS;i++){
            catlist.emplace_back(std::make_shared<UnitCard>(i,1.9f));
        }
        return catlist;
    }

protected:
    unsigned short int m_currentunit = 0;
    void UpdateCatList(float y) const;
    void DrawNumber() const;
    static inline std::vector<std::shared_ptr<UnitCard>> m_catlist = Init();
    std::shared_ptr<GameButton> m_TransFormbtn = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/transform.png");
};

#endif // BATTLECAT_CATLIST_HPP
