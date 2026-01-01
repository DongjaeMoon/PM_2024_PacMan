#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <GL/freeglut.h>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>
#include <algorithm>
#include <mmsystem.h>
#include <Windows.h>
#include <FreeImage.h>
#include "Constants.h"
#include "Light.h"
#include "Sphere.h"
#include "Map.h"
#include "CollisionHandler.h"

using namespace std;

const int FPS = 60;
int sTime = 0;
int eTime = 0;
int bTime = 0; int pTime = 0; int iTime = 0; int cTime = 0;
int sb0 = 0; int sp0 = 0; int si0 = 0; int sc0 = 0;
int sb = 0; int sp = 0; int si = 0; int sc = 0;
bool bPaused = false;

enum GAME_STATE { HOME, GUIDE, PLAY, CLEAR, FAIL, BOARD };
GAME_STATE gamestate = GAME_STATE::HOME;
vector<int> score; vector<string>player;
Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

PacMan pacman(BLOCK_SIZE / 2.5f, 20, 20, 3, false);
Ghost blinky(BLOCK_SIZE / 2.5f, 20, 20, 1.5, Ghost::CHASE);
Ghost pinky(BLOCK_SIZE / 2.5f, 20, 20, 1.5, Ghost::CHASE);
Ghost inky(BLOCK_SIZE / 2.5f, 20, 20, 1.5, Ghost::CHASE);
Ghost clyde(BLOCK_SIZE / 2.5f, 20, 20, 1.5, Ghost::CHASE);
Map map;
CollisionHandler colHandler;

static GLuint textureIDs[11];
GLubyte* textureDatas[11];
int imageWidths[11], imageHeights[11];

FIBITMAP* createBitMap(char const* filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);   
    FIBITMAP* bitmap = FreeImage_Load(format, filename);
    int bitsPerPixel = FreeImage_GetBPP(bitmap);
    FIBITMAP* bitmap32;
    if (bitsPerPixel == 32) bitmap32 = bitmap;
    else bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    return bitmap32;
}

