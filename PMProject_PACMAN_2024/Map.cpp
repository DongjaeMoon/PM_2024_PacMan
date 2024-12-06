#include <GL/freeglut.h>
#include "Map.h"
#include <iostream>

using namespace std;
extern PacMan pacman;

Block::Block() {
    width = height = 0;
}
Block::Block(float x, float y, float z, float w, float h) {
    setCenter(x, y, z);
    width = w;
    height = h;
}

void Block::setWidth(float w) {
    width = w;
}

void Block::setHeight(float h) {
    height = h;
}

void Block::setPassable(bool v) {
    bPassable = v;
}

float Block::getWidth() const {
    return width;
}

float Block::getHeight() const {
    return height;
}

bool Block::isPassable() const {
    return bPassable;
}

void Block::draw() const {
    if (isPassable() == 0) {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(width / 2, -height / 2);
        glVertex2f(width / 2, height / 2);
        glVertex2f(-width / 2, height / 2);
        glEnd();
    }
};

Dot::Dot() {}

Dot::Dot(float x, float y, float z, int r) {
    setCenter(x, y, z);
    setRadius(r);
}

void Dot::setRadius(int r) {
    radius = r;
}

void Dot::setIdx(int x, int y) {
    xIdx = x;
    yIdx = y;
}

int Dot::getRadius() const {
    return radius;
}

int Dot::getXIdx() const {
    return xIdx;
}

int Dot::getYIdx() const {
    return yIdx;
}

void Dot::draw() const {
    glShadeModel(GL_SMOOTH);
    switch (radius) {
    case SMALL_DOT_RADIUS:
        glColor3f(1.0f, 0.7f, 0.0f);

        glPushMatrix();
        glTranslatef(center[0], center[1], center[2]);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

        glPopMatrix();
        break;
    case LARGE_DOT_RADIUS:
        glColor3f(1.0f, 0.5f, 0.0f);

        glPushMatrix();
        glTranslatef(center[0], center[1], center[2]);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= 10; ++i) {
            float angle = 2.0f * M_PI * i / 10;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

        glPopMatrix();
        break;
    }
}
Item::Item(){}
void Item::setRadius(int r) {
    radius = r;
}
void Item::setIdx(int x, int y) {
    xIdx = x;
    yIdx = y;
}
int Item::getRadius() const {
    return radius;
}
int Item::getXIdx() const {
    return xIdx;
}
int Item::getYIdx() const {
    return yIdx;
}
void Item::draw()const {
    glColor3f(0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 4; ++i) {
        float angle = 2.0f * M_PI * i / 4;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}
Map::Map()
    : mapIdx{ {
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
        {{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}},
        {{0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0}},
        {{0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,0}},
        {{0,1,1,1,2,1,1,1,0,0,0,1,1,1,2,1,1,1,0}},
        {{0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,0}},
        {{0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0}},
        {{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}},
        {{0,0,1,1,0,1,1,0,0,3,0,0,1,1,0,1,1,0,0}},
        {{0,0,1,1,0,1,1,0,3,4,3,0,1,1,0,1,1,0,0}},/*middle*/
        {{0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0}},
        {{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}},
        {{0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0}},
        {{0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,0}},
        {{0,1,1,1,2,1,1,1,0,0,0,1,1,1,2,1,1,1,0}},
        {{0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,0}},
        {{0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0}},
        {{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}},
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
    } } // 2 = large dot, 3 = ghost room  4 = item
{
    dots.reserve(NUM_COL * NUM_ROW);
}
void Map::createMap() {
    dots.clear();

    for (int r = 0; r < blocks.size(); r++) {
        for (int c = 0; c < blocks[r].size(); c++) {
            blocks[r][c].setCenter(LEFT_BOUNDARY + c * BLOCK_SIZE + BLOCK_SIZE / 2, TOP_BOUNDARY - r * BLOCK_SIZE - BLOCK_SIZE / 2, 0.0f);
            blocks[r][c].setWidth(BLOCK_SIZE);
            blocks[r][c].setHeight(BLOCK_SIZE);
            if (mapIdx[r][c] != 0)
                blocks[r][c].setPassable(true);
            else blocks[r][c].setPassable(false);
        }
    }

    // Dots
    vector<vector<int>> largeDotIdx;
    vector<vector<int>> smallDotIdx;
    vector<vector<int>> itemIdx;

    for (int r = 0; r < blocks.size(); r++) {
        for (int c = 0; c < blocks[r].size(); c++) {
            if (mapIdx[r][c] == 2) largeDotIdx.push_back({ c,r });
            else if (mapIdx[r][c] == 1) smallDotIdx.push_back({ c,r });
            else if (mapIdx[r][c] == 4) itemIdx.push_back({ c,r });
        }
    }

    for (int i = 0; i < largeDotIdx.size(); i++) {
        int xIdx = largeDotIdx[i][0];
        int yIdx = largeDotIdx[i][1];

        Dot largedot;
        largedot.setCenter(LEFT_BOUNDARY + xIdx * BLOCK_SIZE + BLOCK_SIZE / 2, TOP_BOUNDARY - yIdx * BLOCK_SIZE - BLOCK_SIZE / 2, 0.0f);
        largedot.setRadius(LARGE_DOT_RADIUS);
        largedot.setIdx(xIdx, yIdx);

        dots.push_back(largedot);
    }

    for (int i = 0; i < smallDotIdx.size(); i++) {
        int xIdx = smallDotIdx[i][0];
        int yIdx = smallDotIdx[i][1];

        Dot smalldot;
        smalldot.setCenter(LEFT_BOUNDARY + xIdx * BLOCK_SIZE + BLOCK_SIZE / 2, TOP_BOUNDARY - yIdx * BLOCK_SIZE - BLOCK_SIZE / 2, 0.0f);
        smalldot.setRadius(SMALL_DOT_RADIUS);
        smalldot.setIdx(xIdx, yIdx);

        dots.push_back(smalldot);
    }

    for (int i = 0; i < itemIdx.size(); i++) {
        int xIdx = itemIdx[i][0];
        int yIdx = itemIdx[i][1];

        Item item;
        item.setCenter(LEFT_BOUNDARY + xIdx * BLOCK_SIZE + BLOCK_SIZE / 2, TOP_BOUNDARY - yIdx * BLOCK_SIZE - BLOCK_SIZE / 2, 0.0f);
        item.setRadius(ITEM_RADIUS);
        item.setIdx(xIdx, yIdx);             
        items.push_back(item);
    }
}

const Block& Map::getBlock(int r, int c) const {
    return blocks[r][c];
}

const vector<Dot> Map::getDots() const {
    return dots;
}
vector<Item> Map::getItems() const {
    return items;
}
void Map::eraseDot(int idx) {
    dots.erase(dots.begin() + idx);
}
void Map::eraseItem(int idx) {
    items.erase(items.begin() + idx);
}
void Map::draw() const {
    for (int r = 0; r < NUM_ROW; ++r) {
        for (int c = 0; c < NUM_COL; ++c) {
            float x = LEFT_BOUNDARY + c * BLOCK_SIZE + BLOCK_SIZE / 2;
            float y = TOP_BOUNDARY - r * BLOCK_SIZE - BLOCK_SIZE / 2;

            glPushMatrix();
            glTranslatef(x, y, 0.0f);

            blocks[r][c].draw();

            glPopMatrix();
        }
    }
    //Dots draw
    for (int i = 0; i < dots.size(); i++) {
        dots[i].draw();
    }
    for (int i = 0; i < items.size(); i++) {
        items[i].draw();
    }
}
