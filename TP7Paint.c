#include "raylib.h"
#include "stdlib.h"

#define NB_OUTILS  7
#define NB_COULEUR 24
#define NB_CURSEUR 4
#define HISTORIQUE 9

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
    
    //initialise le fond de la zone de dessin à blanc
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
    
    //initialisation du tableu contenant le Image des curseurs
    Image curseur[NB_CURSEUR];
    curseur[0] = LoadImage("png/001-cursor.png");
    curseur[1] = LoadImage("png/001-eraser.png");
    curseur[2] = LoadImage("png/001-paint-bucket.png");
    curseur[3] = LoadImage("png/001-pipette.png");
    
    //convertissement des curseurs en texture
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
    
    // variable
    int couleurSousSouris = -1; // pas de couleur
    int couleurSelectionneeDroit = 22; // noir
    int couleurSelectionneeGauche = 21; // blanc
    
    // couleur des boutons de la souris
    Color cDroit = choixCouleur[couleurSelectionneeDroit];
    Color cGauche = choixCouleur[couleurSelectionneeGauche];

    //-------------------- le peinceau ------------------------
    
    int taillePeinceau = 20;
    
    //souris
    Vector2 positionSouris = { 500.0f, 400.0f };
    //souris en mémoire
    Vector2 positionAvant = positionSouris;
    
    //---------------------la gestion des differents outils---------------------
    
    // initialisation du tableau permettant de savoir sous quel mode l'utilisateur est
    bool modeOutils[NB_OUTILS];
    for (int i = 0; i < NB_OUTILS; i++)
    {
        if (i == 1) modeOutils[i] = true; // mode initial sur pinceau
        else modeOutils[i] = false;
    }
    
    int outilsSousSouris = -1; // -1 pas d'outils
    int outilsSelectionnee = 2; // initialisation avec le pinceau selectionné
    int outilAvant =  outilsSelectionnee;
    
    //--------------------le mode camera-----------------------
    
    //initialisation de la camera 
    Camera2D deplacement = { 0 };
    deplacement.target = (Vector2){ //positionnement au milieu de la zone de dessin
    (xDessin + widthDessin/2) * 1.0f, 
    (yDessin + heightDessin/2) * 1.0f };
    deplacement.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    deplacement.rotation = 0.0f;
    deplacement.zoom = 1.0f;
    
    // calcule la position et la taille de la zone de travail en prenant en compte le zoom
    float dessinZoomWidth = widthDessin * deplacement.zoom;
    float dessinZoomHeight = heightDessin * deplacement.zoom;
    float dessinZoomX = (screenWidth - dessinZoomWidth) / 2;
    float dessinZoomY = (screenHeight - dessinZoomHeight) / 2;
    
    //target en mémoire 
    Vector2 targetAvant = deplacement.target;
    
    // calcule la position de la souris 
    Vector2 posSourisDessin = { 
     (positionSouris.x - (1 - deplacement.zoom) * widthDessin / 2) / deplacement.zoom, 
     (positionSouris.y  - (1 - deplacement.zoom) * heightDessin / 2) / deplacement.zoom };
    Vector2 posAvantSourisDessin = posSourisDessin;
    
    //---------------------------l'historique--------------------------------
    
    //variable qui récupère le geste de la souris
    int mouvementPresent = GESTURE_NONE;
    int mouvementPrecedent = mouvementPresent;
    
    //variable permettant de savoir si un deplacement dans l'historique est en cours
    bool ctrlZ_Active = false;
    bool ctrlY_Active = false;
    
    bool entreDansLhistorique = false;
    //premiere image (blanche) de l'historique
    Image blankHistorique = GetTextureData(dessin.texture);
    
    //tableau d'image pour l'historique
    Image* historique = malloc(10*sizeof(Image));
    historique[0] = blankHistorique;
    
    //emplacement de l'image afficher dans l'historique
    int indiceHistorique = 0;
    
    Texture2D textureHistorique;
    
    //---------------------------l'outil pipette & seau------------------------
    
    // image sous forme d'un pixel unique
    Image pixel;
    
    //rvariable pour récupérer l'image avec la couleur remplacer pour le pot de peinture
    Image* couleurRemplacee;
    Texture2D textureCouleurRemplacee;
    
    //couleur récupérer par la pipette
    Color* couleurPipette;
    
    //couleur récupérer par le pot de peinture
    Color* couleurSeau;
    
    //---------------------------la sauvegarde---------------------------
    
    // indiquateur pour permettre d'afficher le message de sauvegarde ou non
    bool messageDeSauvegarde = false;
    int compteur = 0;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        
        positionSouris = GetMousePosition();
        mouvementPrecedent = mouvementPresent;
        mouvementPresent = GetGestureDetected();
        
        //action entre la souris et les outils
        for (int i = 0; i < NB_OUTILS; i++)
        {
            if (CheckCollisionPointRec(positionSouris, outilsRec[i]))
            {
                outilsSousSouris = i; 
                
                //choix de l'outils avec la souris
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && GetGestureDetected() == GESTURE_TAP) 
                {
                    outilAvant = outilsSelectionnee;
                    outilsSelectionnee = i;
                }
                break; // sort de la boucle, gain de temps
            }
            else outilsSousSouris = -1; // aucun outils
        }
        
        //action entre la souris et les couleurs
        for (int i = 0; i < NB_COULEUR; i ++)
        {
            if (CheckCollisionPointRec(positionSouris, couleurRec[i]))
            {
                couleurSousSouris = i;
                
                //choix des couleurs en fonctions du bouton de la souris
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && GetGestureDetected() == GESTURE_TAP) 
                {
                    couleurSelectionneeGauche = i;
                    cGauche = choixCouleur[couleurSelectionneeGauche];
                }
                else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && GetGestureDetected() == GESTURE_TAP) 
                {
                    couleurSelectionneeDroit = i;
                    cDroit = choixCouleur[couleurSelectionneeDroit];
                }
                break;
            }
            else couleurSousSouris = -1; // aucune couleur
        }
        // récupération des couleurs choisies
        
        
        
        // action pour un click gauche appuyé
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (GetGestureDetected() == GESTURE_DRAG))
        {   
            if (outilsSelectionnee == 1) // pour l'outils deplacement
            {
                
                deplacement.target.x = targetAvant.x + (positionAvant.x - positionSouris.x);
                deplacement.target.y = targetAvant.y + (positionAvant.y - positionSouris.y);
            }
            
            posSourisDessin.x =  (positionSouris.x - (screenWidth - deplacement.zoom * widthDessin) / 2 ) / deplacement.zoom - (screenWidth/2 - deplacement.target.x);
            posSourisDessin.y =  (positionSouris.y - (screenHeight - heightDessin * deplacement.zoom) / 2 ) / deplacement.zoom - (screenHeight/2 - deplacement.target.y);
            
            //recupération des coordonée en fonction du zoom de la zone de dessin
            dessinZoomWidth = widthDessin * deplacement.zoom;
            dessinZoomHeight = heightDessin * deplacement.zoom;
            dessinZoomX = (screenWidth - dessinZoomWidth) / 2 + (screenWidth/2 - deplacement.target.x)*deplacement.zoom;
            dessinZoomY = (screenHeight - dessinZoomHeight) / 2  + (screenHeight/2 - deplacement.target.y)*deplacement.zoom;
            
            if(CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }))
            {
                switch (outilsSelectionnee)
                {
  
                    case 2 : // outil pinceau rond
                        BeginTextureMode(dessin);
                        DrawCircle(posAvantSourisDessin.x, posAvantSourisDessin.y, taillePeinceau, cGauche);
                        DrawLineEx(posAvantSourisDessin, posSourisDessin,2 * taillePeinceau, cGauche);
                        DrawCircle(posSourisDessin.x, posSourisDessin.y, taillePeinceau, cGauche);
                        EndTextureMode();
                        break;
                        
                    case 3 : // outils peinceau carré
                        BeginTextureMode(dessin);
                        DrawRectangle(posSourisDessin.x - taillePeinceau/2, posSourisDessin.y - taillePeinceau/2,  taillePeinceau, taillePeinceau, cGauche);
                        DrawLineEx(posAvantSourisDessin, posSourisDessin, taillePeinceau, cGauche);
                        EndTextureMode();
                        break;
                        
                    case 4 : // outil gomme
                        BeginTextureMode(dessin);
                        DrawCircle(posAvantSourisDessin.x, posAvantSourisDessin.y,  taillePeinceau, WHITE);
                        DrawLineEx((Vector2){ posAvantSourisDessin.x, posAvantSourisDessin.y}, (Vector2){ posSourisDessin.x, posSourisDessin.y },2 * taillePeinceau, WHITE);
                        DrawCircle(posSourisDessin.x, posSourisDessin.y, taillePeinceau, WHITE);
                        EndTextureMode();
                        break;
                    
                }
            }
        }
        
        // action du click droit
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && (GetGestureDetected() == GESTURE_DRAG))
        {   
            posSourisDessin.x =  (positionSouris.x - (screenWidth - deplacement.zoom * widthDessin) / 2 ) / deplacement.zoom - (screenWidth/2 - deplacement.target.x);
            posSourisDessin.y =  (positionSouris.y - (screenHeight - heightDessin * deplacement.zoom) / 2 ) / deplacement.zoom - (screenHeight/2 - deplacement.target.y);
            
            if(CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }))
            {
                switch (outilsSelectionnee)
                {
  
                    case 2 : // outils pinceau rond
                        BeginTextureMode(dessin);
                        DrawCircle(posAvantSourisDessin.x, posAvantSourisDessin.y, taillePeinceau, cDroit);
                        DrawLineEx(posAvantSourisDessin, posSourisDessin,2 * taillePeinceau, cDroit);
                        DrawCircle(posSourisDessin.x, posSourisDessin.y, taillePeinceau, cDroit);
                        EndTextureMode();
                        break;
                        
                    case 3 : // our peinceau carré
                        BeginTextureMode(dessin);
                        DrawRectangle(posSourisDessin.x - taillePeinceau/2, posSourisDessin.y - taillePeinceau/2,  taillePeinceau, taillePeinceau, cDroit);
                        DrawLineEx(posAvantSourisDessin, posSourisDessin, taillePeinceau, cDroit);
                        EndTextureMode();
                        break;
                }
            }
        }
        
        //action de la pipette
        if (outilsSelectionnee == 6 && CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight })) //pipette
        {
            //recupération du pixel sur lequel la souris est placée
            pixel =  ImageFromImage(historique[indiceHistorique], (Rectangle){ posSourisDessin.x ,posSourisDessin.y, 1, 1 });
            couleurPipette = GetImageData(pixel); //recupération de la couleur du pixel
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))cGauche = *couleurPipette;
            else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))cDroit = *couleurPipette;
            UnloadImage(pixel);
        }  
        
        // action du pot de peinture
        /*if (outilsSelectionnee == 5 && CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }) && !CheckCollisionPointRec(positionSouris, barreOutils))//outil seau de remplissage
        {
            pixel =  ImageFromImage(historique[indiceHistorique], (Rectangle){ posSourisDessin.x ,posSourisDessin.y, 1, 1 });
            couleurSeau = GetImageData(pixel); //recupération de la couleur a replacer
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                entreDansLhistorique = true;
                //recupération de l'image presente pour ne pas l'écraser dans l'historique
                *couleurRemplacee = historique[indiceHistorique]; 
                //remplace la couleur selectionner par la souris par la couleur dans le click gauche
                ImageColorReplace(couleurRemplacee, *couleurSeau, cGauche);
                textureCouleurRemplacee = LoadTextureFromImage(*couleurRemplacee);
                BeginTextureMode(dessin);
                DrawTexture(textureCouleurRemplacee, 0, 0, WHITE); //dessine la nouvelle texture sur le dessin
                EndTextureMode();
                UnloadTexture(textureCouleurRemplacee);
            }
            else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            {
                entreDansLhistorique = true;
                //recupération de l'image presente pour ne pas l'écraser dans l'historique
                *couleurRemplacee = historique[indiceHistorique]; 
                //remplace la couleur selectionner par la souris par la couleur dans le click droit
                ImageColorReplace(couleurRemplacee, *couleurSeau, cDroit);
                textureCouleurRemplacee = LoadTextureFromImage(*couleurRemplacee);
                BeginTextureMode(dessin);
                DrawTexture(textureCouleurRemplacee, 0, 0, WHITE); //dessine la nouvelle texture sur le dessin
                EndTextureMode();
                UnloadTexture(textureCouleurRemplacee);
            }
            UnloadImage(pixel);
        }*/
        
        //actions de la mollette de la souris
        if (outilsSelectionnee == 1)
        {
            // zoom
            deplacement.zoom += ((float)GetMouseWheelMove()*-0.05f);
            
            if (deplacement.zoom > 3.0f) deplacement.zoom = 3.0f;
            else if (deplacement.zoom < 0.1f) deplacement.zoom = 0.1f;
        }
        else 
        {
            //taille du peinceau
            taillePeinceau += GetMouseWheelMove()*-5;
            if (taillePeinceau < 2) taillePeinceau = 2;
            if (taillePeinceau > 50) taillePeinceau = 50;
        }
        
        //gestion de l'historique
        if ((mouvementPrecedent == GESTURE_DRAG && mouvementPresent != GESTURE_DRAG) || entreDansLhistorique == true)
        {
            if (indiceHistorique < HISTORIQUE - 1 ) // ajout de la texture du dessin à la suite du tableau
            {
                indiceHistorique++;
                historique[indiceHistorique] = GetTextureData(dessin.texture);
                ImageFlipVertical(&historique[indiceHistorique]);
                for (int i = indiceHistorique + 1; i < HISTORIQUE; i++) UnloadImage(historique[i]);
            }
            else 
            { // decalage de chaque image du tableau et ajout de l'image presente à la fin du tableau
                //UnloadImage(historique[1]);
                for (int i = 2; i < HISTORIQUE; i++)
                {
                    historique[i - 1] = historique[i];
                }
                historique[HISTORIQUE - 1] = GetTextureData(dessin.texture);
                ImageFlipVertical(&historique[HISTORIQUE-1]);
            }
            entreDansLhistorique = false;
        }
        
        // gestion du crtl-z W = Z sur le QWERTY
        if (IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_CONTROL))
        {
            if (indiceHistorique != 0 && ctrlZ_Active == false)
            {
                indiceHistorique--;
                textureHistorique = LoadTextureFromImage(historique[indiceHistorique]); 
                BeginTextureMode(dessin);
                DrawTexture(textureHistorique, 0, 0, WHITE);
                EndTextureMode();
                UnloadTexture(textureHistorique);
       
            }
            ctrlZ_Active = true;
        }
        else ctrlZ_Active = false;
        
        //gestion du ctrl-Y
        if (IsKeyDown(KEY_Y) && IsKeyDown(KEY_LEFT_CONTROL))
        {
            if(indiceHistorique != HISTORIQUE - 1 && ctrlY_Active == false)
            {
                indiceHistorique++;
                textureHistorique = LoadTextureFromImage(historique[indiceHistorique]); 
                BeginTextureMode(dessin);
                DrawTexture(textureHistorique, 0, 0, WHITE);
                EndTextureMode();
                UnloadTexture(textureHistorique);
            }
            ctrlY_Active = true;
        }
        else ctrlY_Active = false;
        
        // outils de sauvegarde
        if (outilsSelectionnee == 0)
        {
            if (compteur == 0)
            {
            Image sauvegarde =  GetTextureData(dessin.texture);
            ImageFlipVertical(&sauvegarde);
            ExportImage(sauvegarde, "my_painting.png");
            UnloadImage(sauvegarde);
            messageDeSauvegarde = true;
            }
            else if (compteur > 150)
            {
                outilsSelectionnee = outilAvant;
            }
        }
                
        if(messageDeSauvegarde)
        {
           // montre le un message de sauvegarde pendant 2 secondes
           compteur++;
           if (compteur > 240)
           {
               messageDeSauvegarde =  false;
               compteur = 0;
           }
        }
        
        //recupération des coordonée en fonction du zoom de la zone de dessin
        dessinZoomWidth = widthDessin * deplacement.zoom;
        dessinZoomHeight = heightDessin * deplacement.zoom;
        dessinZoomX = (screenWidth - dessinZoomWidth) / 2 + (screenWidth/2 - deplacement.target.x)*deplacement.zoom;
        dessinZoomY = (screenHeight - dessinZoomHeight) / 2  + (screenHeight/2 - deplacement.target.y)*deplacement.zoom;
        
        //récuperation des coordonnées de la souris en fonction du zoom et du déplacement
        posSourisDessin.x = ((positionSouris.x - (screenWidth - deplacement.zoom * widthDessin) / 2 ) / deplacement.zoom) - (screenWidth/2 - deplacement.target.x);
        posSourisDessin.y = ((positionSouris.y - (screenHeight -  heightDessin * deplacement.zoom) / 2 ) / deplacement.zoom) - (screenHeight/2 - deplacement.target.y);
        
        // valeur tampon
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
                DrawTextureRec(dessin.texture, (Rectangle){ 0, 0, dessin.texture.width, -dessin.texture.height }, (Vector2){ 25, 25 }, WHITE);
            
            EndMode2D();
            
            if (outilsSelectionnee == 1)
            {   // curseur de l'outils deplacement
                DrawTexture(curseurTexture[0], positionSouris.x, positionSouris.y, WHITE);
            }
            
            //dessine la forme du peinceau sous la souris
            if(CheckCollisionPointRec(positionSouris, (Rectangle){ dessinZoomX, dessinZoomY, dessinZoomWidth, dessinZoomHeight }))
            {
                switch (outilsSelectionnee)
                {
                    
                    case 2 : // outils peinceau rond
                        if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) DrawCircleLines(positionSouris.x, positionSouris.y, taillePeinceau, cDroit);
                        else DrawCircleLines(GetMouseX(), GetMouseY(), taillePeinceau * deplacement.zoom, cGauche);
                        break;
                    
                    case 3 : // outils peinceau carré
                        if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) DrawRectangleLines(positionSouris.x - (taillePeinceau / 2 * deplacement.zoom), positionSouris.y - (taillePeinceau / 2 * deplacement.zoom), taillePeinceau * deplacement.zoom, taillePeinceau * deplacement.zoom, cDroit);
                        else DrawRectangleLines(positionSouris.x - (taillePeinceau / 2 * deplacement.zoom), positionSouris.y - (taillePeinceau / 2 * deplacement.zoom), taillePeinceau * deplacement.zoom , taillePeinceau * deplacement.zoom, cGauche);
                        break;
                    
                    case 4 : // outil gomme
                        DrawCircleLines(GetMouseX(), GetMouseY(), taillePeinceau * deplacement.zoom, BLACK);
                        DrawTexture(curseurTexture[1], positionSouris.x + taillePeinceau/2 + 2, positionSouris.y + taillePeinceau/2 + 2, WHITE);
                        break;
                    
                    case 5 : // ourils seau de remplissage
                        DrawTexture(curseurTexture[2], positionSouris.x - 24, positionSouris.y - 24, WHITE);
                        break;
                    
                    case 6 : // outils pipette
                        DrawTexture(curseurTexture[3], positionSouris.x, positionSouris.y - 24, WHITE);
                        break;
                }
            }
            
            if (messageDeSauvegarde == true) // affichage du message de sauvegarde
            {
                DrawRectangle(screenWidth / 2 - 310, screenHeight / 2 - 20, 620, 80, Fade(BLACK, 0.7f));
                DrawText("dessin Sauvegardé!", screenWidth / 2 - 290, screenHeight / 2 - 10, 60, WHITE);
            }
            
            // affichage du message ctrl-z
            if (IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_CONTROL) ) 
            {
                DrawRectangle(0, screenHeight - 80, 250, 80, Fade(BLACK, 0.7f));
                DrawText("CTRL+Z", 20, screenHeight -60, 40, WHITE);
            }
            
            // affichage du message ctrl-y
            if (IsKeyDown(KEY_Y) && IsKeyDown(KEY_LEFT_CONTROL) )
            {
                DrawRectangle(0, screenHeight - 80, 250, 80, Fade(BLACK, 0.7f));
                DrawText("CTRL+Y", 20, screenHeight -60, 40, WHITE);
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
    
    //unload la render texture
    UnloadImage(blankHistorique);
    UnloadImage(historique[indiceHistorique]);
    UnloadImage(pixel);
    
    UnloadImage(couleurRemplacee[0]);
    UnloadTexture(textureCouleurRemplacee);
    
    UnloadRenderTexture(dessin);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}