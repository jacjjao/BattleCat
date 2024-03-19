#ifndef ANIMATEDGAMEOBJECT_HPP
#define ANIMATEDGAMEOBJECT_HPP

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

    void Reset();

    std::shared_ptr<Util::Animation> GetAnime();

    std::size_t GetCurrentFrameIndex();

private:
    std::shared_ptr<Util::Animation> m_Anime;
};
#endif //ANIMATEDGAMEOBJECT_HPP
