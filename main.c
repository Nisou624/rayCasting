#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <math.h>


const int map[20][20]= {
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

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

    for (size_t i = 0; i < 20; i++)
    {
        for (size_t j = 0; j < 20; j++)
        {
            if(map[i][j] == 1){
                DrawRectangle(j * cellSize.x, i * cellSize.y, cellSize.x, cellSize.y, LIGHTGRAY);
            }
        }
        
    }
    
    
}

float distanceVec(Vector2 p1, Vector2 p2){
    return sqrtf(powf(p2.x - p1.x, 2.0f) + powf(p2.y - p1.y, 2.0f));
}


Vector2 drawNextPoint(Vector2 cellsize, Vector2 p1, Vector2 p2){

    Vector2 nPoint1, nPoint2;

    /*
    y1 = x1 * m + b
    y2 = x2 * m + b

    m = (y2 - y1) / (x2 - x1)
    y3 = x3 * m + b

    */

    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    float m = dy / dx;
    float b = p1.y - m * p1.x;

    if (m != 0){
        nPoint1.x = snap(p2.x, dx, cellsize.x);
        nPoint1.y = nPoint1.x * m + b;

        nPoint2.y = snap(p2.y, dy, cellsize.y);
        nPoint2.x = (nPoint2.y - b) / m;

        if (distanceVec(p2, nPoint1) < distanceVec(p2, nPoint2)) return nPoint1;
        else return nPoint2;
    }else{
        nPoint1.y = snap(p2.y, dy, cellsize.y);
        nPoint1.x = p2.x;
        return nPoint1;
    }

}

int main(void)
{
    //srand(time(NULL));
    
    // Initialize the window
    const int screenPx = 80; 
    const int screenWidth  = screenPx * 16; //1280
    const int screenHeight = screenPx * 9; //720

    //cell Size
    Vector2 cellSize = {screenWidth / 20, screenHeight / 20};

    Vector2 point = {400, 600};

    Vector2 playerPos = {0, 0};

    InitWindow(screenWidth, screenHeight, "My Raylib Window");

    printf("MAP[0][1] SHOULD BE 1, IT IS IN FACT: %d\n", map[0][1]);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        drawGrid(cellSize, screenWidth, screenHeight);

        DrawCircleV(point, 10, RED);
        
        playerPos = (Vector2) {GetMouseX(), GetMouseY()};
        DrawCircle(playerPos.x, playerPos.y, 10, RED);

        DrawLineV(point, playerPos, BLUE);

        //drawNextPoint(cellSize, point, playerPos);
        Vector2 p2 = drawNextPoint(cellSize, point, playerPos);
        DrawCircleV(p2, 10, GREEN);

        bool wallhit = false;
        while (!wallhit && !borderhit(p2, cellSize, screenWidth, screenHeight))
        {
            Vector2 p3 = drawNextPoint(cellSize, playerPos, p2);
            DrawCircleV(p3, 10, GREEN);
            if (hittingWall(cellSize, p2, p3)){
                wallhit = true;
                break;
            }else{
                p2 = p3;
            }
        }
        


        

        //DrawRectangleV((Vector2) {cellSize.x * mouseCell.x , mouseCell.y * cellSize.y}, cellSize , LIGHTGRAY);
        


        EndDrawing();
    }

    // Close the window
    CloseWindow();

    return 0;
}