# TP7Paint
 Projet C MLOD paint
-------------------------------------
 Fonctionne sur NotePad++ de Raylib
-------------------------------------Description------------------------------------

Le projet choisit est la rpogrammation d'un logiciel type Paint à partir de la librry Raylib. C'est a dire un logiciel de dessin basique comprenant :
   * le deplacement de la zonne de dessin
   * le zoom et de-zoom de la zone de dessin
   * l'outil peinceau rond permettant de dessiner avec un bout rond
   * l'outil peinceai carré permettant de dessiner avec un bout carré
   * l'outils gomme permettnt d'effacer le dessin
   * la pipette permettant d'échantillonner une couleur directement sur le dessin
   * l'outil pot de peinture permettant de remplacer une couleur sur le dessin par une autre
   * une palette de couleurs
   * un afficheur de couleur pour le clique gauche et droit
   * un outil de sauvegarde de l'image
   * la capaciter de changer la taille des peinceaux et de la gomme
   * un historique des actions effectuées

-----------------------------------Description des éléments graphiques-------------------------------

Les differents outils sont afficher sur la barre en haut de l'écran.

Sur la partie gauche de cette barre on retrouve les différents outils représenté de gauche à droite par les icons ci-dessous:
   * la disquette pour enregistrer l'image
   * la souris avec les quatre fleches pour l'outils de déplacement
   * le peinceau basique pour l'outil peinceau rond
   * le peinceau plat pour l'outil peinceau carré
   * la gomme pour l'outil gomme
   * le pot de peinture pour l'outil pot de peinture
   * la pipette pour l'outil pipette

Pour en selectionner un, il suffit de cliquer dans l'encadré où se trouve l'icon correspondante.
Lorsque l'un de ces outil est selectionné, l'encadré de l'outil devient plus épais, si votre souris passe dessus l'encadrer deviendra plus foncé.
Le curseur afficher sera aussi different en fonction de l'outil selectionné:
   * le curseur est la même icon que celui représentant l'outils deplacement pour celui-ci
   * pour l'outils peinceau rond le curseur est un cercle de la même couleur que la couleur du clique gauche et de la même taille que le peinceau
   * pour l'outi peinceau carré le curseur est un carré de la même couleur que la couleur du clique gauche et de la même taille que le peinceau
   * pour la gomme le curseur est un cercle noir de la même taille que la taille de peinceau
   * pour le pot de peinture le curseur et un pot de peinture
   * pour la pipette le curseur est une pipette

En continuant sur la droite, on trouve deux grands carrés de couleur,
le carré le plus à gauche affiche la couleur contenue dans le clique gauche et l'autre la couleur contenue dans le clique droit.
Ensuite on trouve deux ligne de petit carré coloré. Ce sont les couleurs componsant la palette du paint. Lorsque la souris passe sur l'un de ces carrés, ils se grisent.
Lorsque la souris clique gauche dessus, l'afficheur de couleur gauche prend la même couleur que celle selectionnée.
Lorsque la souris clique droit dessus, l'afficher de couleur droit prend la même couleur que celle selectionnée.

La grand zone blanche est la zone de dessin, les curseurs spécifiques n'apparaissent que sur cette zone.

------------------------------------Comment ça marche ?----------------------------------

Choisir un outils
   * cliquer dans l'encadré de l'outils à choisir

choisir une couleur
   - par la palette
	* clique gauche sur une couleur de la palette pour selectionner la couleur du clique gauche
	* clique droit sur une couleur de la palette pour selectionner la couleur du clique droit
   - par la pipette
	*selectionner l'outil pipette
	* clique gauche sur une couleur de la zone de dessin de la palette pour selectionner la couleur du clique gauche
	* clique droit sur une couleur de la zone de dessin de la palette pour selectionner la couleur du clique droit

La sauvegarder
   * selectionner l'outil sauvegarder, un texte s'affichera sur vottre ecrant pendant 30 secondes
   * l'image se trouvera dans le même dossier que le .c du paint sous le nom MyDrawing.png
   * l'outils sera automatique remis sur l'outil selectionné avant la sauvegarde

Dessiner/gommer
   * selectionner l'un des outils peinceau rond ou carré en encore la gomme
   * maintenir le click dorit ou gauche enfoncer et déplacer la souris sur la zone de dessin

Changer la taille du peinceau
   * selectionner l'un des outils peinceau rond ou carré en encore la gomme
   * faite rouler la molette de votre souris vers l'arrière pour agrandir la taille du peinceau
   * faite rouler la molette de votre souris vers l'avant pour réduire la taille du peinceau

Zoomer/ De-zoomer la zone de dessin
   *selectionner l'outils deplacement
   * faite rouler la molette de votre souris vers l'arrière pour zoomer la zone de dessin
   * faite rouler la molette de votre souris vers l'avant pour de-zoomer la zone de dessin

Deplacer la zone de dessin
   * selectionner l'outils deplacement
   * maintenir le clique gauche de la souris et déplacer celle-ci pour deplacer la zone de dessin

Remplacer une couleur par une autre
   * selectionner l'outils pot de peinture
   * clique gauche sur une couleur pour la remplacer par la couleur du clique gauche
   * clique droit sur une couleur pour la remplacer par la couleur du clique droit

Annuler/Rétablir une action
   * Control-gauche + Z pour annuler une action
   * Comtrol-gauche + Y pour rétablir une action


