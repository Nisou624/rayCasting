#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <math.h>
#include "map.h"

Vector2 pozsdInMap(Vector2 point, Vector2 screenSize, Vector2 mapSize)
{
    return (Vector2){mapSize.x * point.x / screenSize.x, mapSize.y * point.y / screenSize.y};
}

Vector2 realPos(Vector2 point){
    return (Vector2){point.x * screenSize.x / miniMapSize.x, point.y * screenSize.y / miniMapSize.y};
}

void vecAdd(Vector2 *v1, Vector2 v2)
{
    v1->x += v2.x;
    v1->y += v2.y;
}

void drawMiniMap(player player, float FOV, Vector2 cellSize, Vector2 minimapSize){
    drawGrid(cellSize, minimapSize.x, minimapSize.y);

    DrawCircleV(player.point, 5, RED);

    drawFOV(player, FOV);
};


void drawPoint(Vector2 point, Vector2 minimapSize){
    Vector2 border;
    if (!borderhit(point, &border, 0)){
        DrawCircleV(point, 5, RED);
    }else{
        DrawCircleV(border, 5, RED);
    }
}

void playerMouvement(player *player, Vector2 minimapSize, Vector2 cellSize, float FOV) {
    Vector2 pG = helperPointFromAngle(player->point, toRad(player->direction) - toRad(FOV / 2), 10);
    Vector2 pD = helperPointFromAngle(player->point, toRad(player->direction) + toRad(FOV / 2), 10);

    Vector2 nextPoint = player->point;

    //TODO: the player should move even when pg or pd hits border of a wall 
    if (IsKeyDown(KEY_W)) {
        Vector2 tempPoint = helperPointFromAngle(nextPoint, toRad(player->direction), .2f);
        /*
            commented out as a temp solution:
            && !borderhit(pG, NULL, minimapSize.x, minimapSize.y) && !hittingWall(cellSize, player->point, pG, NULL, NULL) && !borderhit(pD, NULL, minimapSize.x, minimapSize.y) && !hittingWall(cellSize, player->point, pD, NULL, NULL)
        */
        if (!borderhit(tempPoint, NULL, 0) && !hittingWall(cellSize, player->point, tempPoint, NULL, NULL)) {
            nextPoint = tempPoint;
        }
    }
    if (IsKeyDown(KEY_S)) {
        Vector2 tempPoint = helperPointFromAngle(nextPoint, toRad(player->direction), -0.2f);
        if (!borderhit(tempPoint, NULL, 0) &&
            !hittingWall(cellSize, player->point, tempPoint, NULL, NULL)) {
            nextPoint = tempPoint;
        }
    }
    if (IsKeyDown(KEY_A)) {
        Vector2 tempPoint = helperPointFromAngle(nextPoint, toRad(player->direction - 90), .2f);
        if (!borderhit(pG, NULL, 0) &&
            !hittingWall(cellSize, player->point, pG, NULL, NULL)) {
            nextPoint = tempPoint;
        }
    }
    if (IsKeyDown(KEY_D)) {
        Vector2 tempPoint = helperPointFromAngle(nextPoint, toRad(player->direction + 90), .2f);
        if (!borderhit(pD, NULL, 0) &&
            !hittingWall(cellSize, player->point, pD, NULL, NULL)) {
            nextPoint = tempPoint;
        }
    }

    player->point = nextPoint;

    if (IsKeyDown(KEY_LEFT)) {
        if(!borderhit(pG, NULL, 0) &&
            !hittingWall(cellSize, player->point, pG, NULL, NULL)){
                player->direction -= .2f; // Adjust rotation speed as needed
            } 
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (!borderhit(pD, NULL, 0) &&
            !hittingWall(cellSize, player->point, pD, NULL, NULL)){
            player->direction += .2f; // Adjust rotation speed as needed
        }
    }
}

void drawMap(player player, float FOV){
    DrawRectangleV((Vector2){0,0}, miniMapSize, GOLD);
    drawGrid(cellSize, miniMapSize.x, miniMapSize.y);
    DrawCircleV(player.point, 3, RED);

    Vector2 pG = helperPointFromAngle(player.point, toRad(player.direction) - toRad(FOV / 2), 10);
    DrawCircleV(pG, 3, GREEN);

    Vector2 pD = helperPointFromAngle(player.point, toRad(player.direction) + toRad(FOV / 2), 10);
    DrawCircleV(pD, 3, BLUE);
}

int main(void)
{

    // init window
    const int screenPx = 80;
    const int screenWidth = screenPx * 16;
    const int screenHeight = screenPx * 9;

    Vector2 pPoint = {670, 690};

    player p1MM = {posInMap(pPoint, (Vector2){screenWidth, screenHeight}, miniMapSize), -45};
    player p1 = {pPoint, 0};

    InitWindow(screenWidth, screenHeight, "My Raylib Window");

    Image img = LoadImage("res/wezu_tex_cc_by/wall1_color.png");
    Texture2D wall = LoadTextureFromImage(img);
    UnloadImage(img);

    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        float FOV = 45;
        playerMouvement(&p1MM, miniMapSize, cellSize, FOV);
        
        
        BeginDrawing();
        ClearBackground(GRAY);


        drawMap(p1MM, FOV);
        
        rayFOV(cellSize, p1MM, FOV, wall);

        if(IsKeyDown(KEY_P)){
            Vector2 realpos = realPos(p1MM.point);
            printf("Player coordinates: (%f, %f)\n", realpos.x, realpos.y);
        }
        

        DrawText(TextFormat("CURRENT FPS: %i", GetFPS()), GetScreenWidth() - 220, 40, 20, GOLD);

        
        EndDrawing();
    }

    UnloadTexture(wall);

    CloseWindow();

    return 0;
}