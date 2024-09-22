#ifndef MISC_H
#define MISC_H

#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include "map.h"

typedef struct {
    Vector2 point;
    float direction;
} player;

Color darkenColor(Color color, float distance);

float distanceVec(Vector2 p1, Vector2 p2);

Vector2 distance(Vector2 p1, Vector2 p2);

float snap(float x, float dx, float cellSize);

Vector2 drawNextPoint(Vector2 p1, Vector2 p2, Vector2 cellSize);

void drawRay(Vector2 cellSize, Vector2 p1, Vector2 p2, float angle, Vector2 *wallPoint, Vector2 *borderPoint, Color *wallColor);

Vector2 helperPointFromAngle(Vector2 point, float angle, float distance);

void rayFOV(Vector2 cellSize, player player, float FOV, Texture2D texture);

void drawWall(player player, Vector2 wallPoint, Color wallColor, int iter, Vector2 screenSize, float FOV);

void drawFOV(player player, float FOV);

float toRad(float angle);

Vector2 addVec(Vector2 p1, Vector2 p2);


#endif