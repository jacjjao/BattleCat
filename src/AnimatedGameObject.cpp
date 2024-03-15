#include "AnimatedGameObject.hpp"

AnimatedGameObject::AnimatedGameObject(std::initializer_list<std::string> AnimationPaths)
    : m_Anime(std::make_shared<Util::Animation>(AnimationPaths, false, 500,
                                                false, 0)) {
    SetDrawable(m_Anime);
}

void AnimatedGameObject::SetLooping(const bool looping) {
    m_Anime->SetLooping(looping);
}

void AnimatedGameObject::SetInterval(const int interval) {
    m_Anime->SetInterval(interval);
}

void AnimatedGameObject::Play() {
    m_Anime->Play();
}

void AnimatedGameObject::Pause() {
    m_Anime->Pause();
}

void AnimatedGameObject::Reset(){
    m_Anime->SetCurrentFrame(0);
    m_Anime->Pause();
}


std::size_t AnimatedGameObject::GetCurrentFrameIndex(){
    return m_Anime->GetCurrentFrameIndex();
}

std::shared_ptr<Util::Animation> AnimatedGameObject::GetAnime(){
    return m_Anime;
}