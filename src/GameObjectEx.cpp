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

void GameObjectEx::SetPosition(float x, float y){
    m_Transform.translation = glm::vec2(x,y);
}

void GameObjectEx::SetPosition(glm::vec2 position){
    m_Transform.translation = position;
}

void GameObjectEx::MovePosition(float x, float y){
    m_Transform.translation = glm::vec2(GetPosition().x + x ,GetPosition().y + y);
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);;
        childEx->MovePosition(x,y);
    }
}

void GameObjectEx::MovePosition(glm::vec2 position){
    m_Transform.translation = GetPosition() + position;
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);;
        childEx->MovePosition(position);
    }
}

void GameObjectEx::SetRotation(float r){
    m_Transform.rotation = r;
}

void GameObjectEx::AddRotation(float r){
    m_Transform.rotation = GetRotation() + r;
    for(std::shared_ptr<GameObject> &child : m_Children){
        auto childEx = std::static_pointer_cast<GameObjectEx>(child);;
        childEx->AddRotation(r);
    }
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

/*void GameObjectEx::PlayShapeAnime(std::initializer_list<int> &frames,std::size_t interval) {
    static int frame;
    frame+=1;
}*/


