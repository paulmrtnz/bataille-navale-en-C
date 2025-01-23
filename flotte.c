#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "interface.h"
#include "flotte.h"

void init_flotte(flotte_t f)
{
  for (int i = 0; i < NB_BATEAU; i++)
    f[i] = NULL;
}

bateau_t creer_bateau(coord_t coord, int taille, direction_t dir)
{
  bateau_t bateau, temp;
  bateau = NULL;
  for (int i = 0; i < taille; i++)
  {
    temp = malloc(sizeof(celluleB));
    temp->coord = coord + i * dir + TAILLE_GRILLE * i * (1 - dir);
    temp->suiv = bateau;
    bateau = temp;
  }
  // printf("Bateau cree\n");
  return bateau;
}

int distance(coord_t a, coord_t b)
{
  int x = (a / TAILLE_GRILLE - b / TAILLE_GRILLE),
      y = (a % TAILLE_GRILLE - b % TAILLE_GRILLE);
  return x * x + y * y;
}

int distance_min(bateau_t bateau, flotte_t flotte)
{
  int i, dist, dmin = 2 * TAILLE_GRILLE * TAILLE_GRILLE;
  bateau_t pBateauFlotte, pBateau = bateau;
  while (pBateau != NULL)
  {
    i = 0;
    pBateauFlotte = flotte[i];
    while (pBateauFlotte != NULL)
    {
      dist = distance(pBateau->coord, pBateauFlotte->coord);
      if (dist < dmin)
        dmin = dist;
      if (pBateauFlotte->suiv == NULL && i < 4)
        pBateauFlotte = flotte[++i];
      else
        pBateauFlotte = pBateauFlotte->suiv;
    }
    pBateau = pBateau->suiv;
  }
  return dmin;
}

int hors_grille(coord_t coord, direction_t dir, int taille)
{
  int ligne_d = coord / TAILLE_GRILLE,
      ligne_f = (coord + (taille - 1)) / TAILLE_GRILLE,
      col_f = coord + (taille - 1) * 10;
  
  if (dir == HORIZONTAL)
    if (ligne_d != ligne_f)
      return 1;
  if (dir == VERTICAL)
    if (0 > col_f || col_f > 99 )
      return 1;
  return 0;

  /*
  return (
    ((dir == HORIZONTAL) && (coord / 10 != (coord + taille - 1) / 10))
    ||
    ((dir == VERTICAL) && (coord + (taille - 1) * 10 > 99))
    );
  */
}

void modifier_bateau(bateau_t bateau, coord_t coord, direction_t dir)
{
  bateau_t temp = bateau;
  int i = 0;

  while (temp != NULL)
  {
    temp->coord = coord + i * dir + TAILLE_GRILLE * i * (1 - dir);
    temp = temp->suiv;
    i++;
  }
}

int placer_bateau(flotte_t flotte, int taille, int bateau_courant, joueur_t joueur, grille_t grille, coord_t c, direction_t d, int *tab_placement)
{
  coord_t coord = c;
  direction_t dir = d;
  int fait = 0;
  bateau_t bateau;

  if (joueur == ORDI)
    generer_placement(&coord, &dir);

  bateau = creer_bateau(coord, taille, dir);

  while (!fait) // tant que la position du bateau n'est pas correcte, on redemande
  {
    if (distance_min(bateau, flotte) >= 4 && !hors_grille(coord, dir, taille))
    {
      bateau_t temp = bateau;
      flotte[bateau_courant] = bateau;
      // on initialise les coordonnées de chaque élément de la liste chaînée
      int n = 0;
      while (temp != NULL)
      {
        grille[temp->coord] = (bateau_courant + 1) * 10 + n;
        temp = temp->suiv;
        n++;
      }
      tab_placement[bateau_courant] = coord + 100 * (1 - dir);
      fait = 1;
    }
    else
    {
      if (joueur == ORDI)
        generer_placement(&coord, &dir);
      else
      {
        // printf("\nPlacement interdit");
        return 0;
      }
      modifier_bateau(bateau, coord, dir);
    }
  }
  return fait;
}

void creer_flotte_ordi(flotte_t flotte, grille_t grille, int *placement)
{
  int taille[5] = {5, 4, 3, 3, 2};
  int n = 0;
  // if (joueur == ORDI) printf("Placement des bateaux de l'ordi\n");
  while (n < NB_BATEAU)
  {
    if (placer_bateau(flotte, taille[n], n, ORDI, grille, 0, 0, placement))
      n++;
  }
}

void generer_direction(direction_t *p_dir)
{
  *p_dir = rand() % 2; // 0 ou 1
}

void generer_coord(coord_t *p_coord)
{
  *p_coord = rand() % 100; // nombre entre 0 et 99
}

void generer_placement(coord_t *p_coord, direction_t *p_dir)
{
  generer_direction(p_dir);
  generer_coord(p_coord);
}