#pragma once

#include "CollisionDetector.h"

class CollisionHandler
{
public:
    void operator()(PacMan& pacman, Map& map);
    void operator()(PacMan& pacman, Ghost& ghost);
};

