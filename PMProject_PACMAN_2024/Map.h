#pragma once

#include "Sphere.h"
#include "Constants.h"

#include <array>
#include <vector>
using namespace std;

/* Block class */
class Block : public Shape3D
{
public:
    Block();
    Block(float x, float y, float z, float w, float h);

    void setWidth(float w);
    void setHeight(float h);
    void setPassable(bool v);

    float getWidth() const;
    float getHeight() const;
    bool isPassable() const;

    virtual void draw() const;

private:
    float width, height;
    bool bPassable;
};
/* Dot class*/
class Dot : public Shape3D {
public:
    Dot();
    Dot(float x, float y, float z, int r);

    void setRadius(int r);

    void setIdx(int x, int y);
    int getRadius() const;
    int getXIdx() const;
    int getYIdx() const;

    virtual void draw() const;

private:
    int radius;
    int xIdx;
    int yIdx;
};
class Item : public Shape3D {
public:
    Item();
    void setRadius(int r);
    void setIdx(int x, int y);
    int getRadius() const;
    int getXIdx() const;
    int getYIdx() const;
    virtual void draw() const;
private:
    int radius;
    int xIdx;
    int yIdx;
};
/* Map class */
class Map
{
public:
    Map();
    void createMap();
    
    const Block& getBlock(int r, int c) const;
    const vector<Dot> getDots() const;
    vector<Item> getItems() const;
    void eraseDot(int idx);
    void eraseItem(int idx);
    void draw() const;

private:
    std::array<std::array<Block, NUM_COL>, NUM_ROW>  blocks;
    vector<Dot> dots;
    vector<Item> items;
    array<array<int, NUM_COL>, NUM_ROW> mapIdx;
};