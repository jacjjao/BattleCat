#include "SharedAnimation.hpp"
#include "Util/Time.hpp"

SharedRc::Animation::Animation(std::initializer_list<std::string> paths) {
    imgs.reserve(paths.size());
    for (const auto &path : paths) {
        imgs.emplace_back(std::make_shared<Util::Image>(path));
    }
}

SharedRc::SharedAnimation::SharedAnimation(const Animation &anime, bool play,
                                           std::size_t interval, bool looping,
                                           std::size_t cooldown)
    : m_State(play ? State::PLAY : State::PAUSE),
      m_BaseTime(Util::Time::GetElapsedTimeMs()),
      m_Interval(interval),
      m_Looping(looping),
      m_Cooldown(cooldown),
      m_Index(0) {
    m_Frames = anime.imgs; // shared the images
}

void SharedRc::SharedAnimation::SetCurrentFrame(std::size_t index) {
    m_Index = index;
    m_BaseTime = Util::Time::GetElapsedTimeMs();
}

void SharedRc::SharedAnimation::Draw(const Util::Transform &transform,
                                     const float zIndex) {
    m_Frames[m_Index]->Draw(transform, zIndex);
    Update();
}

void SharedRc::SharedAnimation::Update() {
    if (m_State == State::PAUSE) {
        LOG_TRACE("[ANI] is pause");
        m_BaseTime = Util::Time::GetElapsedTimeMs();
        return;
    }

    if (!m_Looping && m_HasEnded) {
        LOG_TRACE("[ANI] not loop and is ended");
        m_BaseTime = Util::Time::GetElapsedTimeMs();
        return;
    }

    std::size_t delta = Util::Time::GetElapsedTimeMs() - m_BaseTime;

    // FIXME: maybe less if

    if (!m_Looping) {
        // don't need to consider cooldown when there is no looping
        if (delta > m_Interval) {
            m_BaseTime += (delta / m_Interval) * m_Interval;
            m_Index += delta / m_Interval;

            if (m_Index >= m_Frames.size() - 1) {
                m_HasEnded = true;
                m_Index = m_Frames.size() - 1;
            }
        }

    } else {
        const auto totalSpan = m_Interval * m_Frames.size() + m_Cooldown;

        const auto spanCount = delta / totalSpan;
        m_BaseTime += totalSpan * spanCount;
        delta %= totalSpan;

        auto addFrameCount = 0;

        if (m_Index == m_Frames.size() - 1) {
            if (delta > m_Interval + m_Cooldown) {
                addFrameCount = (delta - m_Cooldown) / m_Interval;

                m_BaseTime += addFrameCount * m_Interval + m_Cooldown;
            }
        } else if (delta > m_Interval) {
            /**
             * ! if the frame drop spans across the cooldown period, the
             * ! animation can become out of sync
             * ! the probability of this happening is very low, so we leave this
             * ! for now
             */
            addFrameCount = delta / m_Interval;

            m_BaseTime += addFrameCount * m_Interval;
        }

        m_Index = (m_Index + addFrameCount) % m_Frames.size();

        // LOG_DEBUG("{} {}", m_Index, delta);
    }
}

SharedRc::SharedAnimatedGameObject::SharedAnimatedGameObject(
    const SharedRc::Animation &anime)
    : m_Anime(std::make_shared<SharedRc::SharedAnimation>(anime, false, 100)) {
    SetDrawable(m_Anime);
}

void SharedRc::SharedAnimatedGameObject::SetLooping(bool looping) {
    m_Anime->SetLooping(looping);
}

void SharedRc::SharedAnimatedGameObject::SetInterval(int interval) {
    m_Anime->SetInterval(interval);
}

void SharedRc::SharedAnimatedGameObject::Play() {
    m_Anime->Play();
}

void SharedRc::SharedAnimatedGameObject::Pause() {
    m_Anime->Pause();
}

void SharedRc::SharedAnimatedGameObject::Reset() {
    m_Anime->Pause();
    m_Anime->SetCurrentFrame(0);
}

void SharedRc::SharedAnimatedGameObject::SetCurrentFrame(std::size_t index) {
    m_Anime->SetCurrentFrame(index);
}

std::size_t SharedRc::SharedAnimatedGameObject::GetCurrentFrameIndex() {
    return m_Anime->GetCurrentFrameIndex();
}

bool SharedRc::SharedAnimatedGameObject::IsPlaying() const {
    return m_Anime->GetState() == SharedAnimation::State::PLAY;
}
