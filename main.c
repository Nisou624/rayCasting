#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <math.h>
#include "map.h"

Vector2 posInMap(Vector2 point, Vector2 screenSize, Vector2 mapSize)
{
    return (Vector2){mapSize.x * point.x / screenSize.x, mapSize.y * point.y / screenSize.y};
}

void drawMiniMap(player player, float FOV, Vector2 cellSize, Vector2 minimapSize){
    drawGrid(cellSize, minimapSize.x, minimapSize.y);

    DrawCircleV(player.point, 5, RED);

    drawFOV(player, FOV);
};


void drawPoint(Vector2 point, Vector2 minimapSize){
    Vector2 border;
    if (!borderhit(point, &border, minimapSize.x, minimapSize.y)){
        DrawCircleV(point, 5, RED);
    }else{
        DrawCircleV(border, 5, RED);
    }
}


int main(void)
{

    // init window
    const int screenPx = 80;
    const int screenWidth = screenPx * 16;
    const int screenHeight = screenPx * 9;

    Vector2 cellSize = {15, 15};
    //Vector2 cellSize = {screenWidth / mapSize, screenHeight / mapSize};
    const Vector2 minimapSize = {cellSize.x * mapSize, cellSize.y * mapSize};

    Vector2 pPoint = {578, 600};

    player p1MM = {posInMap(pPoint, (Vector2){screenWidth, screenHeight}, minimapSize), -100};
    player p1 = {pPoint, 0};

    // Vector2 point = {1054, 233};
    //  Vector2 point = {400, 600};
    //  Vector2 playerPos = point;

    InitWindow(screenWidth, screenHeight, "My Raylib Window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawGrid(cellSize, minimapSize.x, minimapSize.y);
        //drawGrid(cellSize, screenWidth, screenHeight);

        //DrawCircleV(p1.point, 5, RED);
        DrawCircleV(p1MM.point, 5, RED);

        float FOV = 30;

        //rayFOV(cellSize, p1, screenWidth, screenHeight, FOV);
        rayFOV(cellSize, p1MM, minimapSize.x, minimapSize.y, FOV);
        
        
        //drawFOV(p1, FOV);
        //drawFOV(p1MM, FOV);

            if(!borderhit(p1MM.point, NULL, minimapSize.x, minimapSize.y)) {
            if (IsKeyDown(KEY_W))
            {
                p1MM.point.y -= .1f;
            }
            else if (IsKeyDown(KEY_S))
            {
                p1MM.point.y += .1f;
            }
            else if (IsKeyDown(KEY_A))
            {
                p1MM.point.x -= .1f;
            }
            else if (IsKeyDown(KEY_D))
            {
                p1MM.point.x += .1f;
            }
            else if (IsKeyDown(KEY_LEFT))
            {
                p1MM.direction -= .2f;
            }
            else if (IsKeyDown(KEY_RIGHT))
            {
                p1MM.direction += .2f;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}