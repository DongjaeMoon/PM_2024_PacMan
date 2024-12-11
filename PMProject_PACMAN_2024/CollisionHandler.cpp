#pragma comment(lib, "winmm.lib")
#include <GL/glut.h>
#include "CollisionHandler.h"
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>

using namespace std;
extern int bTime; extern int pTime; extern int iTime; extern int cTime;
extern vector<int> score;
extern Ghost blinky, pinky, inky, clyde;
extern int bTime, pTime, iTime, cTime;
void CollisionHandler::operator()(PacMan& pacman, Map& map) {
    CollisionDetector detector;
    if (detector(pacman, map)) {
        pacman.setCurrentDirection(Sphere::NONE);
    }
    else {
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

        const auto& dots = map.getDots();
        for (int i = 0; i < dots.size(); ++i) {
            Dot dot = dots[i];
            if (dot.getXIdx() == next_xIdx || dot.getYIdx() == next_yIdx) {
                if (detector(pacman, dot)) {
                    if (dot.getRadius() == LARGE_DOT_RADIUS) {                                              
                        
                        extern Ghost blinky, pinky, inky, clyde;
                        extern int bTime, pTime, iTime, cTime;
                        score.back() += 900;                        
                        if (blinky.getState() == Ghost::STATE::CHASE) {blinky.setState(Ghost::STATE::FRIGHTENED_C);  bTime = 0;}
                        else if (blinky.getState()==Ghost::STATE::SCATTER){ blinky.setState(Ghost::STATE::FRIGHTENED_S);  bTime = 0; }

                        if (pinky.getState() == Ghost::STATE::CHASE) { pinky.setState(Ghost::STATE::FRIGHTENED_C);  pTime = 0; }
                        else if (pinky.getState() == Ghost::STATE::SCATTER) { pinky.setState(Ghost::STATE::FRIGHTENED_S);  pTime = 0; }

                        if (inky.getState() == Ghost::STATE::CHASE) { inky.setState(Ghost::STATE::FRIGHTENED_C);  iTime = 0; }
                        else if (inky.getState() == Ghost::STATE::SCATTER) { inky.setState(Ghost::STATE::FRIGHTENED_S);  iTime = 0; }

                        if (clyde.getState() == Ghost::STATE::CHASE) { clyde.setState(Ghost::STATE::FRIGHTENED_C);  cTime = 0; }
                        else if (clyde.getState() == Ghost::STATE::SCATTER) { clyde.setState(Ghost::STATE::FRIGHTENED_S);  cTime = 0; }                                       
                    }
                    map.eraseDot(i);                    
                    score.back() += 100;
                    break;
                }
            }
        }
        const auto& items = map.getItems();
        if (!items.empty()) {
            for (int i = 0; i < items.size(); ++i) {
                Item item = items[i];
                if (item.getXIdx() == next_xIdx || item.getYIdx() == next_yIdx) {
                    if (detector(pacman, item) && pacman.getLife() == 1) {
                        blinky.setScale(blinky.getScale() * 0.7);
                        pinky.setScale(pinky.getScale() * 0.7);
                        inky.setScale(inky.getScale() * 0.7);
                        clyde.setScale(clyde.getScale() * 0.7);
                        map.eraseItem(i);
                        break;
                    }
                }
            }
        }
    }
}

void CollisionHandler::operator()(PacMan& pacman, Ghost& ghost) {
    CollisionDetector detector;
    if (detector(pacman, ghost)) {
        switch (ghost.getState()) {
        case Ghost::CHASE: case Ghost::SCATTER:
            //PlaySound(TEXT("Delete.wav"), NULL, SND_ASYNC);
            pacman.setCollided(true);
            Sleep(1000);
            bTime = 0; pTime = 0; iTime = 0; cTime = 0;
            break;
        case Ghost::FRIGHTENED_C:
            ghost.setState(Ghost::STATE::EATEN_C);
            //PlaySound(TEXT("Launch.wav"), NULL, SND_ASYNC);
            score.back() += 500;
            break;
        case Ghost::FRIGHTENED_S:
            ghost.setState(Ghost::STATE::EATEN_S);
            //PlaySound(TEXT("Launch.wav"), NULL, SND_ASYNC);
            score.back() += 500;
            break;
        default:
            break;
        }
    }
}
