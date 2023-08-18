/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

typedef enum {
    NONE = 0, SURFACE, EDGE, POINT, INVALID;
} colType;

typedef struct {
    colType type;
    Vector3 tri[3];
    Vector3 closestPoint;
    Vector3 backstepStart;
    Vector3 BackstepEnd;

    Vector3 newSpherePos;

} triColEvent;

void DrawArrow(Vector3 start, Vector3 end, float size, Color color) {
    Vector3 dir = Vector3Normalize(Vector3Subtract(end,start));
    float length = Vector3Distance(start,end);
    Vector3 arrowStart = Vector3Add(start,Vector3Scale(dir, length*0.8f));
    DrawCylinderEx(arrowStart, end,size*2,0.00f,10,color);
    DrawCylinderEx(start, arrowStart,size,size,10,color);
}

Vector3 getTriangleNormal(Vector3 v1, Vector3 v2, Vector3 v3) {
    Vector3 edge1 = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
    Vector3 edge2 = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
    Vector3 normal = Vector3CrossProduct(edge1, edge2);
    float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= length;
    normal.y /= length;
    normal.z /= length;
    return normal;
}

Vector3 nearestPointOnLine(Vector3 l1, Vector3 l2, Vector3 p, bool* edge) {
    Vector3 lineDirection = Vector3Normalize(Vector3Subtract(l2, l1));
    Vector3 pointDirection = Vector3Subtract(p, l1);

    *edge = false;

    float distance = Vector3Distance(l1, l2);
    //float distance = Vector3DotProduct(pointDirection, lineDirection);
    float t = Vector3DotProduct(pointDirection, lineDirection);
    if (t < 0.0f) {
        return l1;
    }
    if (t > distance) {
        return l2;
    }
     *edge = true;
    return Vector3Add(l1, Vector3Scale(lineDirection, t));
}

typedef struct {
    colType collisionType;
    Vector3 closestPoint
    Vector3 edgePoint1;
    Vector3 edgePoint2;
}closestPointOnTri;

closestPointOnTri closestPointTriangle(Vector3 origin, Vector3 p1, Vector3 p2, Vector3 p3) {

    closestPointOnTri result;

    result.collisionType = NONE;

    Vector3 planeNormal = getTriangleNormal(p1, p2, p3);
    Vector3 inverseNormal = Vector3Subtract((struct Vector3) { 0.0f, 0.0f, 0.0f }, planeNormal);

    Vector3 OriginintersectionPoint;
    Ray OriginintersectionRay = { origin ,Vector3Normalize(inverseNormal) };
    bool Originhit = rayPlaneCollision(OriginintersectionRay, p1, p2, p3, &OriginintersectionPoint);
    float OrigindistToPlane = Vector3Distance(origin, OriginintersectionPoint);

    Vector3 closestPoint;
    Vector3 baryPoint = Vector3Barycenter(OriginintersectionPoint, p1, p2, p3);

    //if point inside triangle
    if (baryPoint.x >= 0 && baryPoint.y >= 0 && baryPoint.x + baryPoint.y <= 1) {
        closestPoint = OriginintersectionPoint;
        result.collisionType = SURFACE;
        result.edgePoint1 = p1;
        result.edgePoint2 = p1;
    }
    else {
        // Find nearest point on triangle edges
        closestPoint = p1;
        float closestDist = Vector3Distance(OriginintersectionPoint, closestPoint);
        bool closestOnEdge = false;
        int closestEdgeIdx = 0;

        Vector3 edgePoints[3] = { p1, p2, p3 };
        for (int i = 0; i < 3; i++) {
            Vector3 e1 = edgePoints[i];
            Vector3 e2 = edgePoints[(i + 1) % 3];
            bool onEdge;
            Vector3 ep = nearestPointOnLine(e1, e2, OriginintersectionPoint,&onEdge);
            float dist = Vector3Distance(OriginintersectionPoint, ep);
            if (dist < closestDist) {
                closestEdgeIdx = i;
                closestOnEdge = onEdge;
                closestDist = dist;
                closestPoint = ep;
            }
        }

        if(closestOnEdge){
            result.collisionType = EDGE;
            result.edgePoint1 = edgePoints[closestEdgeIdx];
            result.edgePoint2 = edgePoints[(closestEdgeIdx + 1) % 3];

        }else{
            result.collisionType = POINT;
            result.edgePoint1 = closestPoint;
            result.edgePoint2 = closestPoint;
        }
    }

    result.closestPoint = closestPoint;
}



triColEvent sphereTriCol(Vector3 Tri[3], Vector3 spherePos, Vector3 sphereDir, float moveDist) {

    triColEvent result;

    closestPointOnTri closestPoint = closestPointTriangle(spherePos,Tri[0],Tri[1],Tri[2]);

    if(Vector3Distance(spherePos,closestPoint) > 1.0f) {
        result.type = NONE;
        return result;
    }

    if(closestPointOnTri.type == POINT) {
        
    }

    result.type = INVALID;
    return result;
}


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Color mainSphereColor = Fade(BLUE, 0.3f);

    Vector3 ColTri[3] = { { -5.0f, 0.0f, 1.0f },{ -0.5f, 0.0f, 1.0f },{ 0.0f, 5.0f, 0.0f } };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        
        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawGrid(10,1.0f);
                //rlDisableDepthTest();
                DrawArrow((struct Vector3){0.0f,0.0f,0.0f}, (struct Vector3){1.0f,0.0f,0.0f},0.05f,GREEN);
                DrawTriangle3D(ColTri[0],ColTri[1],ColTri[2],BLUE);
                DrawSphere( (struct Vector3){0.0f,0.0f,0.0f}, 0.1f, RED);
                DrawSphere( (struct Vector3){0.0f,0.0f,0.0f}, 1.0f, mainSphereColor);
            //DrawTriangle3D(v1, v2, v3, RED);
            EndMode3D();

            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}