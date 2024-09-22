#include "misc.h"

Color darkenColor(Color color, float distance) {
    printf("Distance: %f\n", distance);
    float darkenAmount =  distance;
    //float darkenAmount = distance / 720.0f;
    Color darkenedColor;
    darkenedColor.r = (unsigned char)(color.r * darkenAmount);
    darkenedColor.g = (unsigned char)(color.g * darkenAmount);
    darkenedColor.b = (unsigned char)(color.b * darkenAmount);
    darkenedColor.a = color.a; // Preserve the alpha value
    return darkenedColor;
}

float distanceVec(Vector2 p1, Vector2 p2){
    return sqrtf(powf(p2.x - p1.x, 2.0f) + powf(p2.y - p1.y, 2.0f));
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

void drawRay(Vector2 cellSize, Vector2 p1, Vector2 p2, float angle, Vector2 *wallPoint, Vector2 *borderPoint, Color *wallColor){
    Vector2 nPoint1 = drawNextPoint(p1, p2, cellSize);
    DrawCircleV(nPoint1, 1, GOLD);
    while(!hittingWall(cellSize, p1, nPoint1, wallPoint, wallColor) && !borderhit(nPoint1, borderPoint, angle)){
        Vector2 nPoint2 = drawNextPoint(p2, nPoint1, cellSize);
        DrawCircleV(nPoint2, 1, GOLD);
        nPoint1 = nPoint2;
    }
    DrawCircleLinesV(*wallPoint, 1, RED);
    DrawCircleV(*borderPoint, 3, RED);
}


void castRay(Vector2 cellSize, Vector2 p1, Vector2 p2, Vector2 *wallPoint){
    Vector2 nPoint1 = drawNextPoint(p1, p2, cellSize);
    while(!hittingWall(cellSize, p1, nPoint1, wallPoint, NULL) && !borderhit(nPoint1, wallPoint, 0)){
        Vector2 nPoint2 = drawNextPoint(p2, nPoint1, cellSize);
        nPoint1 = nPoint2;
    }
}

Vector2 helperPointFromAngle(Vector2 point, float angle, float distance){
    Vector2 newPoint = {0, 0};
    newPoint.x = point.x + cos(angle) * distance;
    newPoint.y = point.y + sin(angle) * distance;
    return newPoint;
}

bool equalVec(Vector2 p1, Vector2 p2){
    return (p1.x == p2.x) && (p1.y == p2.y);
}

Vector2 addVec(Vector2 p1, Vector2 p2){
    return (Vector2){p1.x + p2.x, p1.y + p2.y};
}

void rayFOV(Vector2 cellSize, player player, float FOV, Texture2D texture){
    float miniMapScale = 10.0f;
    
    
    Vector2 helperPoint = helperPointFromAngle(player.point, toRad(player.direction), 10);

    Vector2 wallPoint = {0, 0};
    Vector2 borderPoint = {0, 0};
    Color wallColor;
    
    Vector2 pG = helperPointFromAngle(player.point, toRad(player.direction) - toRad(FOV / 2), 10);

    Vector2 pD = helperPointFromAngle(player.point, toRad(player.direction) + toRad(FOV / 2), 10);

    Vector2 dV = distance(pG, pD);
    Vector2 count = {0, 0};
    
    for (int x = 0; x < 1280; x++) {
        wallColor = BLACK;
        wallPoint = (Vector2){0, 0};
        borderPoint = (Vector2){0, 0};
        // Calculate the angle corresponding to this screen pixel
        float angle = toRad(player.direction - FOV / 2.0f) + (x / (float)1280) * toRad(FOV);

        // Cast ray from player's position to the angle
        drawRay(cellSize, player.point, helperPointFromAngle(player.point, angle, 10), angle, &wallPoint, &borderPoint, &wallColor);
        
        drawWall(player, borderPoint, BLACK, x, (Vector2){1280, 720}, FOV);
        if(wallPoint.x != 0 && wallPoint.y != 0){
            drawWall(player, wallPoint, wallColor, x, (Vector2){1280, 720}, FOV);
            //DrawTexturePro(texture, (Rectangle){x, 0, 1, 720}, (Rectangle){x, wallBegin.y, 1, wallH}, (Vector2){0, 0}, 0.0f, WHITE);
        }
        
    }    
}

//TODO: color walls
void drawWall(player player, Vector2 wallPoint, Color wallColor, int iter, Vector2 screenSize, float FOV){
    float angle = (toRad(player.direction - FOV / 2.0f)) + ((iter / screenSize.x) * toRad(FOV));
    float distance = distanceVec(player.point, wallPoint);
    float distanceCorrected = distance * cosf(angle - toRad(player.direction)); //
    float wallH = screenSize.y / (distanceCorrected / 10); // should be: screenheight / v.dot(d) where v = wallpoint sub player.point && d = 
    Vector2 wallBegin = {iter, (screenSize.y / 2) - (wallH / 2)};
    Vector2 wallEnd = {iter, (screenSize.y / 2) + (wallH / 2)};
    DrawLineEx(wallBegin, wallEnd, 1.0f, wallColor); //darkenColor(wallColor, 1468 / distanceCorrected / 10)
}


void drawFOV(player player, float FOV){
    Vector2 helperPoint = helperPointFromAngle(player.point, toRad(player.direction), 10);
    Vector2 pG = helperPointFromAngle(player.point, toRad(player.direction) - toRad(FOV/2), 10);
    DrawCircleV(pG, 3, GREEN);
    Vector2 pD = helperPointFromAngle(player.point, toRad(player.direction) + toRad(FOV/2), 10);
    DrawCircleV(pD, 3, BLUE);

    DrawTriangleLines(player.point, pD, pG, MAGENTA);
    
}

float toRad(float angle){
    return angle * PI / 180;
}