void initialize() {
    // Light
    light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

    // PacMan
    Material mtl;    // basic material
    mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
    mtl.setAmbient(0.6f, 0.6f, 0.0f, 1.0f);
    mtl.setDiffuse(0.8f, 0.8f, 0.0f, 1.0f);
    mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    mtl.setShininess(30.0f);

    pacman.setIndexPosition(PACMAN_X, PACMAN_Y);
    pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pacman.setMTL(mtl);
    pacman.setLife(3);
    pacman.setCurrentDirection(Sphere::NONE);
    pacman.setNextDirection(Sphere::NONE);
    pacman.setAngle(0);

    FIBITMAP* bitmap0_32 = createBitMap("Pacman.PNG");
    imageWidths[0] = FreeImage_GetWidth(bitmap0_32);
    imageHeights[0] = FreeImage_GetHeight(bitmap0_32);
    textureDatas[0] = FreeImage_GetBits(bitmap0_32);
    glGenTextures(1, &textureIDs[0]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[0], imageHeights[0], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[0]);

    // Ghost: blinky
    mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
    mtl.setAmbient(0.6f, 0.0f, 0.0f, 1.0f);
    mtl.setDiffuse(0.8f, 0.0f, 0.0f, 1.0f);
    mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    mtl.setShininess(30.0f);

    blinky.setIndexPosition(BLINKY_X, BLINKY_Y);
    blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    blinky.setMTL(mtl);
    blinky.setCurrentDirection(Sphere::NONE);
    blinky.setNextDirection(Sphere::NONE);
    blinky.setState(Ghost::CHASE);
    bTime = 0; blinky.setScale(1.5);
    
    FIBITMAP* bitmap1_32 = createBitMap("Blinky.PNG");
    imageWidths[1] = FreeImage_GetWidth(bitmap1_32);
    imageHeights[1] = FreeImage_GetHeight(bitmap1_32);
    textureDatas[1] = FreeImage_GetBits(bitmap1_32);
    glGenTextures(1, &textureIDs[1]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[1], imageHeights[1], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[1]);

    // Ghost: pinky
    mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
    mtl.setAmbient(1.0f, 0.5f, 0.8f, 1.0f);
    mtl.setDiffuse(1.0f, 0.5f, 0.8f, 1.0f);
    mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    mtl.setShininess(30.0f);

    pinky.setIndexPosition(PINKY_X, PINKY_Y);
    pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pinky.setMTL(mtl);
    pinky.setCurrentDirection(Sphere::NONE);
    pinky.setNextDirection(Sphere::NONE);
    pinky.setState(Ghost::CHASE);
    pTime = 0; pinky.setScale(1.5);

    FIBITMAP* bitmap2_32 = createBitMap("Pinky.PNG");
    imageWidths[2] = FreeImage_GetWidth(bitmap2_32);
    imageHeights[2] = FreeImage_GetHeight(bitmap2_32);
    textureDatas[2] = FreeImage_GetBits(bitmap2_32);
    glGenTextures(1, &textureIDs[2]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[2], imageHeights[2], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[2]);

    // Ghost: inky
    mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
    mtl.setAmbient(0.5f, 1.0f, 1.0f, 1.0f);
    mtl.setDiffuse(0.5f, 1.0f, 1.0f, 1.0f);
    mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    mtl.setShininess(30.0f);

    inky.setIndexPosition(INKY_X, INKY_Y);
    inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    inky.setMTL(mtl);
    inky.setCurrentDirection(Sphere::NONE);
    inky.setNextDirection(Sphere::NONE);
    inky.setState(Ghost::CHASE);
    iTime = 0; inky.setScale(1.5);

    FIBITMAP* bitmap3_32 = createBitMap("Inky.PNG");
    imageWidths[3] = FreeImage_GetWidth(bitmap3_32);
    imageHeights[3] = FreeImage_GetHeight(bitmap3_32);
    textureDatas[3] = FreeImage_GetBits(bitmap3_32);
    glGenTextures(1, &textureIDs[3]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[3], imageHeights[3], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[3]);

    // Ghost: clyde
    mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
    mtl.setAmbient(1.0f, 0.6f, 0.0f, 0.5f);
    mtl.setDiffuse(0.8f, 0.8f, 0.0f, 0.5f);
    mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    mtl.setShininess(30.0f);

    clyde.setIndexPosition(CLYDE_X, CLYDE_Y);
    clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    clyde.setMTL(mtl);
    clyde.setCurrentDirection(Sphere::NONE);
    clyde.setNextDirection(Sphere::NONE);
    clyde.setState(Ghost::CHASE);
    cTime = 0; clyde.setScale(1.5);

    FIBITMAP* bitmap4_32 = createBitMap("Clyde.PNG");
    imageWidths[4] = FreeImage_GetWidth(bitmap4_32);
    imageHeights[4] = FreeImage_GetHeight(bitmap4_32);
    textureDatas[4] = FreeImage_GetBits(bitmap4_32);
    glGenTextures(1, &textureIDs[4]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[4], imageHeights[4], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[4]);

    FIBITMAP* bitmap5_32 = createBitMap("Frightened.PNG");
    imageWidths[5] = FreeImage_GetWidth(bitmap5_32);
    imageHeights[5] = FreeImage_GetHeight(bitmap5_32);
    textureDatas[5] = FreeImage_GetBits(bitmap5_32);
    glGenTextures(1, &textureIDs[5]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[5], imageHeights[5], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[5]);
    
    FIBITMAP* bitmap6_32 = createBitMap("Eaten.PNG");
    imageWidths[6] = FreeImage_GetWidth(bitmap6_32);
    imageHeights[6] = FreeImage_GetHeight(bitmap6_32);
    textureDatas[6] = FreeImage_GetBits(bitmap6_32);
    glGenTextures(1, &textureIDs[6]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[6], imageHeights[6], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[6]);

    FIBITMAP* bitmap7_32 = createBitMap("Home.PNG");
    imageWidths[7] = FreeImage_GetWidth(bitmap7_32);
    imageHeights[7] = FreeImage_GetHeight(bitmap7_32);
    textureDatas[7] = FreeImage_GetBits(bitmap7_32);
    glGenTextures(1, &textureIDs[7]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[7], imageHeights[7], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[7]);

    FIBITMAP* bitmap8_32 = createBitMap("Guide.PNG");
    imageWidths[8] = FreeImage_GetWidth(bitmap8_32);
    imageHeights[8] = FreeImage_GetHeight(bitmap8_32);
    textureDatas[8] = FreeImage_GetBits(bitmap8_32);
    glGenTextures(1, &textureIDs[8]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[8]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[8], imageHeights[8], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[8]);

    FIBITMAP* bitmap9_32 = createBitMap("Clear.PNG");
    imageWidths[9] = FreeImage_GetWidth(bitmap9_32);
    imageHeights[9] = FreeImage_GetHeight(bitmap9_32);
    textureDatas[9] = FreeImage_GetBits(bitmap9_32);
    glGenTextures(1, &textureIDs[9]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[9]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[9], imageHeights[9], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[9]);

    FIBITMAP* bitmap10_32 = createBitMap("Fail.PNG");
    imageWidths[10] = FreeImage_GetWidth(bitmap10_32);
    imageHeights[10] = FreeImage_GetHeight(bitmap10_32);
    textureDatas[10] = FreeImage_GetBits(bitmap10_32);
    glGenTextures(1, &textureIDs[10]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[10]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[10], imageHeights[10], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[10]);
    // Map
    map.createMap();
}

