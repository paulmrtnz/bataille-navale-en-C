#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include "interface.h"

void init_placement(placement_t placement)
{
  for (int i = 0; i < NB_BATEAU; i++)
    placement[i] = -999;
}

int souris_dans_element(SDL_Rect elem, int mouseX, int mouseY)
{
  return (elem.x < mouseX && mouseX < elem.x + elem.w && elem.y < mouseY && mouseY < elem.y + elem.h);
}

void recup_coord_clic(int *coord, int mouseX, int mouseY, SDL_Rect grille)
{
  int col = floor((mouseX - grille.x) / TAILLE_CASE_PX);
  int row = floor((mouseY - grille.y) / TAILLE_CASE_PX);
  *coord = row * 10 + col;
}

SDL_Texture *charger_textures(SDL_Renderer *moteur)
{
  SDL_Surface *tileset = SDL_LoadBMP(FICHIER_TEXTURES);
  if (!tileset)
    SDL_Log("Tileset non chargée: %s\n", SDL_GetError());
  SDL_Texture *tileset_texture = SDL_CreateTextureFromSurface(moteur, tileset);

  SDL_FreeSurface(tileset);
  return tileset_texture;
}

SDL_Rect dessiner_grille(SDL_Renderer *moteur, int joueur)
{
  SDL_Rect Rect_dest, Rect_contour;

  // on charge l'image de la grille
  SDL_Surface *image_grille = SDL_LoadBMP(CHEMIN_GRILLE);
  if (!image_grille)
    SDL_Log("Image non chargée: %s\n", SDL_GetError());
  // on dessine les carreaux de la grille
  Rect_dest.w = image_grille->w;
  Rect_dest.h = image_grille->h;
  int x, y = HAUTEUR / 2 - image_grille->h / 2;
  if (joueur == 0)
    x = (LARGEUR / 2) - image_grille->w - 3 * TAILLE_CASE_PX;
  else
    x = (LARGEUR / 2) + 3 * TAILLE_CASE_PX;
  Rect_dest.x = x;
  Rect_dest.y = y;
  SDL_Texture *texture_grille = SDL_CreateTextureFromSurface(moteur, image_grille);
  SDL_RenderCopy(moteur, texture_grille, NULL, &Rect_dest);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(texture_grille);
  SDL_FreeSurface(image_grille);

  Rect_contour.x = x + TAILLE_CASE_PX;
  Rect_contour.y = y + TAILLE_CASE_PX;
  Rect_contour.w = TAILLE_CASE_PX * TAILLE_GRILLE + 2;
  Rect_contour.h = TAILLE_CASE_PX * TAILLE_GRILLE + 2;

  return Rect_contour;
}

void afficher_bateaux(SDL_Renderer *moteur, SDL_Texture *texture, placement_t tableau_placements, SDL_Rect Rect_grille)
{
  int taille[5] = {5, 4, 3, 3, 2}, angle;
  int coule = 0;
  SDL_Rect Rect_destination;
  // on crée et initialise un rectangle sur la première case de la grille, qui va se déplacer
  Rect_destination.x = Rect_grille.x;
  Rect_destination.y = Rect_grille.y;
  Rect_destination.w = TAILLE_CASE_PX;
  Rect_destination.h = TAILLE_CASE_PX;
  // on crée et initialise un rectangle qui va sélectionner la texture depuis le tilset
  SDL_Rect Rect_source;
  Rect_source.h = TAILLE_CASE_PX;
  // on crée des coordonnées relative pour la rotation des textures, situées dans le coin supérieur gauche

  for (int i = 0; i < NB_BATEAU; i++)
  {
    if (tableau_placements[i] > -999)
    {

      // on extrait la direction et la coordonnée depuis la donnée contenue dans le tableau
      int coord = tableau_placements[i] % 100, dir = tableau_placements[i] / 100;
      // printf("\n%i - %i - %i",tableau_placements[i], coord, dir);
      //  on adapte la rotation de la texture en fonction de la direction du bateau
      //  on positionne la destination sur la grille
      Rect_destination.x = Rect_grille.x + abs(coord % 10) * TAILLE_CASE_PX;
      Rect_destination.y = Rect_grille.y + abs(coord / 10) * TAILLE_CASE_PX;
      // on sélectionne le bon bateau sur le tileset de texture
      coule = (tableau_placements[i] < 0);

      Rect_source.x = 0;
      Rect_source.y = i * TAILLE_CASE_PX + coule * TEXTURE_BATEAU_COULE_Y;
      Rect_source.w = taille[i] * TAILLE_CASE_PX;

      // calcul du centre de rotation
      SDL_Point centre = {TAILLE_CASE_PX / 2, TAILLE_CASE_PX / 2};

      Rect_destination.w = taille[i] * TAILLE_CASE_PX;
      Rect_destination.h = TAILLE_CASE_PX;

      angle = dir ? 90 : 0;

      SDL_RenderCopyEx(moteur, texture, &Rect_source, &Rect_destination, angle, &centre, SDL_FLIP_HORIZONTAL);
      SDL_RenderPresent(moteur);
    }
  }
}

