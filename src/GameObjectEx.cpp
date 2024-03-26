//
// Created by user on 2024/3/1.
//
#include "GameObjectEx.hpp"
#include "Util/Time.hpp"

void GameObjectEx::SetScale(float x, float y){
    m_Transform.scale = glm::vec2(x,y);
}

void GameObjectEx::SetScale(glm::vec2 scale){
    m_Transform.scale = scale;
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
    m_Transform.rotation = r;
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);;
        childEx->SetRotation(r);
    }
}

void GameObjectEx::AddRotation(float r){
    SetRotation(GetRotation() + r);
}

void GameObjectEx::SetTransform(Util::Transform transform){
    m_Transform = transform;
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
};

glm::vec2 GameObjectEx::GetTopLeftPos() const{
    return m_Transform.translation + GetScaledSize()*glm::vec2(-0.5f,0.5f);
};

glm::vec2 GameObjectEx::GetBottomRightPos() const{
    return m_Transform.translation + GetScaledSize()*glm::vec2(0.5f,-0.5f);
};



