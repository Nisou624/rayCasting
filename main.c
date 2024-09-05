#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <math.h>


typedef struct {
    Vector2 point;
    float direction;
} player;


const int mapSize = 10;

const int map[10][10]= {
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

float distanceVec(Vector2 p1, Vector2 p2){
    return sqrtf(powf(p2.x - p1.x, 2.0f) + powf(p2.y - p1.y, 2.0f));
}

void drawGrid(Vector2 cellSize, int screenWidth, int screenHeight)
{
    for (int i = 0; i < screenWidth; i += cellSize.x)
    {
        DrawLine(i, 0, i, screenHeight, BLACK);
    }
    for (int i = 0; i < screenHeight; i += cellSize.y)
    {
        DrawLine(0, i, screenWidth, i, BLACK);
    }

    for (size_t i = 0; i < mapSize; i++)
    {
        for (size_t j = 0; j < mapSize; j++)
        {
            if(map[i][j] == 1){
                DrawRectangle(j * cellSize.x, i * cellSize.y, cellSize.x, cellSize.y, LIGHTGRAY);
            }
        }
        
    }
}

bool borderhit(Vector2 point, Vector2 cellSize, int screenWidth, int screenHeight){
    if (point.x < 0 || point.x > screenWidth || point.y < 0 || point.y > screenHeight) return true;
    else return false;
}

Vector2 distance(Vector2 p1, Vector2 p2){
    return (Vector2){p2.x - p1.x, p2.y - p1.y};
}

float snap(float x, float dx, float cellsize){
    float eps;
    if(__signbitf(dx)){
       eps = 1e-3 * (-1.0);
    }else{
       eps = 1e-3;
    };
    if (dx > 0) {
        return ceilf((x / cellsize) + eps) * cellsize;
    } else {
        return floorf((x / cellsize) + eps) * cellsize;
    }
}

bool hittingWall(Vector2 cellSize, Vector2 p1, Vector2 p2){
    
    Vector2 d = distance(p1, p2);
    Vector2 cell = {
        (snap(p2.x, d.x, cellSize.x) / cellSize.x) - (d.x > 0 ? 1 : 0), 
        (snap(p2.y, d.y, cellSize.y) / cellSize.y) - (d.y > 0 ? 1 : 0)
        
    };

    if(map[(int)cell.y][(int)cell.x] == 1) return true; 
    else return false;
};


Vector2 drawNextPoint(Vector2 p1, Vector2 p2, Vector2 cellSize){
    /*
        y1 = x1 * m + b
        y2 = x2 * m + b

        m =  (y2 - y1) / (x2 - x1)
        y3 = x3 * m + b

        cases:
        - dx = 0: line is vertical so we snap to the y axis
        - dx != 0:
            - m = 0: line is horizontal so we snap to the x axis
            - m != 0: line is diagonal so we snap to the x and y axis
    */
   Vector2 nPoint1;

   float dx = p2.x - p1.x;
   float dy = p2.y - p1.y;
   float m = dy / dx;
   float b = p1.y - m * p1.x;


   if(dx == 0)return (Vector2){p2.x, snap(p2.y, dy, cellSize.y)};
   else{
    if(m != 0){
        Vector2 nPoint2;
        nPoint1.x = snap(p2.x, dx, cellSize.x);
        nPoint1.y = nPoint1.x * m + b;

        nPoint2.y = snap(p2.y, dy, cellSize.y);
        nPoint2.x = (nPoint2.y - b) / m;

        if (distanceVec(p2, nPoint1) < distanceVec(p2, nPoint2)) return nPoint1;
        else return nPoint2;
    }else{
        
        nPoint1.x = snap(p2.x, dx, cellSize.x);
        nPoint1.y = p2.y;
    }
   }
    
}

void drawRay(Vector2 cellSize, Vector2 p1, Vector2 p2, int mapWidth, int mapHeight){
    Vector2 nPoint1 = drawNextPoint(p1, p2, cellSize);
    DrawCircleV(nPoint1, 5, GREEN);
    while(!hittingWall(cellSize, p1, nPoint1) && !borderhit(nPoint1, cellSize, mapWidth, mapHeight)){
        Vector2 nPoint2 = drawNextPoint(p2, nPoint1, cellSize);
        DrawCircleV(nPoint2, 5, GREEN);
        nPoint1 = nPoint2;
    }
}

float toRad(float angle){
    return angle * PI / 180;
}

Vector2 helperPointFromAngle(Vector2 point, float angle, int distance){
    Vector2 newPoint = {0, 0};
    newPoint.x = point.x + cos(angle) * distance;
    newPoint.y = point.y + sin(angle) * distance;
    return newPoint;
}

void rayFOV(Vector2 cellSize, player p1, int mapWidth, int mapHeight, float FOV){
    Vector2 helperPoint = helperPointFromAngle(p1.point, toRad(p1.direction), 30);
    
    Vector2 pG = helperPointFromAngle(p1.point, toRad(p1.direction) - FOV, 30);
    DrawCircleV(pG, 5, GREEN);

    Vector2 pD = helperPointFromAngle(p1.point, toRad(p1.direction) + FOV, 30);
    DrawCircleV(pD, 5, BLUE);

    for (float x = toRad(p1.direction) - FOV; x <= toRad(p1.direction) + FOV; x+= toRad(1))
    {
        drawRay(cellSize, p1.point, helperPointFromAngle(p1.point, x, 30), mapWidth, mapHeight);
    }
    
}

void drawFOV(Vector2 p1, float playerAngle, float FOV){
    Vector2 helperPoint = helperPointFromAngle(p1, playerAngle, 30);
    
    float dy = distance(helperPoint, p1).y;

    //Vector2 pG = {p1.x - (dy / tanf(FOV)), helperPoint.y};
    Vector2 pG = helperPointFromAngle(p1, playerAngle - FOV, 30);
    DrawCircleV(pG, 5, GREEN);
    // Vector2 pD = {(dy / tanf(FOV)) + p1.x, helperPoint.y};
    Vector2 pD = helperPointFromAngle(p1, playerAngle + FOV, 30);
    DrawCircleV(pD, 5, BLUE);

    DrawTriangleLines(p1, pD, pG, MAGENTA);
    
}


int main(void){

    //init window
    const int screenPx = 80;
    const int screenWidth = screenPx * 16;
    const int screenHeight = screenPx * 9;

    // Vector2 cellSize = {15, 15};
    Vector2 cellSize = {screenWidth / mapSize, screenHeight / mapSize};
    const Vector2 minimapSize = {cellSize.x * mapSize, cellSize.y * mapSize};


    player p1 = {{578, 600}, -100};

    //Vector2 point = {1054, 233};
    // Vector2 point = {400, 600};
    // Vector2 playerPos = point;

    InitWindow(screenWidth, screenHeight, "My Raylib Window");

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // drawGrid(cellSize, minimapSize.x, minimapSize.y);
        drawGrid(cellSize, screenWidth, screenHeight);

        DrawCircleV(p1.point, 5, RED);

        //Vector2 helperPoint = {400, 590};
        //DrawCircleV(helperPoint, 5, YELLOW);

        float FOV = 30 * PI / 180;

        rayFOV(cellSize, p1, screenWidth, screenHeight, FOV);
        //drawFOV(p1.point, toRad(p1.direction), FOV);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;

}