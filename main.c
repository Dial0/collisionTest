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
void DrawArrow(Vector3 start, Vector3 end, float size, Color color) {
    Vector3 dir = Vector3Normalize(Vector3Subtract(end,start));
    float length = Vector3Distance(start,end);
    Vector3 arrowStart = Vector3Add(start,Vector3Scale(dir, length*0.8f));
    DrawCylinderEx(arrowStart, end,size*2,0.00f,10,color);
    DrawCylinderEx(start, arrowStart,size,size,10,color);
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