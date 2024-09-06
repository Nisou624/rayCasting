#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <stdlib.h>
#include "misc.h"

extern const int mapSize;

extern const int map[10][10];


void drawGrid(Vector2 cellSize, int screenWidth, int screenHeight);

bool borderhit(Vector2 point, Vector2 *border, int screenWidth, int screenHeight);

bool hittingWall(Vector2 cellSize, Vector2 p1, Vector2 p2, Vector2 *hitPoint);

#endif