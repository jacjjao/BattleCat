#include "GameObjectEx.hpp"
#include "Util/Time.hpp"

void GameObjectEx::SetScale(glm::vec2 scale){
    glm::vec2 pre_scale = m_Transform.scale;
    glm::vec2 pre_pos = m_Transform.translation;
    m_Transform.scale = scale;
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);
        glm::vec2 deltaPos = childEx->GetPosition() - pre_pos;
        glm::vec2 ScaleDiff = scale/pre_scale;
        childEx->SetPosition(pre_pos + deltaPos*ScaleDiff);
        childEx->SetScale(childEx->GetScale()*ScaleDiff);
    }
}

void GameObjectEx::SetScale(float x, float y){
    SetScale(glm::vec2(x,y));
}

void GameObjectEx::SetPosition(glm::vec2 position){
    glm::vec2 pre_pos = m_Transform.translation;
    m_Transform.translation = position;
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);
        glm::vec2 delta = childEx->GetPosition() - pre_pos;
        childEx->SetPosition(position + delta);
    }
}

void GameObjectEx::SetPosition(float x, float y){
    SetPosition(glm::vec2(x,y));
}

void GameObjectEx::MovePosition(glm::vec2 position){
    SetPosition(GetPosition() + position);
}

void GameObjectEx::MovePosition(float x, float y){
    MovePosition(glm::vec2(x,y));
}

void GameObjectEx::SetRotation(float r){
    float pre_r = m_Transform.rotation;
    m_Transform.rotation = r;
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);
        float delta = childEx->GetRotation() - pre_r;
        childEx->SetRotation(r+delta);
    }
}

void GameObjectEx::AddRotation(float r){
    SetRotation(GetRotation() + r);
}

void GameObjectEx::SetTransform(Util::Transform transform){
    m_Transform = transform;
}

void GameObjectEx::Draw(const Util::Transform &transform, const float zIndex){
    if(!m_Visible){
        return;
    }
    m_Drawable->Draw(transform,zIndex);
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);
        auto deltaZ = childEx->GetZIndex() - this->GetZIndex();
        Util::Transform tran;
        tran.translation = transform.translation + (childEx->GetPosition() - this->GetPosition())*(transform.scale/this->GetScale());
        //tran.rotation = transform.rotation + childEx->GetRotation() - this->GetRotation();
        tran.scale = childEx->GetScale()*transform.scale/this->GetScale();
        childEx->Draw(tran,zIndex+deltaZ);
    }
}

glm::vec2 GameObjectEx::GetScale() const{
    return m_Transform.scale;
}

glm::vec2 GameObjectEx::GetPosition() const{
    return m_Transform.translation;
}

float GameObjectEx::GetRotation() const{
    return m_Transform.rotation;
}

std::shared_ptr<Core::Drawable> GameObjectEx::GetDrawable() const{
    return m_Drawable;
}

glm::vec2 GameObjectEx::GetTopLeftPos() const{
    return m_Transform.translation + GetScaledSize()*glm::vec2(-0.5f,0.5f);
}

glm::vec2 GameObjectEx::GetBottomRightPos() const{
    return m_Transform.translation + GetScaledSize()*glm::vec2(0.5f,-0.5f);
}





