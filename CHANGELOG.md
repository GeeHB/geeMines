#### v0.2.2
* xxx avril 2025
* Ajouts
  * shared/scrollBar.c + shared/scrollBar.h
    * Ttes les fonctions et constantes relatives à la gestion des barres de défilement

* Corrections
  * Retrait de scroll.png (oubli dans la v0.2.1)
  * Pas besoin de scroll vertical en mode expert::horizontal

#### v0.2.1 - Première version complète (à tester !!!)
* 2 avril 2025
* Ajouts
  * Remplacement des boutons de navigation par des barres de défilement
  * Gestion des scores
    * Chargement / Enregistrement / Incrément
    * Affichage / Intégration au menu principal
  * Message si partie gagnée
  * Réorganisation des sources
    * casioCalcs : ajout des fonctions de rotation
    * Suppression des macros non utilisées

* Corrections
    * Couleur du board
    * BUG : pas d'affichage du msg en cas de victoire
    * BUG : Affichage partiel de la grille en fin de jeu
      * BUG : board_setGameStateEx - Modification que des 'box' visibles
      * BUG : board_setGameStateEx - inversion col & row

#### v0.1.2
* 21 mars 2025
* Ajouts
  * Retrait du menu en mode jeu
    * Les grilles sont toujours centrées (hors scrolling)
  * Capture d'écran si SCREEN_CAPTURE défini
  * Sortie du jeu => "start" est grisé

* Corrections
    * BUG : Rotation => pos = {0,0}
    * BUG : en mode DEBUG, affichage des stats même lorsque la 'grid' est vide
    * BUG : _onStep => affichage en dehors du viewPort possible

#### v0.0.1 - beta 4
* 20 mars 2025
* Corrections
  * Rotation des affichages
    * Bordures 3D : pas de prise en compte de la rotation
    * viewPort : mauvaise matrice de rotation et erreur calcul largeur
    * 'Déplacements' KO en mode horizontal
  * 4 nav bars => 2 scroll bars
    * BUG : pas de rotation des boutons
    * Changement des noms des ressources, variables et constantes (nav -> scroll)
