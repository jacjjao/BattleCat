#ifndef BATTLECAT_SOUND_HPP
#define BATTLECAT_SOUND_HPP
namespace Sounds {
    std::shared_ptr<Util::SFX> ButtonClick = std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds/click.mp3");
}

#endif // BATTLECAT_SOUND_HPP
