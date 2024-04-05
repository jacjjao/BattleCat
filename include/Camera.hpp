#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Util/Transform.hpp"

class Camera
{
public:
    void Update();

    void Reset();

    Util::Transform GetTransform() const;

private:
    void Move(float dx, float dy);
    glm::vec2 UpdateMousePos();

    Util::Transform m_Transform;

    glm::vec2 m_LastMousePos;
};

#endif // CAMERA_HPP