void updateDirectionOfPacMan() {
    int xIdx = pacman.getXIndex();
    int yIdx = pacman.getYIndex();

    const Block& lBlock = map.getBlock(yIdx, xIdx - 1);    // left
    const Block& rBlock = map.getBlock(yIdx, xIdx + 1);    // right
    const Block& tBlock = map.getBlock(yIdx - 1, xIdx);    // top
    const Block& bBlock = map.getBlock(yIdx + 1, xIdx);    // bottom
    Sphere::DIRECTION nextDir = pacman.getNextDirection();
    switch (nextDir)
    {
    case Sphere::NONE:
        break;
    case Sphere::LEFT:
        if (lBlock.isPassable()) {pacman.setAngle(180); pacman.updateDirection();}                   
        break;
    case Sphere::UP:
        if (tBlock.isPassable()) {pacman.setAngle(90); pacman.updateDirection();}
        break;
    case Sphere::RIGHT:
        if (rBlock.isPassable()) { pacman.setAngle(0); pacman.updateDirection();}
        break;
    case Sphere::DOWN:
        if (bBlock.isPassable()) { pacman.setAngle(270); pacman.updateDirection(); }
        break;
    default:
        break;
    }
}
void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY) {
    /* Implement */
    int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };
    int lIdx[2] = { idx[0] - 1 , idx[1] };// left
    int tIdx[2] = { idx[0], idx[1] - 1 };// top
    int rIdx[2] = { idx[0] + 1, idx[1] };// right
    int bIdx[2] = { idx[0], idx[1] + 1 };// bottom
    const Block& lBlock = map.getBlock(lIdx[1], lIdx[0]);// left
    const Block& tBlock = map.getBlock(tIdx[1], tIdx[0]);// top
    const Block& rBlock = map.getBlock(rIdx[1], rIdx[0]);// right
    const Block& bBlock = map.getBlock(bIdx[1], bIdx[0]);// bottom
            
    Sphere::DIRECTION currDir = ghost.getCurrentDirection();
    Sphere::DIRECTION newDir = Sphere::DIRECTION::NONE;
    int minIdxDist = INT_MAX;

    if (lBlock.isPassable() && currDir != Ghost::RIGHT) {
        int dist = ((lIdx[0] - targetX) * (lIdx[0] - targetX) + (lIdx[1] - targetY) * (lIdx[1] - targetY));
        if (minIdxDist >= dist) {minIdxDist = dist;   newDir = Ghost::LEFT; }
    }
    // right
    if (rBlock.isPassable() && currDir != Ghost::LEFT) {
        int dist = ((rIdx[0] - targetX) * (rIdx[0] - targetX) + (rIdx[1] - targetY) * (rIdx[1] - targetY));
        if (minIdxDist >= dist) {minIdxDist = dist;  newDir = Ghost::RIGHT;}        
    }
    // top
    if (tBlock.isPassable() && currDir != Ghost::DOWN) {
        int dist = ((tIdx[0] - targetX) * (tIdx[0] - targetX) + (tIdx[1] - targetY) * (tIdx[1] - targetY));
        if (minIdxDist >= dist) {minIdxDist = dist;   newDir = Ghost::UP; }
    }
    // bottom
    if (bBlock.isPassable() && currDir != Ghost::UP) {
        int dist = ((bIdx[0] - targetX) * (bIdx[0] - targetX) + (bIdx[1] - targetY) * (bIdx[1] - targetY));
        if (minIdxDist >= dist) {minIdxDist = dist;   newDir = Ghost::DOWN; }
    }
    // if newDir = None
    if (newDir == Sphere::DIRECTION::NONE) {
        switch (currDir) {
        case Sphere::LEFT:
            newDir = Sphere::RIGHT;
            break;
        case Sphere::RIGHT:
            newDir = Sphere::LEFT;
            break;
        case Sphere::UP:
            newDir = Sphere::DOWN;
            break;
        case Sphere::DOWN:
            newDir = Sphere::UP;
            break;
        default:
            break;
        }
    }
    if (ghost.getXIndex() == NUM_COL / 2 && ghost.getYIndex() == NUM_ROW / 2 && currDir != Ghost::DOWN) newDir = Ghost::UP;
    if (ghost.getXIndex() == ROOM_X && ghost.getYIndex() == ROOM_Y - 1 && ghost.getState() != Ghost::EATEN_C && ghost.getState() != Ghost::EATEN_S) {
        if (currDir == Ghost::LEFT) newDir = Ghost::LEFT;
        else if (currDir == Ghost::RIGHT) newDir = Ghost::RIGHT;        
    }    
    ghost.setNextDirection(newDir);
    ghost.updateDirection();
}

