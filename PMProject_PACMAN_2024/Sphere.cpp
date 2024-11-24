#include <GL/freeglut.h>
#include "Sphere.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Sphere::Sphere(float r, int sl, int st, float sc) {
    radius = r;
    slice = sl;
    stack = st;
    scale = sc;
}

void Sphere::setRadius(float r) {
    radius = r;
}
float Sphere::getRadius() const {
    return radius;
}
void Sphere::setSlice(int sl) {
    slice = sl;
}
void Sphere::setStack(int st) {
    stack = st;
}

void Sphere::setIndexPosition(int x, int y) {
    idxPos[0] = x;
    idxPos[1] = y;
    center = Vector3f(LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE + BLOCK_SIZE / 2,
        TOP_BOUNDARY - idxPos[1] * BLOCK_SIZE - BLOCK_SIZE / 2,
        0);
}

bool Sphere::isIndexPositionUpdated() const {
    return bInxPosUpdated;
}

int Sphere::getXIndex() const {
    return idxPos[0];
}

int Sphere::getYIndex() const {
    return idxPos[1];
}


void Sphere::setCurrentDirection(DIRECTION d) {
    currDirection = d;
}

void Sphere::setNextDirection(DIRECTION d) {
    nextDirection = d;
}

Sphere::DIRECTION Sphere::getCurrentDirection() const {
    return currDirection;
}

Sphere::DIRECTION Sphere::getNextDirection() const {
    return nextDirection;
}

void Sphere::updateDirection() {
    if (nextDirection != NONE) {
        currDirection = nextDirection;
        nextDirection = NONE;
    }
}

void Sphere::move() {
    switch (getCurrentDirection()) {
    case Sphere::NONE:
        setVelocity(0, 0, 0);
        break;
    case Sphere::RIGHT:
        setVelocity(MOVE_SPEED * scale, 0, 0);
        break;
    case Sphere::LEFT:
        setVelocity(-MOVE_SPEED * scale, 0, 0);
        break;
    case Sphere::UP:
        setVelocity(0, MOVE_SPEED * scale, 0);
        break;
    case Sphere::DOWN:
        setVelocity(0, -MOVE_SPEED * scale, 0);
        break;
    }

    center = center + velocity;

    float xFromIdx = LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE + BLOCK_SIZE / 2; //xÁÂÇ¥
    float yFromIdx = TOP_BOUNDARY - idxPos[1] * BLOCK_SIZE - BLOCK_SIZE / 2; //yÁÂÇ¥
    bInxPosUpdated = false;

    if (velocity[0] > 0.0f) {
        if (center[0] - radius * 2.0f >= xFromIdx) {
            if (idxPos[0] < NUM_COL - 1) {
                setIndexPosition(idxPos[0] + 1, idxPos[1]);
            }
            else
                setIndexPosition(0, idxPos[1]);
            bInxPosUpdated = true;
        }
    }

    else if (velocity[0] < 0.0f) {
        if (center[0] + radius * 2.0f <= xFromIdx) {
            if (idxPos[0] > 0) {
                setIndexPosition(idxPos[0] - 1, idxPos[1]);
            }
            else
                setIndexPosition(NUM_COL - 1, idxPos[1]);
            bInxPosUpdated = true;
        }
    }

    if (velocity[1] > 0.0f) {
        if (center[1] - radius * 2.0f >= yFromIdx) {
            if (idxPos[1] > 0) {
                setIndexPosition(idxPos[0], idxPos[1] - 1);
            }
            else
                setIndexPosition(idxPos[0], NUM_ROW - 1);
            bInxPosUpdated = true;
        }
    }

    else if (velocity[1] < 0.0f) {
        if (center[1] + radius * 2.0f <= yFromIdx) {
            if (idxPos[1] < NUM_ROW - 1) {
                setIndexPosition(idxPos[0], idxPos[1] + 1);
            }
            else
                setIndexPosition(idxPos[0], 0);
            bInxPosUpdated = true;
        }
    }
}

void Sphere::draw() const {
    glShadeModel(GL_SMOOTH);

    GLfloat emissionArr[4] = {
        mtl.getEmission()[0], mtl.getEmission()[1],
        mtl.getEmission()[2], mtl.getEmission()[3]
    };
    GLfloat ambientArr[4] = {
        mtl.getAmbient()[0], mtl.getAmbient()[1],
        mtl.getAmbient()[2], mtl.getAmbient()[3]
    };
    GLfloat diffuseArr[4] = {
        mtl.getDiffuse()[0], mtl.getDiffuse()[1],
        mtl.getDiffuse()[2], mtl.getDiffuse()[3]
    };
    GLfloat specularArr[4] = {
        mtl.getSpecular()[0], mtl.getSpecular()[1],
        mtl.getSpecular()[2], mtl.getSpecular()[3]
    };
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionArr);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArr);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
    glMaterialf(GL_FRONT, GL_SHININESS, mtl.getShininess());

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glutSolidSphere(radius, slice, stack);
    glPopMatrix();
}


PacMan::PacMan(float r, int sl, int st, float sc, bool bCol) : Sphere(r, sl, st, sc), bCollided(bCol) {
    setCurrentDirection(NONE);
    setNextDirection(NONE);
    life = 0;
    angle = 0;
}

void PacMan::setCollided(bool bCol) {
    bCollided = bCol;
}
bool PacMan::isCollided()const {
    return bCollided;
}
void PacMan::setLife(int l) {
    life = l;
}
int PacMan::getLife()const {
    return life;
}
void PacMan::setAngle(float ang) {
    angle = ang;
}
float PacMan:: getAngle()const {
    return angle;
}
void PacMan::draw() const {
    glShadeModel(GL_SMOOTH);

    GLfloat emissionArr[4] = {
        mtl.getEmission()[0], mtl.getEmission()[1],
        mtl.getEmission()[2], mtl.getEmission()[3]
    };
    GLfloat ambientArr[4] = {
        mtl.getAmbient()[0], mtl.getAmbient()[1],
        mtl.getAmbient()[2], mtl.getAmbient()[3]
    };
    GLfloat diffuseArr[4] = {
        mtl.getDiffuse()[0], mtl.getDiffuse()[1],
        mtl.getDiffuse()[2], mtl.getDiffuse()[3]
    };
    GLfloat specularArr[4] = {
        mtl.getSpecular()[0], mtl.getSpecular()[1],
        mtl.getSpecular()[2], mtl.getSpecular()[3]
    };
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionArr);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArr);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
    glMaterialf(GL_FRONT, GL_SHININESS, mtl.getShininess());

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glutSolidSphere(radius, slice, stack);
    glPopMatrix();
}

Ghost::Ghost(float r, int sl, int st, float sc, STATE s) : Sphere(r, sl, st, sc), state(s) {
    setCurrentDirection(NONE);
    setNextDirection(NONE);
}
void Ghost::setScale(float sc) {
    scale = sc;
}
float Ghost::getScale()const {
    return scale;
}
void Ghost::setState(STATE s) {
    state = s;
}

Ghost::STATE Ghost::getState() const {
    return state;
}
