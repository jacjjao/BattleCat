//
// Created by user on 2024/3/1.
//
#include "GameObjectEx.hpp"

void GameObjectEx::SetScale(float x, float y){
    m_Transform.scale = glm::vec2(x,y);
}

void GameObjectEx::SetScale(glm::vec2 scale){
    m_Transform.scale = scale;
}

void GameObjectEx::SetPosition(float x, float y){
    m_Transform.translation = glm::vec2(x,y);
}

void GameObjectEx::SetPosition(glm::vec2 position){
    m_Transform.translation = position;
}
void GameObjectEx::SetRotation(float r){
    m_Transform.rotation = r;
};

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