void afficher_bateau_sous_souris(SDL_Renderer *moteur, SDL_Texture *texture, SDL_Rect rect_grille, int col, int ligne, int num_bateau, int dir_bateau)
{
  int taille[5] = {5, 4, 3, 3, 2}, angle;
  // on extrait les coordonées de la souris relatives à la grille
  // on convertit les valeurs relativement à la grille,
  // cela rend l'affichage "collant" a la grille

  SDL_Rect Rect_dest;
  Rect_dest.x = rect_grille.x + col * TAILLE_CASE_PX;
  Rect_dest.y = rect_grille.y + ligne * TAILLE_CASE_PX;
  Rect_dest.w = taille[num_bateau] * TAILLE_CASE_PX;
  Rect_dest.h = TAILLE_CASE_PX;

  SDL_Rect Rect_source;
  Rect_source.x = 0;
  Rect_source.y = TEXTURE_BATEAU_FANTOME_Y + num_bateau * TAILLE_CASE_PX;
  Rect_source.w = taille[num_bateau] * TAILLE_CASE_PX;
  Rect_source.h = TAILLE_CASE_PX;

  SDL_Point centre = {TAILLE_CASE_PX / 2, TAILLE_CASE_PX / 2};

  angle = !dir_bateau ? 90 : 0;

  SDL_RenderCopyEx(moteur, texture, &Rect_source, &Rect_dest, angle, &centre, SDL_FLIP_HORIZONTAL);
  SDL_RenderPresent(moteur);
}

void afficher_bouton_direction(SDL_Renderer *moteur, SDL_Rect *rect, int direction)
{
  SDL_Surface *bouton_pivoter_image;
  // choix de l'image à charger
  if (direction)
    bouton_pivoter_image = SDL_LoadBMP(CHEMIN_BOUTON_DIR_H);
  else
    bouton_pivoter_image = SDL_LoadBMP(CHEMIN_BOUTON_DIR_V);

  rect->w = bouton_pivoter_image->w;
  rect->h = bouton_pivoter_image->h;

  SDL_Texture *bouton_pivoter_texture = SDL_CreateTextureFromSurface(moteur, bouton_pivoter_image);
  SDL_RenderCopy(moteur, bouton_pivoter_texture, NULL, rect);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(bouton_pivoter_texture);
  SDL_FreeSurface(bouton_pivoter_image);
}

void rafraichir(SDL_Renderer *moteur, SDL_Texture *texture, int *placement, SDL_Rect rect_grille_humain, SDL_Rect rect_grille_ordi, Tables tables, etat_jeu_t etat_jeu)
{
  // efface la fenetre et affiche le fond
  SDL_RenderClear(moteur);
  remplir_fond(moteur);

  // affiche les elements en fonction de l'état du jeu
  if (etat_jeu != PLACEMENT)
    dessiner_grille(moteur, 1);
  else 
    afficher_instructions_placement(moteur, rect_grille_ordi);

  dessiner_grille(moteur, 0);
  afficher_bateaux(moteur, texture, placement, rect_grille_humain);

  afficher_resultats(moteur, texture, rect_grille_humain, tables[1]);
  afficher_resultats(moteur, texture, rect_grille_ordi, tables[0]);
}

void convertir_coord_relatives(SDL_Rect Rect, int mouseX, int mouseY, int *col, int *ligne)
{
  *col = floor((mouseX - Rect.x) / TAILLE_CASE_PX);
  *ligne = floor((mouseY - Rect.y) / TAILLE_CASE_PX);
}

void dessiner_resultat_attaque(SDL_Renderer *moteur, SDL_Texture *texture, SDL_Rect Rect, int coord, int resultat)
{
  int col = coord % TAILLE_GRILLE, ligne = coord / TAILLE_GRILLE;

  SDL_Rect Rect_dest;
  Rect_dest.x = Rect.x + col * TAILLE_CASE_PX;
  Rect_dest.y = Rect.y + ligne * TAILLE_CASE_PX;
  Rect_dest.w = TAILLE_CASE_PX;
  Rect_dest.h = TAILLE_CASE_PX;

  SDL_Rect Rect_source;
  Rect_source.x = TEXTURE_TIR_X;
  Rect_source.y = !resultat ? TAILLE_CASE_PX : 0;
  Rect_source.w = TAILLE_CASE_PX;
  Rect_source.h = TAILLE_CASE_PX;

  SDL_RenderCopy(moteur, texture, &Rect_source, &Rect_dest);
  SDL_RenderPresent(moteur);
}

