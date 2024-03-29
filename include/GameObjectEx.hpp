//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_GAMEOBJECTEX_HPP
#define BATTLECAT_GAMEOBJECTEX_HPP

#include "Util/GameObject.hpp"

class GameObjectEx : public Util::GameObject{
public:
    using Util::GameObject::GameObject;

    void SetScale(glm::vec2 scale);
    void SetScale(float x, float y);

    void SetPosition(glm::vec2 position);
    void SetPosition(float x, float y);

    void MovePosition(glm::vec2 position);
    void MovePosition(float x, float y);

    void SetRotation(float r);
    void AddRotation(float r);

    void SetTransform(Util::Transform transform);

    void Draw(){ GameObject::Draw(); }
    void Draw(const Util::Transform &transform, const float zIndex);

    [[nodiscard]]
    glm::vec2 GetScale() const;
    [[nodiscard]]
    glm::vec2 GetPosition() const;
    [[nodiscard]]
    float GetRotation() const;
    [[nodiscard]]
    std::shared_ptr<Core::Drawable> GetDrawable() const;
    [[nodiscard]]
    glm::vec2 GetTopLeftPos() const;
    [[nodiscard]]
    glm::vec2 GetBottomRightPos() const;


    virtual ~GameObjectEx() override = default;

};

#endif //BATTLECAT_GAMEOBJECTEX_HPP
