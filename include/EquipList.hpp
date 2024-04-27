#ifndef BATTLECAT_EQUIPLIST_HPP
#define BATTLECAT_EQUIPLIST_HPP

#define MAXEQUIP 10

#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Draggable.hpp"
#include "GameButton.hpp"
//#include "CatList.hpp"

#include <sstream>
#include <iostream>

class UnitCard;
class EquipCard : public Draggable{
friend class EquipScene;
public:
    EquipCard(unsigned int unitnum,float zIndex,bool form);

    void Transform();
    void SetPos(float x,float y);

    [[nodiscard]]
    unsigned int GetUnitNum() const{ return m_UnitNum;};

    std::weak_ptr<UnitCard> m_UnitCard;

private:
    void Unpressed() override;
    void Dragging() override;
    void Put_OFF() override;
    void PickUp() override;
    bool IsMouseHovering() override;

    std::shared_ptr<GameObjectEx> m_uni1;
    std::shared_ptr<GameObjectEx> m_uni2;
    std::shared_ptr<GameObjectEx> m_curruni;
    short m_FrameTimer = 0;
    Util::Transform m_DragTrans;
    unsigned int m_UnitNum = 0;
    bool m_form;

};

//-------------------------------------------------------------------------------
class EquipList{
friend class EquipScene;
public:
    EquipList() = delete;

    static std::vector<std::shared_ptr<EquipCard>> GetEquipList(){
        return m_equiplist;
    };

private:
    static inline std::vector<std::shared_ptr<EquipCard>> m_equiplist = []()->std::vector<std::shared_ptr<EquipCard>>{
        std::vector<std::shared_ptr<EquipCard>> eq_list;
        eq_list.reserve(MAXEQUIP);
        return eq_list;
    }();
};



#endif // BATTLECAT_EQUIPLIST_HPP
