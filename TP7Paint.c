#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "raylib Paint by Vinciane Leclercq");
    
    
    Rectangle zoneDeDessin = { 25, 25, screenWidth - 50, screenHeight - 50 };
    Rectangle barreOutils = { 5, 5, screenWidth - 10, 75};
    
    //
    

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
        BeginDrawing();

            ClearBackground(GRAY);

            DrawText("Congrats! You created your first window!", screenHeight/2, screenWidth/4, 20, LIGHTGRAY);
            DrawRectangleRec(zoneDeDessin, WHITE);
            DrawRectangleRec(barreOutils, RAYWHITE);
            DrawRectangleLines(5, 5, screenWidth - 10, 75, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}