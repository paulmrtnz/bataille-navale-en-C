#include <stdio.h>
#include <stdlib.h>
#include "flotte.h"
#include "interface.h"
#include "ordi.h"

void init_ordi(ordi_t *ordi)
{
  ordi->derniere_coord_touche = 0;
  ordi->etat = VIDE;
  ordi->direction = -1;
  ordi->sens = 1;
  ordi->dernier_resultat = -1;
}

void reset_ordi(ordi_t *ordi)
{
  ordi->etat = VIDE;
  ordi->direction = -1;
  ordi->sens = 1;
}

void ordi_etablir_direction(coord_t coord, ordi_t *ordi)
{
  if (coord - 1 == ordi->derniere_coord_touche || coord + 1 == ordi->derniere_coord_touche)
    // si la coordonnée que l'on vient de toucher était à droite ou a gauche de la précédente,
    // alors la direction est horizontale
    ordi->direction = HORIZONTAL;
  else if (coord - 10 == ordi->derniere_coord_touche || coord + 10 == ordi->derniere_coord_touche)
    // si la coordonnée que l'on vient de toucher était en haut ou a gauche,
    // alors la direction est verticale
    ordi->direction = VERTICAL;
  ordi->etat = DEUX_TOUCHES;
}

void maj_resultat_ordi(coord_t coord, resultat_t resultat, ordi_t *ordi)
{
  // si on a touché-coulé, on va ensuite chercher à trouver un nouveau bateau sur la grille
  switch (resultat)
  {
  case TOUCHE_COULE:
    // on réinitialise les données de l'ordi
    reset_ordi(ordi);
    break;
  case TOUCHE:
    if (ordi->etat == UNE_TOUCHE && ordi->direction == -1)
      ordi_etablir_direction(coord, ordi);
    // on avait trouvé un bateau et on a touché à nouveau, alors on a trouvé la direction
    else if (ordi->etat == VIDE)
      // on a trouvé un nouveau bateau sur la grille
      ordi->etat = UNE_TOUCHE;
    ordi->derniere_coord_touche = coord;
    break;
  case PLOUF:
    if (ordi->etat == UNE_TOUCHE)
    // on avait trouvé un bateau mais on a tiré dans l'eau
    {
      // on connaissait la direction mais on a fait plouf avant d'avoir coulé le bateau
      if (ordi->direction != -1)
        ordi->sens = -ordi->sens;
    }
    break;
  }
  ordi->dernier_resultat = resultat;
}

// on tire aléatoirement jusqu'à avoir touché
// on évite de tirer là où on a déjà tiré
// une fois qu'on a touché, on essaye de tirer sur une des 4 cases adjacente
// on a donc la direction du bateau, on tire alors le long de cette direction en partant de la dernière case touchée
// si on tire dans l'eau avant d'avoir touché-coulé, on tire de l'autre côté
// si on a un touché-coulé, on tire aléatoirement ailleurs
void attaque_ordi(table_t table, coord_t *p_coord, ordi_t *ordi)
{
  int c,  // coordonée provisoire
      ok; // integral booléen
  switch (ordi->niveau)
  {
  case 0:
    do
    {
      generer_coord(p_coord);
    } while (table[*p_coord] != T_VIDE);
    break;
  case 1:
    if (ordi->etat == VIDE) // on cherche à trouver un bateau, en tirant au hasard;
      do
      {
        generer_coord(&c);
      } while (table[c] != T_VIDE);
    else // on vise un bateau déjà trouvé
    {
      if (ordi->direction == -1) // on ne connait pas la direction dans laquelle viser
      {
        // on cherche à toucher une des quatres cases autour de celle que l'on vient de toucher
        // cas particuliers dans les coins et sur les cotés
        // printf("\nDerniere touchee: %i", c);
        int ct[4] = {1, 10, -1, -10};
        int tt[4] = {2, 2, -1, -1};
        int dirt[4] = {1, 0, 1, 0};
        ok = 0;
        int i = 0;
        // printf("\nRecherche de la direction");
        while (!ok)
        {
          c = ordi->derniere_coord_touche + ct[i];
          // printf("\nTest de la case %i", c);
          if (!hors_grille(ordi->derniere_coord_touche, dirt[i], tt[i]))
          {
            if (table[c] == T_VIDE)
              ok = 1;
            else
              i++;
          }
          else
            i++;
        }
      }
      else // on connait la direction dans laquelle viser
      {
        // on fixe la ligne/colonne et on se déplace dedans
        c = ordi->derniere_coord_touche;
        int ok = 0;
        if (ordi->direction == HORIZONTAL)
          while (!ok)
          {
            c = c + 1 * ordi->sens;
            if (table[c] == T_PLOUF || c / 10 != ordi->derniere_coord_touche / 10)
              ordi->sens = -ordi->sens;
            else if (table[c] == T_VIDE)
              ok = 1;
          }
        else
        {
          while (!ok)
          {
            c = c + 10 * ordi->sens;
            if (table[c] == T_PLOUF || c > 99)
              ordi->sens = -ordi->sens;
            else if (table[c] == T_VIDE)
              ok = 1;
          }
        }
      }
    }
    *p_coord = c;
    break;
  }
}