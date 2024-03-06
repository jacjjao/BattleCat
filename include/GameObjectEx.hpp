//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_GAMEOBJECTEX_HPP
#define BATTLECAT_GAMEOBJECTEX_HPP

#include "Util/GameObject.hpp"

class GameObjectEx : public Util::GameObject{
public:
    GameObjectEx() = default;
    GameObjectEx(std::unique_ptr<Core::Drawable> drawable, const float zIndex,
                 const bool visible = true,
                 const std::vector<std::shared_ptr<GameObject>> &children =
                     std::vector<std::shared_ptr<GameObject>>())
        : Util::GameObject(std::move(drawable),zIndex,visible,children){};


    virtual void SetScale(float x, float y);
    virtual void SetScale(glm::vec2 scale);
    virtual void SetPosition(float x, float y);
    virtual void SetPosition(glm::vec2 position);
    virtual void MovePosition(float x, float y);
    virtual void MovePosition(glm::vec2 position);
    virtual void SetRotation(float r);
    virtual void AddRotation(float r);
    virtual void SetTransform(Util::Transform transform);

    [[nodiscard]]
    glm::vec2 GetScale() const;
    [[nodiscard]]
    glm::vec2 GetPosition() const;
    [[nodiscard]]
    float GetRotation() const;

    ~GameObjectEx() override = default;

};

#endif //BATTLECAT_GAMEOBJECTEX_HPP
