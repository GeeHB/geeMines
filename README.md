## *geeMines* - Un jeu de *démineur* pour les Casio GRAPH90+E / FX-CG50 et GRAPH35+E ii

### Présentation

`geeMines` est un jeu ...  utilisant `gint`.

Il offre différentes options : affichage de l'ombre de la pièce, démarrage avec un handicap de lignes 'sales', démarrage sur un niveau plus rapide.
Il permet aussi de jouer avec la calculatrice en mode horizontal (position standard) ou en mode vertical.

basé sur :
projet pédagogique : https://www.codeproject.com/Articles/183582/Adapted-WinMine-Source-for-Teaching-Win32-API-Prog
https://github.com/reactos/reactos/tree/master/base/applications/games/winmine
https://minesweeper.online/fr/game/4289190080

### Informations de version

| Dépôt          | https://gitea.planet-casio.com/Jhb/geeMines     |
|----------------|--------------------------------------------------|
| **Date**           | xx mars 2025                                      |
| **Version stable** | **0.2.1** - branche `dev`                             |
| **Dépendances**    | **Casio / gint**                                     |
| **Testé sur**      | *Linux* (Fedora 41) avec `code::blocks` et *Zed*              |
|                | *Casio Graph90+E*  et Casio Grapg35+E ii avec `gint` |

### Utilisation du clavier

#### A partir du menu principal

L'application se présente sous la forme d'onglets associés aux touches de contrôle. Les différents onglets permettent soit de modifier les paramètres de l'application soit de lancer l'application :

|                    Touches                             | Action                                                                                                                                                                                        |
|--------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ![F1](assets/key_F1.png)                                                 | Affichage de l'**écran d'accueil**                                                                                                                                                                |
| ![F2](assets/key_F2.png)                                                 | Choix du **niveau de départ**. Plus le niveau est important et plus les pièces tomberont rapidement mais plus les lignes complètes raporteront de points. Par défaut le jeu commence au niveau 1. |
| ![F3](assets/key_F3.png)                                                 | Choix du **nombre de lignes 'sales'** affichées au démarrage de la partie. Par défaut il n'y a aucune ligne sur l'écran.                                                                          |
| ![F4](assets/key_F4.png)                                                 | Affichage de **l'ombre de la pièce**. En l'absence d'ombre les lignes complètes raporteront plus de points. Les ombres sont affichées par défaut.                                                 |
| ![F5](assets/key_F5.png)                                                 | **Lancement** d'une partie avec les paramètres sélectionnés.                                                                                                                                      |
| ![F6](assets/key_F6.png)                                                 | **Sortie** de l'application.                                                                                                                                                                      |
| ![left](assets/key_left.png) ![right](assets/key_right.png)                                          | Modification de la valeur du paramètre dans les onglets *Level* et *Lines*.                                                                                                                       |
| ![shift](assets/key_shift.png) ![7](assets/key_7.png)                                          | Activation / désactivation de la **capture** (communication avec `fxlink`). Il faut le programme soit compilé en mode *TRACE*.                                                                        |
| ![shift](assets/key_alpha.png) ![7](assets/key_mult.png)                                          | Affichage du tableau des scores.                                                                                                                                                              |

**Attention** L'activation du mode *capture* peut bloquer la calculatrice.

#### Pendant la partie ...

Quel que soit le mode d'affichage choisi, vertical ou horizontal, les touches suivantes fonctionneront à l'identique à tout moment de la partie :

| Touche | Action                                                                                                                                                                                                                                                                                                                                                                                               |
|--------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ![Rotation](assets/key_xot.png) | **Changement du mode d'affichage**. Par défaut l'affichage est en mode horizontal (calculatrice en mode normal), un appui transformera instantanément les affichages du jeu vers le mode vertical. Compte tenu des dimensions de l'écran, en mode vertical les pièces peuvent être plus  larges. Le jeu est assurément plus jouable. Réciproquement suite à un second appui, il retourna au mode normal. |
| ![Pause](assets/key_optn.png) | Pause du jeu (Graph90+E seulement). En mode **pause**, une image d'écran factice est affichée. Lorsque l'on appuie à nouveau sur la touche, le jeu reprend son cours.                                                                                                                                                                                                                                    |
| ![Exit](assets/key_exit.png) | **Sortie** du jeu et retour au menu principal.                                                                                                                                                                                                                                                                                                                                                           |

#### Contrôles en mode horizontal

| Touche | Action                                     |
|--------|--------------------------------------------|
| ![Gauche](assets/key_left.png) | Déplacement de la pièce vers la **gauche**     |
| ![Droite](assets/key_right.png) | Déplacement de la pièce vers la **droite**     |
| ![Bas](assets/key_down.png) | **Descente** de la pièce d'un rang             |
| ![Rotation](assets/key_up.png) | **Rotation** de la pièce                       |
| ![Descente](assets/key_0.png) | Faire **tomber** la pièce le plus bas possible |

#### Contrôles en mode vertical

Les contrôles "suivent" le mouvement de la calculatrice :

| Touche | Action                                     |
|--------|--------------------------------------------|
| ![Gauche](assets/key_down.png) | Déplacement de la pièce vers la **gauche**     |
| ![Droite](assets/key_up.png) | Déplacement de la pièce vers la **droite**     |
| ![Bas](assets/key_right.png) | **Descente** de la pièce d'un rang             |
| ![Rotation](assets/key_left.png) | **Rotation** de la pièce                       |
| ![Descente](assets/key_0.png) | Faire **tomber** la pièce le plus bas possible |

### Copies d'écran

Ecran d'accueil :

![Accueil](assets/welcome.png)

Grilles :

![Débutant - Vert](assets/9x9_vert.png)
![Débutant - Vert](assets/9x9_horz.png)

![Medium - Vert](assets/16x16_horz.png)

![Expert - Vert](assets/expert_vert.png)
