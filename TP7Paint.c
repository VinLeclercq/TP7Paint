#include "raylib.h"

#define NB_OUTILS  3
#define NB_COULEUR 24

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "raylib Paint by Vinciane Leclercq");
    
    //creer les element graphique
    Rectangle zoneDeDessin = { 25, 25, screenWidth - 50, screenHeight - 50 };
    Rectangle barreOutils = { 5, 5, screenWidth - 10, 75};
    
    //------------------- les outils de la barre d'outils -----------------
    
    //Outils a choisir
    char* outils[NB_OUTILS] = {
        "DEPLACER", "DESSINER", "EFFACER" };
        
    //creation des rectangles outils
    Rectangle outilsRec[NB_OUTILS];
    for (int i = 0; i < NB_OUTILS; i++)
    {
        outilsRec[i].x = 10 + 70*i;
        outilsRec[i].y = 10;
        outilsRec[i].width = 65;
        outilsRec[i].height = 65;
    }
    
    //Tableau des icons outils
    Image outilsIcon[NB_OUTILS]; 
    outilsIcon[0] = LoadImage("png/001-cursor.png"); 
    outilsIcon[1] = LoadImage("png/001-brush.png");
    outilsIcon[2] = LoadImage("png/006-eraser.png");
    
    //Tableau de Texture pour les icons
    Texture2D outilsTexture[NB_OUTILS];
    for (int i = 0; i < NB_OUTILS; i++)
    {
        outilsTexture[i] = LoadTextureFromImage(outilsIcon[i]);
        UnloadImage(outilsIcon[i]);
    }
    
    //---------------- les couleurs de la barre d'outils ---------------
    
    //couleurs à choisir
    Color choixCouleur[NB_COULEUR] = {
        YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
        SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
        LIGHTGRAY, GRAY, DARKGRAY, BLACK, WHITE, RAYWHITE };
    
    //Tableau de rectangle pour les couleurs
    Rectangle couleurRec[NB_COULEUR];
    for (int i = 0; i < NB_COULEUR; i++)
    {
        if (i > 11)
        {
            couleurRec[i].x = screenWidth - 8 - 5 - 24 - (i-12)*(24+8);
            couleurRec[i].y = 5 + 16 + 24;
            couleurRec[i].width = 24;
            couleurRec[i].height = 24;
        }
        else 
        {
            couleurRec[i].x = screenWidth - 8 - 5 - 24 - i*(24+8);
            couleurRec[i].y = 5 + 8;
            couleurRec[i].width = 24;
            couleurRec[i].height = 24;
        }
    }
    
    //---------------------- les couleurs selectionnées ---------------------
    
    //Rectangles pour la couleur click-Droit click-gauche
    Rectangle couleurDroit = { screenWidth - 421 - 8, 10, 65, 65 };
    Rectangle couleurGauche = { screenWidth - 421 - 18 - 65, 10, 65, 65 };
    
    // variables
    
    Color cDroit = WHITE;
    Color cGauche = BLACK;

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
            
            //dessine les outils
            for (int i = 0; i < NB_OUTILS; i++)
            {
                DrawRectangleRec(outilsRec[i], RAYWHITE);
                DrawTexture(outilsTexture[i], outilsRec[i].x + 15, outilsRec[i].y + 15, WHITE);
                DrawRectangleLines(outilsRec[i].x, outilsRec[i].y, outilsRec[i].width, outilsRec[i].height, GRAY);
            }
            
            //dessine les couleurs
            for (int i = 0; i < NB_COULEUR; i++)
            {
                DrawRectangleRec(couleurRec[i], choixCouleur[i]);
                DrawRectangleLines(couleurRec[i].x, couleurRec[i].y, couleurRec[i].width, couleurRec[i].height, GRAY);
            }
             
             //dessine les couleurs selectionnées
             DrawRectangleRec(couleurDroit, cDroit);
             DrawRectangleLines(couleurDroit.x, couleurDroit.y, couleurDroit.width, couleurDroit.height, GRAY);
             DrawRectangleRec(couleurGauche, cGauche);
             DrawRectangleLines(couleurGauche.x, couleurGauche.y, couleurGauche.width, couleurGauche.height, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    for (int i = 0; i < NB_OUTILS; i++)
    {
        UnloadTexture(outilsTexture[i]);
    }
    
    
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}