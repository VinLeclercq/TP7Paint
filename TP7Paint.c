#include "raylib.h"

#define NB_OUTILS  7
#define NB_COULEUR 24
#define NB_CURSEUR 4

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "raylib Paint by Vinciane Leclercq");
    
    //variable de taille de la zone de dessin
    int xDessin = 25, yDessin = 25, widthDessin = screenWidth - 50, heightDessin = screenHeight -50;
    
    //creer les element graphique
   // Rectangle zoneDeDessin = { xDessin, yDessin, widthDessin, heightDessin };
    Rectangle barreOutils = { 5, 5, screenWidth - 10, 75 };
    
    //creer la toile pour dessiner
    RenderTexture2D dessin = LoadRenderTexture(widthDessin, heightDessin);
    
    BeginTextureMode(dessin);
    ClearBackground(WHITE);
    EndTextureMode();
    
    
    //------------------- les outils de la barre d'outils -----------------
    
    //Outils a choisir
    char* outils[NB_OUTILS] = {
        "ENREGISTRER", "DEPLACER", "DESSINER_ROND", "DESSINER_CARREE", "EFFACER", "SEAU", "PIPETTE" };
        
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
    outilsIcon[0] = LoadImage("png/001-save.png"); 
    outilsIcon[1] = LoadImage("png/001-cursor.png"); 
    outilsIcon[2] = LoadImage("png/001-brush.png");
    outilsIcon[3] = LoadImage("png/002-brush-1.png");
    outilsIcon[4] = LoadImage("png/006-eraser.png");
    outilsIcon[5] = LoadImage("png/003-bucket.png");
    outilsIcon[6] = LoadImage("png/001-dropper.png");
    
    //Tableau de Texture pour les icons
    Texture2D outilsTexture[NB_OUTILS];
    for (int i = 0; i < NB_OUTILS; i++)
    {
        outilsTexture[i] = LoadTextureFromImage(outilsIcon[i]);
        UnloadImage(outilsIcon[i]);
    }
    
    //---------------- les curseurs -----------------------
    
    Image curseur[NB_CURSEUR];
    curseur[0] = LoadImage("png/001-cursor.png");
    curseur[1] = LoadImage("png/001-eraser.png");
    curseur[2] = LoadImage("png/001-paint-bucket.png");
    curseur[3] = LoadImage("png/001-pipette.png");
    
    Texture2D curseurTexture[NB_CURSEUR];
    for (int i = 0; i < NB_CURSEUR; i++)
    {
        curseurTexture[i] = LoadTextureFromImage(curseur[i]);
        UnloadImage(curseur[i]);
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

    int couleurSousSouris = -1;
    int couleurSelectionneeDroit = 22;
    int couleurSelectionneeGauche = 21;
    
    Color cDroit = choixCouleur[couleurSelectionneeDroit];
    Color cGauche = choixCouleur[couleurSelectionneeGauche];

    //-------------------- le peinceau ------------------------
    
    int taillePeinceau = 20;
    
    //souris
    Vector2 positionSouris = { 500.0f, 400.0f };
    Vector2 positionAvant = positionSouris;
    
    //---------------------les differents modes---------------------
    
    // initialisation du tableau permettant de savoir sous quel mode l'utilisateur est
    bool modeOutils[NB_OUTILS];
    for (int i = 0; i < NB_OUTILS; i++)
    {
        if (i == 1) modeOutils[i] = true; // mode initial sur pinceau
        else modeOutils[i] = false;
    }
    
    int outilsSousSouris = -1;
    int outilsSelectionnee = 2;
    
    //--------------------le mode camera-----------------------
    
    Camera2D deplacement = { 0 };
    deplacement.target = (Vector2){ (xDessin + widthDessin/2) * 1.0f, (yDessin + heightDessin/2) * 1.0f };
    deplacement.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    deplacement.rotation = 0.0f;
    deplacement.zoom = 1.0f;
    
    float dessinZoomX = xDessin * deplacement.zoom;
    float dessinZoomY = yDessin * deplacement.zoom;
    float dessinZoomWidth = widthDessin * deplacement.zoom;
    float dessinZoomHeight = heightDessin * deplacement.zoom;
    
    Vector2 targetAvant = deplacement.target;
    
    Vector2 posSourisDessin = { ( positionSouris.x - (screenWidth - 50 - dessinZoomWidth)/ 2 ) / deplacement.zoom, ( positionSouris.y - (screenHeight - 50 - dessinZoomHeight) / 2) / deplacement.zoom };
    Vector2 posAvantSourisDessin = posSourisDessin;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        positionSouris = GetMousePosition();
        
        //action entre la souris et les outils
        for (int i = 0; i < NB_OUTILS; i++)
        {
            if (CheckCollisionPointRec(positionSouris, outilsRec[i]))
            {
                outilsSousSouris = i; 
                
                //choix de l'outils avec la souris
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) outilsSelectionnee = i;
                break; // sort de la boucle, gain de temps
            }
            else outilsSousSouris = -1;
        }
        
        for (int i = 0; i < NB_COULEUR; i ++)
        {
            if (CheckCollisionPointRec(positionSouris, couleurRec[i]))
            {
                couleurSousSouris = i;
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) couleurSelectionneeGauche = i;
                else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) couleurSelectionneeDroit = i;
                break;
            }
            else couleurSousSouris = -1;
        }
        cDroit = choixCouleur[couleurSelectionneeDroit];
        cGauche = choixCouleur[couleurSelectionneeGauche];
        
        // zoom
        deplacement.zoom += ((float)GetMouseWheelMove()*0.05f);
        
        if (deplacement.zoom > 3.0f) deplacement.zoom = 3.0f;
        else if (deplacement.zoom < 0.1f) deplacement.zoom = 0.1f;
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (GetGestureDetected() == GESTURE_DRAG))
        {   
            if (outilsSelectionnee == 1)
            {
                deplacement.target.x = targetAvant.x + (positionAvant.x - positionSouris.x);
                deplacement.target.y = targetAvant.y + (positionAvant.y - positionSouris.y);
            }
            
            posSourisDessin.x = ( positionSouris.x - (screenWidth - 50 - dessinZoomWidth)/ 2 ) / deplacement.zoom;
            posSourisDessin.y = ( positionSouris.y - (screenHeight - 50 - dessinZoomHeight) / 2) / deplacement.zoom;
            
            if(CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }))
            {
                switch (outilsSelectionnee)
                {
  
                    case 2 :
                        BeginTextureMode(dessin);
                        DrawCircle(posSourisDessin.x, posSourisDessin.y, taillePeinceau, cGauche);
                        DrawLineEx((Vector2){ posAvantSourisDessin.x, posAvantSourisDessin.y}, (Vector2){ posSourisDessin.x, posSourisDessin.y },2 * taillePeinceau, cGauche);
                        EndTextureMode();
                        break;
                        
                    case 3 :
                        break;
                        
                    case 4 :
                        break;
                        
                        
                    case 5 :
                        break;
                        
                    case 6 :
                        break;
                    
                }
            }
        }
        
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && (GetGestureDetected() == GESTURE_DRAG))
        {   
    
            if(CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }))
            {
                switch (outilsSelectionnee)
                {
  
                    case 2 :
                        BeginTextureMode(dessin);
                        DrawCircle(positionSouris.x, positionSouris.y - 2 * yDessin, taillePeinceau, cDroit );
                        DrawLineEx(positionAvant, positionSouris, taillePeinceau, cDroit);
                        EndTextureMode();
                        break;
                        
                    case 3 :
                        break;
                        
                    case 4 :
                        break;
                        
                        
                    case 5 :
                        break;
                        
                    case 6 :
                        break;
                    
                }
            }
        }
        
        dessinZoomWidth = widthDessin * deplacement.zoom;
        dessinZoomHeight = heightDessin * deplacement.zoom;
        
        dessinZoomX = (screenWidth - dessinZoomWidth) / 2;
        dessinZoomY = (screenHeight - dessinZoomHeight) /2;
        
        posSourisDessin.x = ( positionSouris.x - (screenWidth - 50 - dessinZoomWidth)/ 2 ) / deplacement.zoom;
        posSourisDessin.y = ( positionSouris.y - (screenHeight - 50 - dessinZoomHeight) / 2) / deplacement.zoom;
        
        posAvantSourisDessin = posSourisDessin;
        
        positionAvant = positionSouris;
        targetAvant = deplacement.target;
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

           ClearBackground(GRAY);
           
           BeginMode2D(deplacement);
            
                //dessine le dessin
                DrawTextureRec(dessin.texture, (Rectangle){ 25, 25, dessin.texture.width, -dessin.texture.height }, (Vector2){ 25, 25 }, WHITE);
            
            EndMode2D();
            
            if (outilsSelectionnee == 1)
            {
                DrawTexture(curseurTexture[0], positionSouris.x, positionSouris.y, WHITE);
            }
            
            //dessine la forme du peinceau sous la souris
            if(CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }))
            {
                switch (outilsSelectionnee)
                {
                    
                    case 2 :
                        if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) DrawCircleLines(positionSouris.x, positionSouris.y, taillePeinceau, cDroit);
                        else DrawCircleLines(GetMouseX(), GetMouseY(), taillePeinceau, cGauche);
                        break;
                    
                    case 3 :
                        if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) DrawRectangleLines(positionSouris.x, positionSouris.y, taillePeinceau, taillePeinceau, cDroit);
                        else DrawRectangleLines(GetMouseX() - taillePeinceau/2, GetMouseY() - taillePeinceau/2, taillePeinceau, taillePeinceau, cGauche);
                        break;
                    
                    case 4 :
                        DrawCircleLines(GetMouseX(), GetMouseY(), taillePeinceau, BLACK);
                        DrawTexture(curseurTexture[1], positionSouris.x + taillePeinceau/2 + 2, positionSouris.y + taillePeinceau/2 + 2, WHITE);
                        break;
                    
                    case 5 :
                        DrawTexture(curseurTexture[2], positionSouris.x - 24, positionSouris.y - 24, WHITE);
                        break;
                    
                    case 6 :
                        DrawTexture(curseurTexture[3], positionSouris.x, positionSouris.y - 24, WHITE);
                        break;
                }
            }
            
            // dessine la barre d'outils
            DrawRectangleRec(barreOutils, RAYWHITE);
            DrawRectangleLines(5, 5, screenWidth - 10, 75, GRAY);
            
            //dessine les outils
            for (int i = 0; i < NB_OUTILS; i++)
            {
                DrawRectangleRec(outilsRec[i], RAYWHITE);
                DrawTexture(outilsTexture[i], outilsRec[i].x + 15, outilsRec[i].y + 15, WHITE);
                DrawRectangleLines(outilsRec[i].x, outilsRec[i].y, outilsRec[i].width, outilsRec[i].height, GRAY);
            }
            
            if (outilsSousSouris >= 0) DrawRectangleRec(outilsRec[outilsSousSouris], Fade(GRAY, 0.3f));
            
            DrawRectangleLines(outilsRec[outilsSelectionnee].x - 1, outilsRec[outilsSelectionnee].y - 1, outilsRec[outilsSelectionnee].width + 2, outilsRec[outilsSelectionnee].height + 2, BLACK);
            
            //dessine les couleurs
            for (int i = 0; i < NB_COULEUR; i++)
            {
                DrawRectangleRec(couleurRec[i], choixCouleur[i]);
                DrawRectangleLines(couleurRec[i].x, couleurRec[i].y, couleurRec[i].width, couleurRec[i].height, GRAY);
            }
            
            if (couleurSousSouris >= 0) DrawRectangleRec(couleurRec[couleurSousSouris], Fade(WHITE, 0.6f));
             
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
    
    for (int i = 0; i < NB_CURSEUR; i++)
    {
        UnloadTexture(curseurTexture[i]);
    }
    
    
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}