#ifndef MISC_H
#define MISC_H

#include <raylib.h>
#include <math.h>
#include "map.h"

typedef struct {
    Vector2 point;
    float direction;
} player;

float distanceVec(Vector2 p1, Vector2 p2);

Vector2 distance(Vector2 p1, Vector2 p2);

float snap(float x, float dx, float cellSize);

Vector2 drawNextPoint(Vector2 p1, Vector2 p2, Vector2 cellSize);

void drawRay(Vector2 cellSize, Vector2 p1, Vector2 p2, Vector2 *wallPoint, int mapWidth, int mapHeight);

Vector2 helperPointFromAngle(Vector2 point, float angle, int distance);

void rayFOV(Vector2 cellSize, player player, int mapWidth, int mapHeight, float FOV);

void drawFOV(player player, float FOV);

float toRad(float angle);


#endif