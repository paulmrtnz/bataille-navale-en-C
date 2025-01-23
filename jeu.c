#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <SDL.h>
#include "flotte.h"
#include "interface.h"
#include "ordi.h"
#include "jeu.h"

void init_grille(grille_t grille)
{
  for (int i = 0; i < TAILLE_GRILLE * TAILLE_GRILLE; i++)
    grille[i] = 0;
}

void init_table(table_t table)
{
  for (int i = 0; i < TAILLE_GRILLE * TAILLE_GRILLE; i++)
    table[i] = T_VIDE;
}

int estVide(flotte_t flotte)
{
  for (int i = 0; i < NB_BATEAU; i++)
    if (flotte[i] != NULL)
      return 0;
  return 1;
}

int estFini(Flottes flottes)
{
  joueur_t j;
  for (j = 0; j <= 1; j++)
    if (estVide(flottes[j]))
      return 1;
  return 0;
}

void bateau_touche(flotte_t flotte, grille_t grille, coord_t x, int num)
{
  celluleB *p0 = flotte[num];
  celluleB *p1 = p0->suiv;
  if (p0->coord == x)
  {
    flotte[num] = p0->suiv;
    free(p0);
  }
  else
  {
    while (p1->coord != x && p1 != NULL)
    {
      p0 = p1;
      p1 = p1->suiv;
    }
    p0->suiv = p1->suiv;
    free(p1);
  }
  grille[x] = -grille[x];
}

int gerer_attaque(Grilles grilles, Tables tables, Flottes flottes, joueur_t joueur, coord_t x, resultat_t *p_resultat, placement_t placement_adverse)
{
  if (grilles[1 - joueur][x] <= 0) // on a tiré dans l'eau
  {
    tables[joueur][x] = T_PLOUF;
    *p_resultat = PLOUF;
    return 0; // on renvoie 0 si on a fait plouf
  }
  else
  {
    int num_bateau = (grilles[1 - joueur][x]) / 10 - 1;
    assert(num_bateau >= 0 && num_bateau <= 4);
    tables[joueur][x] = T_TOUCHE;
    bateau_touche(flottes[1 - joueur], grilles[1 - joueur], x, num_bateau);

    if (flottes[1 - joueur][num_bateau] == NULL)
    {
      *p_resultat = TOUCHE_COULE;
      placement_adverse[num_bateau] = -placement_adverse[num_bateau];
    }
    else
      *p_resultat = TOUCHE;

    return 1; // on renvoie 1 si on a touché
  }
}

int attaque(joueur_t joueur, Grilles grilles, Flottes flottes, Tables tables, coord_t coord, SDL_Renderer *moteur, SDL_Texture *texture, SDL_Window *fenetre, SDL_Rect rect, placement_t placement_adverse, ordi_t *ordi)
{
  int tour = 1, etat = 1;
  resultat_t resultat;
  coord_t x = coord;
  if (joueur == ORDI)
  {
    do
    {
      // délai de réflexion artificiel
      SDL_Delay(1000);
      attaque_ordi(tables[ORDI], &x, ordi);
      tour = gerer_attaque(grilles, tables, flottes, joueur, x, &resultat, placement_adverse);
      // si l'ordi touche, alors tour = 1, donc c'est encore son tour, sinon (tour = 0) on sort
      dessiner_resultat_attaque(moteur, texture, rect, x, resultat);
      maj_resultat_ordi(x, resultat, ordi);
    } while (tour && !estFini(flottes)); // si une des flottes est vide, alors on arrete le jeu
  }
  else if (joueur == HUMAIN) // c'est l'humain qui a joué depuis l'évenement de la souris, la coordonnée est connue d'avance
  {
    if (tables[joueur][x] != T_VIDE)
    {
      etat = 0; // on donne a l'humain le droit de jouer une seconde fois
    }
    else
    {
      // si humain touche, alors etat = 0, donc il a le droit de jouer à nouveau, sinon etat = 1, donc on change de tour
      etat = !gerer_attaque(grilles, tables, flottes, joueur, x, &resultat, placement_adverse);
      // on affiche le resultat a la fin du tour de l'humain
      dessiner_resultat_attaque(moteur, texture, rect, x, resultat);
      // on affiche si c'est touché-coulé
      if (resultat == TOUCHE_COULE)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Bataille navale", "Touché coulé", fenetre);
    }
  }
  // on renvoie 1 (qui confirme le changement de tour) lorsque le joueur a fait plouf
  return etat;
}

void init_jeu(Grilles grilles, Flottes flottes, Tables tables, Placements placements)
{
  init_grille(grilles[HUMAIN]);
  init_grille(grilles[ORDI]);
  init_flotte(flottes[HUMAIN]);
  init_flotte(flottes[ORDI]);
  init_table(tables[HUMAIN]);
  init_table(tables[ORDI]);
  init_placement(placements[HUMAIN]);
  init_placement(placements[ORDI]);
}