void updatePacMan() {    
    bool bNoDir = pacman.getCurrentDirection() == Sphere::DIRECTION::NONE;
    bool bIdxPosUpdated = pacman.isIndexPositionUpdated();
    if (bNoDir || bIdxPosUpdated) {
        updateDirectionOfPacMan();
        colHandler(pacman, map);
    }
    pacman.move();
}

void updateGhost() {
    //blinky
    bool bNoDir1 = blinky.getCurrentDirection() == Sphere::DIRECTION::NONE;
    bool bIdxPosUpdated1 = blinky.isIndexPositionUpdated();
    if (bNoDir1 || bIdxPosUpdated1) {
        int targetX = 0;
        int targetY = 0;

        switch (blinky.getState()) {
        case Ghost::CHASE:
            targetX = pacman.getXIndex();
            targetY = pacman.getYIndex();
            break;
        case Ghost::SCATTER:
            targetX = 1;
            targetY = 1;
            break;
        case Ghost::FRIGHTENED_C: case Ghost::FRIGHTENED_S:
            targetX = (NUM_COL - 2) - pacman.getXIndex();
            targetY = (NUM_ROW - 2) - pacman.getYIndex();
            break;
        case Ghost::EATEN_C: case Ghost::EATEN_S:
            targetX = ROOM_X;
            targetY = ROOM_Y;
            break;
        }
        updateDirectionOfGhost(blinky, targetX, targetY);

        if ((blinky.getXIndex() == targetX) && (blinky.getYIndex() == targetY)) {
            if (blinky.getState() == Ghost::EATEN_C) { blinky.setState(Ghost::CHASE); bTime = 0; }
            else if (blinky.getState() == Ghost::EATEN_S) { blinky.setState(Ghost::SCATTER); bTime = 0; }
        }
    }
    blinky.move();

    //pinky
    bool bNoDir2 = pinky.getCurrentDirection() == Sphere::DIRECTION::NONE;
    bool bIdxPosUpdated2 = pinky.isIndexPositionUpdated();
    if (bNoDir2 || bIdxPosUpdated2) {
        int targetX = 0;
        int targetY = 0;

        switch (pinky.getState()) {
        case Ghost::CHASE:
            switch (pacman.getCurrentDirection()) {
            case Sphere::NONE:
                targetX = pacman.getXIndex();
                targetY = pacman.getYIndex();
                break;
            case Sphere::LEFT:
                targetX = pacman.getXIndex() - 4;
                targetY = pacman.getYIndex();
                break;
            case Sphere::RIGHT:
                targetX = pacman.getXIndex() + 4;
                targetY = pacman.getYIndex();
                break;
            case Sphere::UP:
                targetX = pacman.getXIndex();
                targetY = pacman.getYIndex() - 4;
                break;
            case Sphere::DOWN:
                targetX = pacman.getXIndex();
                targetY = pacman.getYIndex() + 4;
                break;
            }
            break;
        case Ghost::SCATTER:
            targetX = NUM_COL - 2;
            targetY = 1;
            break;
        case Ghost::FRIGHTENED_C: case Ghost::FRIGHTENED_S:
            targetX = (NUM_COL - 2) - pacman.getXIndex();
            targetY = (NUM_ROW - 2) - pacman.getYIndex();
            break;
        case Ghost::EATEN_C: case Ghost::EATEN_S:
            targetX = ROOM_X;
            targetY = ROOM_Y;
            break;
        }
        updateDirectionOfGhost(pinky, targetX, targetY);

        if ((pinky.getXIndex() == targetX) && (pinky.getYIndex() == targetY)) {
            if (pinky.getState() == Ghost::EATEN_C) { pinky.setState(Ghost::CHASE); pTime = 0; }
            else if (pinky.getState() == Ghost::EATEN_S) { pinky.setState(Ghost::SCATTER); pTime = 0; }
        }
    }
    pinky.move();

    //inky
    bool bNoDir3 = inky.getCurrentDirection() == Sphere::DIRECTION::NONE;
    bool bIdxPosUpdated3 = inky.isIndexPositionUpdated();
    if (bNoDir3 || bIdxPosUpdated3) {
        int targetX = 0;
        int targetY = 0;
        int bx = blinky.getXIndex();
        int by = blinky.getYIndex();
        switch (inky.getState()) {
        case Ghost::CHASE:
            switch (pacman.getCurrentDirection()) {
            case Sphere::NONE:
                targetX = 2 * pacman.getXIndex() - bx;
                targetY = 2 * pacman.getYIndex() - by;
                break;
            case Sphere::LEFT:
                targetX = 2 * pacman.getXIndex() - 4 - bx;
                targetY = 2 * pacman.getYIndex() - by;
                break;
            case Sphere::RIGHT:
                targetX = 2 * pacman.getXIndex() + 4 - bx;
                targetY = 2 * pacman.getYIndex() - by;
                break;
            case Sphere::UP:
                targetX = 2 * pacman.getXIndex() - bx;
                targetY = 2 * pacman.getYIndex() - 4 - by;
                break;
            case Sphere::DOWN:
                targetX = 2 * pacman.getXIndex() - bx;
                targetY = 2 * pacman.getYIndex() + 4 - by;
                break;
            }
            break;
        case Ghost::SCATTER:
            targetX = 1;
            targetY = NUM_ROW - 2;
            break;
        case Ghost::FRIGHTENED_C: case Ghost::FRIGHTENED_S:
            targetX = (NUM_COL - 2) - pacman.getXIndex();
            targetY = (NUM_ROW - 2) - pacman.getYIndex();
            break;
        case Ghost::EATEN_C: case Ghost::EATEN_S:
            targetX = ROOM_X;
            targetY = ROOM_Y;
            break;
        }
        updateDirectionOfGhost(inky, targetX, targetY);
        if ((inky.getXIndex() == targetX) && (inky.getYIndex() == targetY)) {
            if (inky.getState() == Ghost::EATEN_C) { inky.setState(Ghost::CHASE); iTime = 0; }
            else if (inky.getState() == Ghost::EATEN_S) { inky.setState(Ghost::SCATTER); iTime = 0; }
        }
    }
    inky.move();

    //clyde
    bool bNoDir4 = clyde.getCurrentDirection() == Sphere::DIRECTION::NONE;
    bool bIdxPosUpdated4 = clyde.isIndexPositionUpdated();
    if (bNoDir4 || bIdxPosUpdated4) {
        int targetX = 0;
        int targetY = 0;
        Vector3f c = clyde.getCenter() - pacman.getCenter();
        switch (clyde.getState()) {
        case Ghost::CHASE:
            if (dotProduct(c, c) < BLOCK_SIZE * BLOCK_SIZE * 64) {
                targetX = NUM_COL - 2;
                targetY = NUM_ROW - 2;
            }
            else {
                targetX = pacman.getXIndex();
                targetY = pacman.getYIndex();
            }
            break;
        case Ghost::SCATTER:
            targetX = NUM_COL - 2;
            targetY = NUM_ROW - 2;
            break;
        case Ghost::FRIGHTENED_C: case Ghost::FRIGHTENED_S:
            targetX = (NUM_COL - 2) - pacman.getXIndex();
            targetY = (NUM_ROW - 2) - pacman.getYIndex();
            break;
        case Ghost::EATEN_C: case Ghost::EATEN_S:
            targetX = ROOM_X;
            targetY = ROOM_Y;
            break;
        }
        updateDirectionOfGhost(clyde, targetX, targetY);

        if ((clyde.getXIndex() == targetX) && (clyde.getYIndex() == targetY)) {
            if (clyde.getState() == Ghost::EATEN_C) { clyde.setState(Ghost::CHASE); cTime = 0; }
            else if (clyde.getState() == Ghost::EATEN_S) { clyde.setState(Ghost::SCATTER); cTime = 0; }
        }
    }
    clyde.move();
}
void updateitem() {
    if (pacman.getXIndex()==NUM_COL/2 && pacman.getYIndex()==NUM_ROW/2)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 5; ++i) {
        float angle = 2.0f * M_PI * i / 100;
        float x = cos(angle);
        float y = sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
void idle() {
    if (bPaused || gamestate != PLAY) return;
    float spf = 1000.0f / FPS;
    eTime = glutGet(GLUT_ELAPSED_TIME);

    if (eTime - sTime > spf) {
        updatePacMan();
        updateGhost();
        colHandler(pacman, blinky);
        colHandler(pacman, pinky);
        colHandler(pacman, inky);
        colHandler(pacman, clyde);

        if (gamestate == PLAY && map.getDots().empty()) {
            gamestate = GAME_STATE::CLEAR;
            PlaySound(TEXT("Clear.wav"), NULL, SND_ASYNC);            
            score.back() = score.back() * pacman.getLife();
            
            Sleep(1000);
            glutPostRedisplay();
            return;
        }
        if (pacman.isCollided()) {
            pacman.setLife(pacman.getLife() - 1);            
            pacman.setIndexPosition(PACMAN_X, PACMAN_Y);
            pacman.setCurrentDirection(Sphere::NONE); pacman.setAngle(0);
            blinky.setIndexPosition(BLINKY_X, BLINKY_Y);           blinky.setCurrentDirection(Sphere::NONE);   blinky.setState(Ghost::CHASE);
            pinky.setIndexPosition(PINKY_X, PINKY_Y);  pinky.setCurrentDirection(Sphere::NONE);    pinky.setState(Ghost::CHASE);
            inky.setIndexPosition(INKY_X, INKY_Y);   inky.setCurrentDirection(Sphere::NONE);     inky.setState(Ghost::CHASE);
            clyde.setIndexPosition(CLYDE_X, CLYDE_Y);  clyde.setCurrentDirection(Sphere::NONE);  clyde.setState(Ghost::CHASE);
            pacman.setCollided(false);
        }             

        if (pacman.getLife() == 0) { gamestate = GAME_STATE::FAIL; PlaySound(TEXT("Fail.wav"), NULL, SND_ASYNC); }

        bTime++; pTime++; iTime++; cTime++;
        int sb = bTime / FPS; int sp = pTime / FPS; int si = iTime / FPS; int sc = cTime / FPS;
        if (sb != sb0) { /*cout << "b: " << sb << "  ";*/    sb0 = sb; }     if (sp != sp0) { /*cout << "p: " << sp << "  ";*/    sp0 = sp; }
        if (si != si0) { /*cout << "i: " << si << "  "; */ si0 = si; }   if (sc != sc0) { /*cout << "c: " << sc << "  " << endl; */ sc0 = sc; }

        if (bTime > FPS * PERIOD) {
            switch (blinky.getState()) {
            case Ghost::STATE::CHASE: case Ghost::STATE::FRIGHTENED_S:
                blinky.setState(Ghost::STATE::SCATTER);
                break;
            case Ghost::STATE::SCATTER: case Ghost::STATE::FRIGHTENED_C:
                blinky.setState(Ghost::STATE::CHASE);
                break;
            }
            bTime = 0;
        }
        if (pTime > FPS * PERIOD) {
            switch (pinky.getState()) {
            case Ghost::STATE::CHASE: case Ghost::STATE::FRIGHTENED_S:
                pinky.setState(Ghost::STATE::SCATTER);
                break;
            case Ghost::STATE::SCATTER: case Ghost::STATE::FRIGHTENED_C:
                pinky.setState(Ghost::STATE::CHASE);
                break;
            }
            pTime = 0;
        }
        if (iTime > FPS * PERIOD) {
            switch (inky.getState()) {
            case Ghost::STATE::CHASE: case Ghost::STATE::FRIGHTENED_S:
                inky.setState(Ghost::STATE::SCATTER);
                break;
            case Ghost::STATE::SCATTER: case Ghost::STATE::FRIGHTENED_C:
                inky.setState(Ghost::STATE::CHASE);
                break;
            }
            iTime = 0;
        }
        if (cTime > FPS * PERIOD) {
            switch (clyde.getState()) {
            case Ghost::STATE::CHASE: case Ghost::STATE::FRIGHTENED_S:
                clyde.setState(Ghost::STATE::SCATTER);
                break;
            case Ghost::STATE::SCATTER: case Ghost::STATE::FRIGHTENED_C:
                clyde.setState(Ghost::STATE::CHASE);
                break;
            }
            cTime = 0;
        }
        sTime = eTime;
        glutPostRedisplay();
    }
}
void displayCharacters(void* font, string str, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (int i = 0; i < str.size(); i++)
        glutBitmapCharacter(font, str[i]);
}
void homedisplay() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureIDs[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void guidedisplay() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureIDs[8]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void faildisplay() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureIDs[10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void cleardisplay() {        
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureIDs[9]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(WINDOW_W * 0.45f, WINDOW_H * 0.45f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(WINDOW_W * 0.45f, -WINDOW_H * 0.45f);
    glEnd();
    glDisable(GL_TEXTURE_2D);   
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, to_string(score.back()), -20.0f, -45.0f);
}
void boarddisplay() {    
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "SCOREBOARD", -80.0f, 240.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "h : Home", -250.0f, -250.0f);
    if (score.size() == 0) displayCharacters(GLUT_BITMAP_HELVETICA_18, "Nobody cleared yet", -80.0f, 0.0f);
    else {
        glLineWidth(3);
        glBegin(GL_LINES); glColor3f(1, 1, 1); glVertex2f(-240, 150); glVertex2f(240, 150); glEnd();
        glBegin(GL_LINES); glColor3f(1, 1, 1); glVertex2f(-80, 200); glVertex2f(-80, -225); glEnd();
        glBegin(GL_LINES); glColor3f(1, 1, 1); glVertex2f(80, 200);  glVertex2f(80, -225);  glEnd();
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Player", -180.0f, 180.0f);
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Score", -20.0f, 180.0f);
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Rank", 120.0f, 180.0f);
        vector<int> rank;
        for (int i = 0; i < score.size(); i++) {
            rank.push_back(1);
            for (int j = 0; j < score.size(); j++) {
                if (score[i] < score[j]) rank[i]++;
            }
        }
        for (int i = 0; i < score.size(); i++) {
            displayCharacters(GLUT_BITMAP_HELVETICA_18, to_string(i+1), -160.0f, 120 - 20 * i);
            //displayCharacters(GLUT_BITMAP_HELVETICA_18, player[i], -160.0f, 120 - 20 * i);
            displayCharacters(GLUT_BITMAP_HELVETICA_18, to_string(score[i]), -25.0f, 120 - 20 * i);
            displayCharacters(GLUT_BITMAP_HELVETICA_18, to_string(rank[i]), 130.0f, 120 - 20 * i);
        }
    }
}
void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
    //gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw 2D
    if (gamestate == PLAY) {
        map.draw();
        
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //Draw Pacman
        glPushMatrix();
        glTranslatef(pacman.getCenter()[0], pacman.getCenter()[1], pacman.getCenter()[2]);
        glRotatef((GLfloat)pacman.getAngle(), 0, 0, 1);
        glTranslatef(-pacman.getCenter()[0], -pacman.getCenter()[1], -pacman.getCenter()[2]);
        glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(pacman.getCenter()[0] - pacman.getRadius(), pacman.getCenter()[1] - pacman.getRadius());
        glTexCoord2f(0.0f, 1.0f); glVertex2f(pacman.getCenter()[0] - pacman.getRadius(), pacman.getCenter()[1] + pacman.getRadius());
        glTexCoord2f(1.0f, 1.0f); glVertex2f(pacman.getCenter()[0] + pacman.getRadius(), pacman.getCenter()[1] + pacman.getRadius());
        glTexCoord2f(1.0f, 0.0f); glVertex2f(pacman.getCenter()[0] + pacman.getRadius(), pacman.getCenter()[1] - pacman.getRadius());
        glEnd();
        glPopMatrix();
        //Draw blinky
        glPushMatrix();
        if (blinky.getState()==Ghost::CHASE || blinky.getState() == Ghost::SCATTER) glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
        else if (blinky.getState() == Ghost::FRIGHTENED_C || blinky.getState() == Ghost::FRIGHTENED_S) glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
        else if (blinky.getState() == Ghost::EATEN_C || blinky.getState() == Ghost::EATEN_S) glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(blinky.getCenter()[0] - blinky.getRadius(), blinky.getCenter()[1] - blinky.getRadius());
        glTexCoord2f(0.0f, 1.0f); glVertex2f(blinky.getCenter()[0] - blinky.getRadius(), blinky.getCenter()[1] + blinky.getRadius());
        glTexCoord2f(1.0f, 1.0f); glVertex2f(blinky.getCenter()[0] + blinky.getRadius(), blinky.getCenter()[1] + blinky.getRadius());
        glTexCoord2f(1.0f, 0.0f); glVertex2f(blinky.getCenter()[0] + blinky.getRadius(), blinky.getCenter()[1] - blinky.getRadius());
        glEnd();
        //Draw pinky
        if (pinky.getState() == Ghost::CHASE || pinky.getState() == Ghost::SCATTER) glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
        else if (pinky.getState() == Ghost::FRIGHTENED_C || pinky.getState() == Ghost::FRIGHTENED_S) glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
        else if (pinky.getState() == Ghost::EATEN_C || pinky.getState() == Ghost::EATEN_S) glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(pinky.getCenter()[0] - pinky.getRadius(), pinky.getCenter()[1] - pinky.getRadius());
        glTexCoord2f(0.0f, 1.0f); glVertex2f(pinky.getCenter()[0] - pinky.getRadius(), pinky.getCenter()[1] + pinky.getRadius());
        glTexCoord2f(1.0f, 1.0f); glVertex2f(pinky.getCenter()[0] + pinky.getRadius(), pinky.getCenter()[1] + pinky.getRadius());
        glTexCoord2f(1.0f, 0.0f); glVertex2f(pinky.getCenter()[0] + pinky.getRadius(), pinky.getCenter()[1] - pinky.getRadius());
        glEnd();
        //Draw inky
        if (inky.getState() == Ghost::CHASE || inky.getState() == Ghost::SCATTER) glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
        else if (inky.getState()==Ghost::FRIGHTENED_C||inky.getState()==Ghost::FRIGHTENED_S) glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
        else if (inky.getState() == Ghost::EATEN_C || inky.getState() == Ghost::EATEN_S) glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(inky.getCenter()[0] - inky.getRadius(), inky.getCenter()[1] - inky.getRadius());
        glTexCoord2f(0.0f, 1.0f); glVertex2f(inky.getCenter()[0] - inky.getRadius(), inky.getCenter()[1] + inky.getRadius());
        glTexCoord2f(1.0f, 1.0f); glVertex2f(inky.getCenter()[0] + inky.getRadius(), inky.getCenter()[1] + inky.getRadius());
        glTexCoord2f(1.0f, 0.0f); glVertex2f(inky.getCenter()[0] + inky.getRadius(), inky.getCenter()[1] - inky.getRadius());
        glEnd();
        //Draw clyde
        if (clyde.getState() == Ghost::CHASE || clyde.getState() == Ghost::SCATTER)glBindTexture(GL_TEXTURE_2D, textureIDs[4]);
        else if (clyde.getState() == Ghost::FRIGHTENED_C || clyde.getState() == Ghost::FRIGHTENED_S) glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
        else if (clyde.getState() == Ghost::EATEN_C || clyde.getState() == Ghost::EATEN_S) glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(clyde.getCenter()[0] - clyde.getRadius(), clyde.getCenter()[1] - clyde.getRadius());
        glTexCoord2f(0.0f, 1.0f); glVertex2f(clyde.getCenter()[0] - clyde.getRadius(), clyde.getCenter()[1] + clyde.getRadius());
        glTexCoord2f(1.0f, 1.0f); glVertex2f(clyde.getCenter()[0] + clyde.getRadius(), clyde.getCenter()[1] + clyde.getRadius());
        glTexCoord2f(1.0f, 0.0f); glVertex2f(clyde.getCenter()[0] + clyde.getRadius(), clyde.getCenter()[1] - clyde.getRadius());
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        
        switch (pacman.getLife()) {
        case 3:
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.3f, BOUNDARY_Y * 0.87f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.3f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.87f);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.87f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.5f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.5f, BOUNDARY_Y * 0.87f);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.5f, BOUNDARY_Y * 0.87f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.5f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.6f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.6f, BOUNDARY_Y * 0.87f);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            break;
        case 2:
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.3f, BOUNDARY_Y * 0.87f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.3f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.87f);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.87f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.5f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.5f, BOUNDARY_Y * 0.87f);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            break;
        case 1:
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.3f, BOUNDARY_Y * 0.87f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.3f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.97f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(BOUNDARY_X * 0.4f, BOUNDARY_Y * 0.87f);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            break;
        }

        string str = to_string(score.back());
        displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "Current score:", -BOUNDARY_X * 0.6f, BOUNDARY_Y * 0.9f);
        displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, str, -BOUNDARY_X * 0.15f, BOUNDARY_Y * 0.9f);

        // Draw 3D
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(light.getID());
        light.draw();
        //pacman.draw(); blinky.draw(); pinky.draw(); inky.draw(); clyde.draw();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(light.getID());
    }
    else if (gamestate == HOME) homedisplay();
    else if (gamestate == FAIL) faildisplay();
    else if (gamestate == GUIDE) guidedisplay();
    else if (gamestate == BOARD) boarddisplay();
    else if (gamestate == CLEAR) cleardisplay();  

    glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (gamestate == PLAY) {
        if (key == 32) bPaused = !bPaused;        
    }
    else if (gamestate == HOME) {        
        if (key == 13) {            
            gamestate = PLAY; initialize(); score.push_back(0);
            PlaySound(TEXT("Play.wav"), NULL, SND_ASYNC | SND_LOOP);
        }
        else if (key == 'g' || key == 'G') gamestate = GUIDE;
        else if (key == 's' || key == 'S') gamestate = BOARD;
    }
    else if (gamestate == FAIL) {        
        if (key == 'h' || key == 'H') {
            gamestate = HOME; initialize(); score.pop_back();
            PlaySound(TEXT("Intro.wav"), NULL, SND_ASYNC | SND_LOOP);
        }
    }
    else if (gamestate == CLEAR) {               
        if (key == 'h' || key == 'H') {
            gamestate = HOME; initialize();
            PlaySound(TEXT("Intro.wav"), NULL, SND_ASYNC | SND_LOOP);
        }
        else if (key == 's' || key == 'S') gamestate = BOARD;
    }
    else if (gamestate == BOARD) {
        if (key == 'h' || key == 'H') { gamestate = HOME; PlaySound(TEXT("Intro.wav"), NULL, SND_ASYNC | SND_LOOP); }
    }
    else if (gamestate == GUIDE) {        
        if (key == 13) {
            gamestate = PLAY; score.push_back(0);  initialize();
        PlaySound(TEXT("Play.wav"), NULL, SND_ASYNC | SND_LOOP);
        }
        else if (key == 'h' || key == 'H') gamestate = HOME;
    }
    glutPostRedisplay();
}

void specialKeyDown(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        pacman.setNextDirection(Sphere::DIRECTION::LEFT); 
        break;
    case GLUT_KEY_UP:
        pacman.setNextDirection(Sphere::DIRECTION::UP);
        break;
    case GLUT_KEY_RIGHT:
        pacman.setNextDirection(Sphere::DIRECTION::RIGHT);
        break;
    case GLUT_KEY_DOWN:
        pacman.setNextDirection(Sphere::DIRECTION::DOWN);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Pac-Man");

    initialize();

    // register callbacks
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardDown);
    glutSpecialFunc(specialKeyDown);    
    
    //PlaySound(TEXT("Intro.wav"), NULL, SND_ASYNC | SND_LOOP );
    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}