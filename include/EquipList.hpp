#ifndef BATTLECAT_EQUIPLIST_HPP
#define BATTLECAT_EQUIPLIST_HPP

#define MAXEQUIP 10

#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Draggable.hpp"
#include "GameButton.hpp"
#include <sstream>
#include <iostream>

class EquipCard : public Draggable{
friend class EquipScene;
public:
    EquipCard(unsigned int unitnum,const float zIndex){
        const int UnitNumLength = 3;
        m_UnitNum = unitnum;
        std::stringstream uni1_img;
        uni1_img << RESOURCE_DIR"/cats/uni/f/" << "uni" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_f00.png";
        std::stringstream uni2_img;
        uni2_img << RESOURCE_DIR"/cats/uni/c/" << "uni" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_c00.png";
        m_uni1 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(uni1_img.str()),zIndex);
        m_uni2 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(uni2_img.str()),zIndex);

        m_curruni = m_uni1;
    };

    void Setform();
    void SetPos(float x,float y);

    unsigned int GetUnitNum(){ return m_UnitNum;};

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
