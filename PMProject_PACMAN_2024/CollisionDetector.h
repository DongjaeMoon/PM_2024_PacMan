#pragma once

#include "Sphere.h"
#include "Map.h"

class CollisionDetector
{
public:
    bool operator()(const PacMan& pacman, const Map& map);
    bool operator()(const PacMan& pacman, const Ghost& ghost);

    bool operator()(const PacMan& pacman, const Dot& dot);
    bool operator()(const PacMan& pacman, const Item& item);
};

