#pragma once

/* OpenGL window constants */
#define WINDOW_X 300
#define WINDOW_Y 300

#define WINDOW_W 640        // window's width
#define WINDOW_H 640        // window's height

#define BOUNDARY_X (WINDOW_W)/2
#define BOUNDARY_Y (WINDOW_H)/2

/* GAME constants */
#define NUM_ROW 19
#define NUM_COL 19

#define BLOCK_SIZE 30
#define MOVE_SPEED 1
#define PERIOD 10

// map boundary
#define LEFT_BOUNDARY (-(NUM_COL / 2) * BLOCK_SIZE)
#define RIGHT_BOUNDARY ((NUM_COL / 2) * BLOCK_SIZE)
#define TOP_BOUNDARY ((NUM_COL / 2) * BLOCK_SIZE)
#define BOTTOM_BOUNDARY (-(NUM_COL / 2) * BLOCK_SIZE)

// pi
#define M_PI 3.14159265358979323846

// dot radius
#define SMALL_DOT_RADIUS (BLOCK_SIZE / 10)
#define LARGE_DOT_RADIUS (BLOCK_SIZE / 5)
#define ITEM_RADIUS (BLOCK_SIZE / 6)

#define PACMAN_X 1
#define PACMAN_Y 1
#define BLINKY_X (NUM_COL / 2)
#define BLINKY_Y (NUM_ROW / 2)
#define PINKY_X (NUM_COL / 2 + 1)
#define PINKY_Y (NUM_ROW / 2)
#define INKY_X (NUM_COL / 2 - 1)
#define INKY_Y (NUM_ROW / 2)
#define CLYDE_X (NUM_COL / 2)
#define CLYDE_Y ((NUM_ROW / 2)-1)
#define ROOM_X (NUM_COL / 2)
#define ROOM_Y ((NUM_ROW / 2)-1)

