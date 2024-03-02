#pragma once

#include <string>
#include <vector>

#include "Util/Animation.hpp"
#include "GameObjectEx.hpp"

class AnimatedGameObject : public GameObjectEx {

public:
    explicit AnimatedGameObject(std::initializer_list<std::string> AnimationPaths);

    void SetLooping(bool looping);

    void SetInterval(int interval);

    void Play();

    void Pause();

private:
    std::shared_ptr<Util::Animation> m_Anime;
};
