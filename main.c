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

void playerMouvement(player *player, Vector2 minimapSize, Vector2 cellSize, float FOV){
    Vector2 pG = helperPointFromAngle(player->point, toRad(player->direction) - toRad(FOV / 2), 10);
    Vector2 pD = helperPointFromAngle(player->point, toRad(player->direction) + toRad(FOV / 2), 10);

    Vector2 nextPoint = {0, 0};

    if (IsKeyDown(KEY_W))
    {
        nextPoint = helperPointFromAngle(player->point, toRad(player->direction), .1f);
        if(!borderhit(nextPoint, NULL, minimapSize.x, minimapSize.y) && !hittingWall(cellSize, player->point, nextPoint, NULL, NULL) &&
            !borderhit(pG, NULL, minimapSize.x, minimapSize.y) && !hittingWall(cellSize, player->point, pG, NULL, NULL) &&
            !borderhit(pD, NULL, minimapSize.x, minimapSize.y) && !hittingWall(cellSize, player->point, pD, NULL, NULL)) player->point = nextPoint;
    }
    else if (IsKeyDown(KEY_S))
    {
        nextPoint = helperPointFromAngle(player->point, toRad(player->direction), -0.1f);
        if(!borderhit(nextPoint, NULL, minimapSize.x, minimapSize.y) &&
            !hittingWall(cellSize, player->point, nextPoint, NULL, NULL)) player->point = nextPoint;
    }
    else if (IsKeyDown(KEY_A))
    {
        if(!borderhit(pG, NULL, minimapSize.x, minimapSize.y) && 
            !hittingWall(cellSize, player->point, pG, NULL, NULL)) player->point = helperPointFromAngle(player->point, toRad(player->direction - 90), .1f);
    }
    else if (IsKeyDown(KEY_D))
    {
        if(!borderhit(pD, NULL, minimapSize.x, minimapSize.y) &&
            !hittingWall(cellSize, player->point, pD, NULL, NULL)) player->point = helperPointFromAngle(player->point, toRad(player->direction + 90), .1f);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if(!borderhit(pG, NULL, minimapSize.x, minimapSize.y) &&
            !hittingWall(cellSize, player->point, pG, NULL, NULL)) player->direction -= .15f;
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        if(!borderhit(pD, NULL, minimapSize.x, minimapSize.y) &&
            !hittingWall(cellSize, player->point, pD, NULL, NULL)) player->direction += .15f;
    }
}

int main(void)
{

    // init window
    const int screenPx = 80;
    const int screenWidth = screenPx * 16;
    const int screenHeight = screenPx * 9;

    Vector2 cellSize = {25, 25};
    //Vector2 cellSize = {screenWidth / mapSize, screenHeight / mapSize};
    const Vector2 minimapSize = {cellSize.x * mapSize, cellSize.y * mapSize};

    Vector2 pPoint = {700, 485};

    player p1MM = {posInMap(pPoint, (Vector2){screenWidth, screenHeight}, minimapSize), -90};
    player p1 = {pPoint, 0};

    // Vector2 point = {1054, 233};
    //  Vector2 point = {400, 600};
    //  Vector2 playerPos = point;

    InitWindow(screenWidth, screenHeight, "My Raylib Window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float FOV = 45;
        
        //DrawLineEx((Vector2){640, 0}, (Vector2) {640, 720}, 1.0f, BLACK);
        rayFOV(cellSize, p1MM, minimapSize.x, minimapSize.y, FOV);

        //DrawRectangleV((Vector2){0, 0}, minimapSize, LIGHTGRAY);
        
        drawGrid(cellSize, minimapSize.x, minimapSize.y);
        //drawGrid(cellSize, screenWidth, screenHeight);

        //DrawCircleV(p1.point, 5, RED);
        DrawCircleV(p1MM.point, 3, RED);


        //rayFOV(cellSize, p1, screenWidth, screenHeight, FOV);
        
        
        //drawFOV(p1, FOV);
        //drawFOV(p1MM, FOV);

        playerMouvement(&p1MM, minimapSize, cellSize, FOV);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}