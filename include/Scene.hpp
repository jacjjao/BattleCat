//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_SCENE_HPP
#define BATTLECAT_SCENE_HPP


#include "Util/BGM.hpp"

class Scene{

public:
    virtual ~Scene() = default;
    virtual void Update() = 0;
};

#endif //BATTLECAT_SCENE_HPP
