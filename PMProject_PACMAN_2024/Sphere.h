#pragma once

#include "Shape3D.h"
#include "Material.h"

/* Sphere class */
class Sphere : public Shape3D
{
public:
    enum DIRECTION { NONE, LEFT, UP, RIGHT, DOWN };

    Sphere(float r, int sl, int st, float sc);

    void setRadius(float r);
    float getRadius() const;
    void setSlice(int sl);
    void setStack(int st);

    void setIndexPosition(int x, int y);
    int getXIndex() const;
    int getYIndex() const;
    bool isIndexPositionUpdated() const;

    void setCurrentDirection(DIRECTION d);
    void setNextDirection(DIRECTION d);
    DIRECTION getCurrentDirection() const;
    DIRECTION getNextDirection() const;
    void updateDirection();

    void move();
    virtual void draw() const;

protected:
    float radius;
    int slice;
    int stack;
    float scale; 

    int idxPos[2];
    bool bInxPosUpdated;

    DIRECTION currDirection;
    DIRECTION nextDirection;
};

/* Pacman class */
class PacMan : public Sphere
{
public:
    PacMan(float r, int sl, int st, float sc, bool bCol);

    void setCollided(bool bCol);
    bool isCollided()const;
    void setLife(int l);    
    int getLife()const;
    void setAngle(float ang);
    float getAngle()const;
    virtual void draw() const;

private:
    bool bCollided;
    int life;
    float angle;
};

/* Ghost class */
class Ghost : public Sphere
{
public:
    enum STATE { CHASE, SCATTER, FRIGHTENED_C, FRIGHTENED_S, EATEN_C, EATEN_S };

    Ghost(float r, int sl, int st, float sc, STATE s);
    void setScale(float sc);
    float getScale()const;
    void setState(STATE s);
    STATE getState() const;

private:
    STATE state;
    int initialPos[2];
};