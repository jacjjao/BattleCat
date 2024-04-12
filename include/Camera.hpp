#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Util/Transform.hpp"
#include "Util/Image.hpp"

class Camera
{
public:
    void Update();

    void Reset();

    Util::Transform GetTransform() const;

    void SetTransform(const Util::Transform &t);

private:
    void Move(float dx);
    glm::vec2 UpdateMousePos();

    Util::Transform m_Transform;

    glm::vec2 m_LastMousePos;
};

#endif // CAMERA_HPP
