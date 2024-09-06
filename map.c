#include "map.h"


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

bool borderhit(Vector2 point, Vector2 *border, int screenWidth, int screenHeight){
    if (point.x < 0)
    {
        if (border != NULL) border->x = 0;
        return true;
    }else if (point.x > screenWidth){
        if (border != NULL) border->x = screenWidth;
        return true;
    }else if (point.y < 0){
        if (border != NULL) border->y = 0;
        return true;
    }else if (point.y > screenHeight){
        if (border != NULL) border->y = screenHeight;
        return true;
    }
    return false;
}

bool hittingWall(Vector2 cellSize, Vector2 p1, Vector2 p2, Vector2 *hitPoint){
    
    Vector2 d = distance(p1, p2);
    Vector2 cell = {
        (snap(p2.x, d.x, cellSize.x) / cellSize.x) - (d.x > 0 ? 1 : 0), 
        (snap(p2.y, d.y, cellSize.y) / cellSize.y) - (d.y > 0 ? 1 : 0)
        
    };

    if(map[(int)cell.y][(int)cell.x] == 1) {
        if(hitPoint != NULL) hitPoint = &cell;
        return true; 
    }
    else return false;
};