//
// Created by user on 2024/3/1.
//
#include "GameObjectEx.hpp"

void GameObjectEx::SetScale(float x, float y){
    m_Transform.scale = glm::vec2(x, y);

}

void GameObjectEx::SetPosition(float x, float y){
    m_Transform.translation = glm::vec2(x,y);
}

GameObjectEx::GameObjectEx() : GameObject(){}
