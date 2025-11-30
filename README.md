<div align="center">
<h1 align="center">Bataille Navale</h1>
<p align="center">
 <b><a href="https://www.linkedin.com/in/paul-martinez-paul/" target="_blank">Paul MARTINEZ</a></b> • <b><a href="https://www.linkedin.com/in/camille-imbert-camille/" target="_blank">Camille Imbert</a></b> • <b><a href="https://www.linkedin.com/in/arnaud-clr-582288261/" target="_blank">Arnaud Cartault-La Rocca</a></b>
<p>
<div align="center" style="display: flex; place-content: center; gap: 1.5em;">
<img src="https://www.insa-centrevaldeloire.fr/themes/custom/efil/logo.svg" height="64">
</div>
</div>
<br/>

Ce dépôt contient le code source d'un jeu de Bataille Navale ("Touché-Coulé") développé en langage C avec la bibliothèque SDL2. Ce projet a été réalisé dans le cadre du cursus de 2ème année à l'INSA Centre Val de Loire.

Le jeu oppose un joueur humain à une intelligence artificielle (IA) sur des grilles de 10x10, avec une flotte composée de 5 navires (Porte-avion, Croiseur, Contre-croiseur, Sous-marin, Torpilleur).

## Auteurs
- **Paul Martinez** : Programmation principale, Interface Graphique
- **Camille Imbert** : Aide à la programmation, Documentation
- **Arnaud Cartault-La Rocca** : Aide à la programmation

## Architecture technique
Le projet a été conçu de manière modulaire pour séparer la logique de jeu, la gestion des données et l'interface graphique. L'architecture repose sur 4 modules principaux :

### 1. Module [`Flotte`](flotte.h) (Gestion des données)
Ce module gère la structure des navires et des flottes.
- Structures de données :
  - Bateaux : Implémentés sous forme de listes chaînées (`struct celluleB`).
  Ce choix permet de gérer les dégâts dynamiquement : lorsqu'une case est touchée, le maillon correspondant est supprimé de la liste. Si la liste devient `NULL`, le bateau est considéré comme "Coulé" .
  - Flotte : Tableau statique contenant les pointeurs vers les têtes de listes des bateaux.
- Algorithme de placement :
  - Les navires ne peuvent pas être adjacents. Pour vérifier cela, nous calculons le carré de la distance euclidienne entre chaque coordonnée du bateau à placer et les bateaux existants. 
  Le placement est valide uniquement si la distance au carré est supérieure à 4 .
<p align="center">
<img width="200" alt="image" src="https://github.com/user-attachments/assets/157f46b8-a128-4572-88c4-eff5a9153cab" />
</p>

### 2. Module [`Jeu`](jeu.h) (Logique métier)
Ce module orchestre le déroulement de la partie via des automates à états finis.
- Grilles : Stockées sous forme de tableaux à une dimension (indices 0 à 99) pour simplifier la gestion mémoire par rapport à des tableaux 2D.
- Boucle de jeu : Gère l'alternance des tours, la vérification des conditions de victoire (flotte vide) et la mise à jour des états (Plouf, Touché, Coulé).

<p align="center">
<img width="400" alt="image" src="https://github.com/user-attachments/assets/aa9302c5-4e40-4366-a5d3-06f06eaf9ba1" style="self-align:center"/>
</p>

### 3. Module [`Ordi`](ordi.h) (Intelligence Artificielle)
Ce module gère l'intelligence contre laquelle le joueur joue. L'IA a été développée selon deux niveaux de difficulté distincts, gérés par une machine à états:
- Niveau 0 (Aléatoire) : L'ordinateur tire sur des coordonnées aléatoires, en mémorisant les tirs précédents pour ne jamais viser deux fois la même case.
- Niveau 1 (Chasse et Ciblage) : L'ordinateur réfléchit de manière séquentielle :
  - Recherche : Tirs aléatoires tant qu'il ne touche rien.
  - Destruction : Lorsqu'un navire est touché, l'IA passe en mode "ciblage" et tire sur les cases adjacentes (Nord, Sud, Est, Ouest) pour déterminer l'orientation du bateau et le couler entièrement .

### 4. Module [`Interface`](interface.h) (Graphisme et I/O)
Ce module gère l'affichage et les interactions utilisateur via la SDL2.
- Rendu dynamique : Utilisation de tilesets pour l'affichage des grilles et des états (eau, touché, coulé).
- Interaction souris : Conversion des coordonnées pixels (clic souris) en index de grille (0-99) via la fonction `recup_coord_clic`.
- Placement visuel : Lors de la phase d'initialisation, une "silhouette" du bateau suit le curseur de la souris pour faciliter le placement.

## Choix d'implémentation spécifiques
- Listes chaînées pour la santé des navires : Contrairement à une simple décrémentation de points de vie, l'utilisation de listes chaînées contenant les coordonnées exactes permet de savoir précisément quelle partie du bateau est intacte.
- Automates (State Machines) : L'utilisation d'automates (représentés par les diagrammes dans le rapport) permet de sécuriser le flux logique du jeu (ex: transition de l'état "aucune info" à "en cours de destruction" pour l'IA) .
<p align="center">
 <img width="500" alt="image" src="https://github.com/user-attachments/assets/ce927a7f-a8f3-4d1b-aa2d-74d55276746b" />
</p>
## Compilation
### Prérequis
- Bibliothèque [SDL](https://www.libsdl.org/)
### Build

#### Windows
```sh
gcc main.c ordi.c interface.c jeu.c flotte.c -o bataille_navale -lmingw32 -lSDL2main -lSDL2 -I C:\SDL2\include\SDL2 -L C:\SDL2\lib
```
#### GNU/Linux
```sh
gcc main.c ordi.c interface.c jeu.c flotte.c -o bataille_navale -lSDL2 -I/usr/include/SDL2
```

## Captures d'écran
<p align="center">
  <img width="500" alt="image" src="https://github.com/user-attachments/assets/8790365c-228f-4726-8e59-39ec155062db" />
  <img width="500" alt="image" src="https://github.com/user-attachments/assets/a7cb0e72-0a43-4577-860f-57704f565aad" />
  <img width="500" alt="image" src="https://github.com/user-attachments/assets/10ccc529-6477-4497-91ad-ea335f43ff94" />
</p>

***
<small><i>Ce projet a été réalisé dans un cadre académique à l'INSA Centre Val de Loire.</i></small>
