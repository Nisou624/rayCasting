#include "map.h"


const int mapSize = 10;

Vector2 cellSize = {25, 25};

Vector2 miniMapSize = {250, 250}; //{cellSize.x * mapSize, cellSize.y * mapSize}

Vector2 screenSize = {1280, 720};

const int map[10][10]= {
    {1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 4, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const Color colors[COLORS_LEN] = {
    RED, GOLD, BLUE, LIME, VIOLET
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
            if(map[i][j] != 0){
                DrawRectangle(j * cellSize.x, i * cellSize.y, cellSize.x, cellSize.y, colors[map[i][j] - 1]);
            }
        }
        
    }
}

bool borderhit(Vector2 point, Vector2 *border, float angle){

   Vector2 b;
   float dx, dy;

    if (point.x <= 0)
    {
        b = (Vector2){0, point.y - (point.x / tanf(angle))};
        if (border != NULL) *border = b;
        
    }else if (point.x >= miniMapSize.x){
        dx = point.x - miniMapSize.x;
        b = (Vector2){miniMapSize.x, point.y - (dx * tanf(angle))};
        if (border != NULL) *border = b;
        
    }else if (point.y <= 0){
        b = (Vector2){point.x - (point.y * tanf(angle)), 0};
        if (border != NULL) *border = b;
        
    }else if (point.y >= miniMapSize.y){
        dy = point.y - miniMapSize.y;
        b = (Vector2){point.x - (dy * tanf(angle)), miniMapSize.y};
        if (border != NULL) *border = b;
        
    }else{
        return false;
    }

    return true;
}

bool hittingWall(Vector2 cellSize, Vector2 p1, Vector2 p2, Vector2 *hitPoint, Color *color){
    
    Vector2 d = distance(p1, p2);
    Vector2 cell = {
        (snap(p2.x, d.x, cellSize.x) / cellSize.x) - (d.x > 0 ? 1 : 0), 
        (snap(p2.y, d.y, cellSize.y) / cellSize.y) - (d.y > 0 ? 1 : 0)
        
    };

    if(cell.x < 0 || cell.y < 0 || cell.x >= mapSize || cell.y >= mapSize) return false;
    int mapIndex = map[(int)cell.y][(int)cell.x];
    
    if(mapIndex != 0) {
        Vector2 wallPoint = {snap(p2.x, d.x, cellSize.x), snap(p2.y, d.y, cellSize.y)};
        if(hitPoint != NULL) *hitPoint = p2;
        if(color != NULL && mapIndex > 0 && mapIndex <= COLORS_LEN) *color = colors[mapIndex - 1];
        return true; 
    }
    else return false;
};