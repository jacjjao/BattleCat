#include "CatList.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Sound.hpp"
#include "EquipList.hpp"
#include "NumberSystem.hpp"

UnitCard::UnitCard(unsigned int unitnum, const float zIndex) {
    const int UnitNumLength = 3;
    SetDrawable(std::make_unique<Util::Image>(RESOURCE_DIR"/cats/unit.png"));
    SetZIndex(zIndex);
    m_DragTrans.scale = glm::vec2(1.4f, 1.4f);

    m_UnitNum = unitnum;
    std::stringstream udi1_img;
    udi1_img << RESOURCE_DIR"/cats/udi/" << "udi" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum<<"_f.png";
    m_udi1 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(udi1_img.str()),zIndex+0.001f);
    m_udi1->SetPosition(-2.0f,32.0f);
    AddChild(m_udi1);

    std::stringstream udi2_img;
    udi2_img << RESOURCE_DIR"/cats/udi/" << "udi" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_c.png";
    m_udi2 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(udi2_img.str()),zIndex+0.001f);
    m_udi2->SetPosition(-2.0f,32.0f);
    AddChild(m_udi2);

    m_udi2->SetVisible(false);
    //m_dollar = 1000;
    m_dollar = BaseCatStats::Stats[unitnum+1].cost;
    //m_dollar = BaseCatStats::TankCat.cost;
    m_currudi = m_udi1;
}

void UnitCard::Transform() {
    //false -> 1 form , true -> 2 form.
    m_form = !m_form;
    m_udi1->SetVisible(!m_form);
    m_udi2->SetVisible(m_form);
}

void UnitCard::Dragging() {
    m_DragTrans.translation = Util::Input::GetCursorPosition();
    if (m_FrameTimer > 0) {
        m_DragTrans.scale += glm::vec2(0.1f,0.1f)*glm::vec2(m_minify ? -1:1);
        m_FrameTimer--;
    }
    Draw(m_DragTrans,m_ZIndex+1.3f);
    {
        unsigned short number = Getlvl();
        glm::vec2 scale = m_DragTrans.scale;
        glm::vec2 rightmost_pos = m_DragTrans.translation + glm::vec2(130*scale.x,-5*scale.y);
        float z = m_ZIndex+ 1.4f;
        unsigned short xoffer = 30 * scale.x;
        NumberSystem::Display(number, rightmost_pos, z,xoffer ,NumberSystem::YellowNumber,scale);
    }
    { // dollars number
        unsigned short number = GetCost();
        glm::vec2 scale = m_DragTrans.scale;
        glm::vec2 rightmost_pos = m_DragTrans.translation + glm::vec2(55*scale.x,-105*scale.y);;
        float z = m_ZIndex+ 1.4f;
        NumberSystem::Display(number,rightmost_pos,z,18*scale.x,NumberSystem::WhiteNumber,scale);
    }
    Util::Transform tmp = m_DragTrans;
    tmp.translation += m_udi1->GetPosition() - this->GetPosition();
}

void UnitCard::Put_OFF() {
    m_minify = false;
    SetVisible(true);
    m_FrameTimer = 0;
    m_DragTrans.scale = glm::vec2(1.4f, 1.4f);
}

void UnitCard::MinifyAnime(){
    if(m_minify || m_DragState == DragState::UNPRESSED){
        return;
    }
    m_DragTrans.scale = glm::vec2(1.35f,1.35f);
    m_FrameTimer = 10;
    m_minify = true;
}

void UnitCard::AmplifyAnime(){
    if(!m_minify || m_DragState == DragState::UNPRESSED){
        return;
    }
    m_DragTrans.scale = glm::vec2(0.35f,0.35f);
    m_FrameTimer = 10;
    m_minify = false;
}

void UnitCard::SetVisible(bool b){
    GameObjectEx::SetVisible(b);
    m_udi1->SetVisible(!m_form && b);
    m_udi2->SetVisible(m_form && b);
}

bool UnitCard::Addlvl() {
    const unsigned short int MAXlvl = 30;
    if(m_lvl >= MAXlvl) {
        return false;
    }
    m_lvl++;
    if (Inuse()){
        m_EquipCard.lock()->Setlvl(m_lvl);
    }
    return true;
}

bool UnitCard::IsMouseHovering(){
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

CatList::CatList() {
    m_TransFormbtn->SetZIndex(2.2f);
    //m_TransFormbtn->SetPosition(185.0f,-280.0f);
    m_TransFormbtn->SetClickSound([]{
        Sounds::ButtonClick->Play();
    });
    m_TransFormbtn->AddButtonEvent([this]{
        auto &unit = m_catlist.at(m_currentunit);
        unit->Transform();
        if (auto eq = unit->m_EquipCard.lock()) {
            eq->Transform();
        }
    });
}

void CatList::UpdateCatList(const float y) const {
    for(unsigned short i=0;i < short(m_catlist.size());i++){
        auto &unit = m_catlist.at(i);
        if(i == m_currentunit){
            unit->SetPosition(0,y);
            unit->SetScale(1.2f,1.2f);
            unit->SetVisible(true);
        }
        else if(std::abs(m_currentunit-i) <= 2){
            unit->SetPosition(float(i-m_currentunit)*285.0f + (i-m_currentunit>=0 ? 70.0f:-70.0f),y - 52);
            unit->SetScale(0.8f,0.8f);
            unit->SetVisible(true);
        }
        else{
            unit->SetVisible(false);
        }
    }
}

void CatList::DrawNumber() const{
    for(int i=m_currentunit-2; i<=m_currentunit+2; i++) {
        if (i < 0 || i >= short(m_catlist.size()) ){
            continue;
        }

        auto &unit = m_catlist.at(i);
        { //lvl number
            unsigned short number = unit->Getlvl();
            float z = unit->GetZIndex() + 0.1f;
            glm::vec2 scale;
            if(i==m_currentunit) {
                scale = {1.2f,1.2f};
            }
            else{
                scale = {0.8f,0.8f};
            }
            glm::vec2 rightmost_pos = unit->GetTransform().translation + glm::vec2(130*scale.x,-5*scale.y);
            NumberSystem::Display(number,rightmost_pos,z,30*scale.x,NumberSystem::YellowNumber,scale);
        }
        { // dollars number
            unsigned short number = unit->GetCost();
            float z = unit->GetZIndex() + 0.1f;
            glm::vec2 scale;
            if(i==m_currentunit) {
                scale = {1.2f,1.2f};
            }
            else{
                scale = {0.8f,0.8f};
            }
            glm::vec2 rightmost_pos = unit->GetTransform().translation + glm::vec2(55*scale.x,-105*scale.y);;
            NumberSystem::Display(number,rightmost_pos,z,18*scale.x,NumberSystem::WhiteNumber,scale);
        }

    }
}



