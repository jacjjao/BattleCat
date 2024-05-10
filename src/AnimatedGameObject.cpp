#include "AnimatedGameObject.hpp"

AnimatedGameObject::AnimatedGameObject(std::initializer_list<std::string> AnimationPaths)
    : m_Anime(std::make_shared<Util::Animation>(AnimationPaths, false, 500,false, 0)) {
    SetDrawable(m_Anime);
}

AnimatedGameObject::AnimatedGameObject(std::shared_ptr<Util::Animation> anime) : 
    m_Anime(std::move(anime)) {
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
    if(img_index >= m_Anime->GetFrameCount()){
        throw std::invalid_argument("The index \"img_index\" out of range of frames");
    }
    m_Anime->SetCurrentFrame(img_index);
    m_Anime->Draw(transform, zIndex);
    Reset();
}

std::size_t AnimatedGameObject::GetCurrentFrameIndex(){
    return m_Anime->GetCurrentFrameIndex();
}

bool AnimatedGameObject::IsPlaying() const {
    return m_Anime->GetState() == Util::Animation::State::PLAY;
}

std::shared_ptr<Util::Animation> AnimatedGameObject::GetAnime(){
    return m_Anime;
}