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
    m_Anime->Pause();
    m_Anime->SetCurrentFrame(0);
}

void AnimatedGameObject::SetCurrentFrame(std::size_t index) {
    m_Anime->SetCurrentFrame(index);
}

void AnimatedGameObject::Draw(const Util::Transform &transform, const float zIndex,const size_t img_index) {
    m_Anime->SetCurrentFrame(img_index);
    m_Anime->Draw(transform, zIndex);
    Reset();
}

std::size_t AnimatedGameObject::GetCurrentFrameIndex(){
    return m_Anime->GetCurrentFrameIndex();
}

std::shared_ptr<Util::Animation> AnimatedGameObject::GetAnime(){
    return m_Anime;
}