void afficher_resultats(SDL_Renderer *moteur, SDL_Texture *texture, SDL_Rect Rect, table_t table)
{
  for (int i = 0; i < TAILLE_GRILLE * TAILLE_GRILLE; i++)
  {
    if (table[i] == T_TOUCHE)
      dessiner_resultat_attaque(moteur, texture, Rect, i, TOUCHE);
    else if (table[i] == T_PLOUF)
      dessiner_resultat_attaque(moteur, texture, Rect, i, PLOUF);
  }
}

void afficher_message_tour(SDL_Renderer *moteur, int joueur)
{
  SDL_Surface *texte_image;
  if (joueur) // ordi
    texte_image = SDL_LoadBMP(CHEMIN_TEXTE_TOUR_O);
  else // humain
    texte_image = SDL_LoadBMP(CHEMIN_TEXTE_TOUR_H);

  SDL_Rect Rect_dest;
  Rect_dest.x = LARGEUR / 2 - texte_image->w / 2;
  Rect_dest.y = 2 * TAILLE_CASE_PX;
  Rect_dest.w = texte_image->w;
  Rect_dest.h = texte_image->h;

  SDL_Texture *texte_texture = SDL_CreateTextureFromSurface(moteur, texte_image);
  SDL_RenderCopy(moteur, texte_texture, NULL, &Rect_dest);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(texte_texture);
  SDL_FreeSurface(texte_image);
}

void afficher_bouton(SDL_Renderer *moteur, SDL_Rect *Bouton, char *chemin)
{
  SDL_Surface *bouton_image = SDL_LoadBMP(chemin);
  Bouton->w = bouton_image->w;
  Bouton->h = bouton_image->h;

  SDL_Texture *bouton_texture = SDL_CreateTextureFromSurface(moteur, bouton_image);
  SDL_RenderCopy(moteur, bouton_texture, NULL, Bouton);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(bouton_texture);
  SDL_FreeSurface(bouton_image);
}

void remplir_fond(SDL_Renderer *moteur)
{
  SDL_Surface *fond_image = SDL_LoadBMP(CHEMIN_FOND);
  SDL_Rect Rect_fond = {0, 0, LARGEUR, HAUTEUR};
  SDL_Texture *fond_texture = SDL_CreateTextureFromSurface(moteur, fond_image);
  SDL_RenderCopy(moteur, fond_texture, NULL, &Rect_fond);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(fond_texture);
  SDL_FreeSurface(fond_image);
}

void afficher_accueil(SDL_Renderer *moteur, SDL_Rect *Bouton_niv_0, SDL_Rect *Bouton_niv_1)
{
  SDL_RenderClear(moteur);
  remplir_fond(moteur);
  SDL_Surface *titre_image = SDL_LoadBMP(CHEMIN_TITRE);
  SDL_Rect Rect_titre;
  Rect_titre.w = titre_image->w;
  Rect_titre.h = titre_image->h;
  Rect_titre.x = LARGEUR / 2 - titre_image->w / 2;
  Rect_titre.y = 96;
  SDL_Texture *titre_texture = SDL_CreateTextureFromSurface(moteur, titre_image);
  SDL_RenderCopy(moteur, titre_texture, NULL, &Rect_titre);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(titre_texture);
  SDL_FreeSurface(titre_image);
  Bouton_niv_0->y = Rect_titre.y + Rect_titre.h + TAILLE_CASE_PX;
  afficher_bouton(moteur, Bouton_niv_0, CHEMIN_BOUTON_NIV_O);
  Bouton_niv_1->y = Bouton_niv_0->y + Bouton_niv_0->h + TAILLE_CASE_PX;
  afficher_bouton(moteur, Bouton_niv_1, CHEMIN_BOUTON_NIV_1);
}

void afficher_instructions_placement(SDL_Renderer *moteur, SDL_Rect Grille_ordi)
{
  SDL_Surface *inst_image = SDL_LoadBMP(CHEMIN_INSTRUCTIONS_PLACEMENT);
  SDL_Texture *inst_texture = SDL_CreateTextureFromSurface(moteur, inst_image);
  SDL_Rect Rect_dest;
  Rect_dest.x = Grille_ordi.x;
  Rect_dest.y = HAUTEUR / 2 - inst_image->h / 2;
  Rect_dest.w = inst_image->w;
  Rect_dest.h = inst_image->h;
  SDL_RenderCopy(moteur, inst_texture, NULL, &Rect_dest);
  SDL_RenderPresent(moteur);
  SDL_DestroyTexture(inst_texture);
  SDL_FreeSurface(inst_image);
}