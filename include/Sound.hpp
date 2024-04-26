#ifndef BATTLECAT_SOUND_HPP
#define BATTLECAT_SOUND_HPP
#include "Util/SFX.hpp"

namespace Sounds {
    const std::shared_ptr<Util::SFX> Attack1 = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/attack1.mp3");
    const std::shared_ptr<Util::SFX> Attack2 = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/attack2.mp3");
    const std::shared_ptr<Util::SFX> AttackCastle = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/attackcastle.mp3");
    const std::shared_ptr<Util::SFX> Blocked  = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/blocked .mp3");
    const std::shared_ptr<Util::SFX> BossWave = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/bosswave.mp3");
    const std::shared_ptr<Util::SFX> ButtonClick = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/click.mp3");
    const std::shared_ptr<Util::SFX> Crit = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/critical.mp3");
    const std::shared_ptr<Util::SFX> Death = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/death.mp3");
    const std::shared_ptr<Util::SFX> Defeat = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/defeat.mp3");
    const std::shared_ptr<Util::SFX> Deploy = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/deploy.mp3");
    const std::shared_ptr<Util::SFX> Recharge = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/recharge.mp3");
    const std::shared_ptr<Util::SFX> Reward = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/reward.mp3");
    const std::shared_ptr<Util::SFX> Scrolling = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/scrolling.mp3");
    const std::shared_ptr<Util::SFX> UsingItem = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/using.mp3");
    const std::shared_ptr<Util::SFX> Victory = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/victory.mp3");
}

#endif // BATTLECAT_SOUND_HPP
