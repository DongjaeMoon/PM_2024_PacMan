#include "CollisionDetector.h"
#include <iostream>

using namespace std;

bool CollisionDetector::operator()(const PacMan& pacman, const Map& map) {
    int xIdx = pacman.getXIndex();
    int yIdx = pacman.getYIndex();

    int next_xIdx = 0;
    int next_yIdx = 0;

    switch (pacman.getCurrentDirection()) {
    case Sphere::NONE:
        next_xIdx = xIdx;
        next_yIdx = yIdx;
        break;
    case Sphere::LEFT:
        next_xIdx = xIdx - 1;
        next_yIdx = yIdx;
        break;
    case Sphere::RIGHT:
        next_xIdx = xIdx + 1;
        next_yIdx = yIdx;
        break;
    case Sphere::UP:
        next_xIdx = xIdx;
        next_yIdx = yIdx - 1;
        break;
    case Sphere::DOWN:
        next_xIdx = xIdx;
        next_yIdx = yIdx + 1;
        break;
    }

    if (map.getBlock(next_yIdx, next_xIdx).isPassable()) return 0;
    else return 1;
}

bool CollisionDetector::operator()(const PacMan& pacman, const Ghost& ghost) {
    Vector3f c = pacman.getCenter() - ghost.getCenter();
    float r = pacman.getRadius() + ghost.getRadius();
    if (dotProduct(c, c) < 0.8 * r * r) return true;
    else return false;
}

bool CollisionDetector::operator()(const PacMan& pacman, const Dot& dot) {
    Vector3f c = pacman.getCenter() - dot.getCenter();
    float r = pacman.getRadius() + dot.getRadius();
    if (dotProduct(c, c) < r * r) return true;
    else return false;
}
bool CollisionDetector::operator()(const PacMan& pacman, const Item& item) {
    Vector3f c = pacman.getCenter() - item.getCenter();
    float r = pacman.getRadius() + item.getRadius();
    if (dotProduct(c, c) < r * r) return true;
    else return false;
}
