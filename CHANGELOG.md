### v0.2.1
* xxx.yyy

#### v0.1.2
* 22 mars 2025
* Ajouts
  * Retrait du menu en mode jeu
    * Les grilles sont toujours centrées (hors scrolling)
  * Capture d'écran si SCREEN_CAPTURE défini
  * Sortie du jeu => "start" est grisé

* Corrections
    * BUG : Rotation => pos = {0,0}
    * BUG : en mode DEBUG, affichage des statss même lorsque la 'grid' est vide
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
