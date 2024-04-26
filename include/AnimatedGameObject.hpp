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

    void SetCurrentFrame(std::size_t index);

    void Draw(const Util::Transform &transform, const float zIndex,const size_t img_index);

    std::shared_ptr<Util::Animation> GetAnime();

    std::size_t GetCurrentFrameIndex();

    bool IsPlaying() const;

private:
    std::shared_ptr<Util::Animation> m_Anime;
};
#endif //ANIMATEDGAMEOBJECT_HPP
