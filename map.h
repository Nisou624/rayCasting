#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <stdlib.h>
#include "misc.h"

extern const int mapSize;

extern Vector2 cellSize;

extern Vector2 miniMapSize;

extern Vector2 screenSize;

extern const int map[10][10];

#define COLORS_LEN 5

extern const Color colors[COLORS_LEN];


void drawGrid(Vector2 cellSize, int screenWidth, int screenHeight);

bool borderhit(Vector2 point, Vector2 *border, float angle);

bool hittingWall(Vector2 cellSize, Vector2 p1, Vector2 p2, Vector2 *hitPoint, Color *color);

#endif