#ifndef BATTLE_BACKGROUND_HPP
#define BATTLE_BACKGROUND_HPP

#include "Util/Image.hpp"
#include "Camera.hpp"

class BattleBackground
{
public:
    BattleBackground(const std::string &path);

    void SetScaleX(float x);

    void Draw(const Camera &cam);

    void ConstraintCam(Camera &cam) const;

private:
    Util::Image m_Img;
    Util::Transform m_Trans;
};

#endif // BATTLE_BACKGROUND_HPP
