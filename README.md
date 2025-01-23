# Bataille Navale
> Paul Martinez, Camille Imbert et Arnaud Cartault-La Rocca

## Introduction
Ce projet est un projet académique. Nous avons souhaité recréer le jeu "Bataille Navale" (ou aussi "Touché-coulé") en C, avec une interface graphique.

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
