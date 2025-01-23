#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <math.h>
#include "interface.h"
#include "flotte.h"
#include "ordi.h"
#include "jeu.h"

/**   Compilation:
 *
 * 		Windows: 	gcc main.c interface.c flotte.c ordi.c jeu.c -o bataille_navale -lmingw32 -lSDL2main -lSDL2 -I C:\SDL2\include\SDL2 -L C:\SDL2\lib
 * 		GNU/Linux:	gcc main.c interface.c flotte.c ordi.c jeu.c -o bataille_navale -lSDL2 -I/usr/include/SDL2
 */

const int tailles_bateaux[5] = {5, 4, 3, 3, 2};

int main(int argc, char *argv[])
{
	// initialiser le générateur aléatoire
	srand(time(0));

	//initialiser la librairie SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Impossible d'initialiser SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	SDL_Window *fenetre = SDL_CreateWindow("Bataille navale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR, HAUTEUR, 0);
	SDL_Renderer *moteur = SDL_CreateRenderer(fenetre, -1, 0);
	if (!fenetre || !moteur)
	{ /* contrôle de validité */
		SDL_Log("Impossible de créer la fenêtre : %s.", SDL_GetError());
		return 1;
	}
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles");
	SDL_Surface * icone_surface = SDL_LoadBMP(CHEMIN_ICONE);
	SDL_SetWindowIcon(fenetre, icone_surface);
	SDL_FreeSurface(icone_surface);

	//charger le fichier de textures
	SDL_Texture *texture = charger_textures(moteur);

	//créer les deux zones de grilles
	SDL_Rect Rect_grille_humain = dessiner_grille(moteur, HUMAIN);
	SDL_Rect Rect_grille_ordi = dessiner_grille(moteur, ORDI);

	//initialiser le jeu de bataille navale
	Grilles grilles;
	Flottes flottes;
	Tables tables;
	Placements placements;
	init_jeu(grilles, flottes, tables, placements);

	joueur_t joueur = HUMAIN;

	//initialiser l'ordi
	ordi_t ordi;
	init_ordi(&ordi);

	//initialiser l'état d'affichage dans le jeu
	etat_jeu_t etat_jeu = ACCUEIL;

	//initialiser variables pour gérer le placement des bateaux
	int bateau_courant = 0;
	int direction_bateau_courant = HORIZONTAL;
	int coord_souris_courante = 0;

	//créer les différents boutons cliquables
	SDL_Rect Bouton_pivoter;
	Bouton_pivoter.x = Rect_grille_humain.x + 4 * TAILLE_CASE_PX;
	Bouton_pivoter.y = Rect_grille_humain.y + 11 * TAILLE_CASE_PX;

	SDL_Rect Bouton_niv_0;
	Bouton_niv_0.x = LARGEUR / 2 - 256 / 2;
	Bouton_niv_0.y = HAUTEUR - 384;

	SDL_Rect Bouton_niv_1;
	Bouton_niv_1.x = LARGEUR / 2 - 256 / 2;
	Bouton_niv_1.y = HAUTEUR - 256;

	SDL_Rect Bouton_rejouer;
	Bouton_rejouer.x = LARGEUR / 2 - 260 / 2;
	Bouton_rejouer.y = HAUTEUR - 128;

	//afficher la page d'accueil du jeu
	afficher_accueil(moteur, &Bouton_niv_0, &Bouton_niv_1);

	SDL_Event event;
	int quit = 0;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		int mouseX, mouseY;
		Uint8 etat_souris = SDL_GetMouseState(&mouseX, &mouseY);
		if (etat_jeu == BATAILLE)
		{
			if (joueur == ORDI)
			{
				if (attaque(joueur, grilles, flottes, tables, 0, moteur, texture, fenetre, Rect_grille_humain, placements[HUMAIN], &ordi))
				{
					/**
					 * L'ordi a attaqué
					*/
					joueur = 1 - joueur;
					rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables, etat_jeu);
					afficher_message_tour(moteur, joueur);
				}
			}
			if (estFini(flottes))
			{
				/**
				 * Une des flottes est vide, on arrête le jeu
				*/
				etat_jeu = FIN;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Bataille Navale", "Le jeu est fini", fenetre);
				afficher_bouton(moteur, &Bouton_rejouer, CHEMIN_BOUTON_REJOUER);
				afficher_bateaux(moteur, texture, placements[ORDI], Rect_grille_ordi);
			}
		}
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (etat_jeu == ACCUEIL)
			{
				if (souris_dans_element(Bouton_niv_0, mouseX, mouseY) || souris_dans_element(Bouton_niv_1, mouseX, mouseY))
				{
					/**
					 * On a appuyé sur le bouton jouer, on évalue le niveau et on lance la partie
					*/
					if(souris_dans_element(Bouton_niv_0, mouseX, mouseY))
						ordi.niveau = 0;
					else ordi.niveau = 1;
					etat_jeu = PLACEMENT;
					rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables, etat_jeu);
					afficher_bouton_direction(moteur, &Bouton_pivoter, direction_bateau_courant);
					creer_flotte_ordi(flottes[ORDI], grilles[ORDI], placements[ORDI]);
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Bataille navale", "Placez vos bateaux sur la grille", fenetre);
				}
			}
			else if (etat_jeu == PLACEMENT)
			{
				if (souris_dans_element(Rect_grille_humain, mouseX, mouseY))
				{
					/**
					 * On a placé un bateau
					*/
					coord_t coord;
					recup_coord_clic(&coord, mouseX, mouseY, Rect_grille_humain);
					if (!placer_bateau(flottes[HUMAIN], tailles_bateaux[bateau_courant], bateau_courant, HUMAIN, grilles[HUMAIN], coord, direction_bateau_courant, placements[HUMAIN]))
						/**
					 	* Le bateau a bien été placé
						*/
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Bataille navale", "Placement interdit", fenetre);
					else
					{
						/**
						 * Le bateau n'a pas été placé
						*/
						bateau_courant++;
						direction_bateau_courant = HORIZONTAL;
						rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables, etat_jeu);
						afficher_bouton_direction(moteur, &Bouton_pivoter, direction_bateau_courant);
						if (bateau_courant == 5)
						{
							/**
							 * On a fini de placer les bateaux
							*/
							etat_jeu = BATAILLE;
							rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables, etat_jeu);
							afficher_message_tour(moteur, joueur);
						}
					}
					afficher_bateaux(moteur, texture, placements[HUMAIN], Rect_grille_humain);
				}
				if (souris_dans_element(Bouton_pivoter, mouseX, mouseY))
				{
					/**
					 * On pivote le bateau de placement
					*/
					rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables,etat_jeu);
					direction_bateau_courant = 1 - direction_bateau_courant;
					afficher_bouton_direction(moteur, &Bouton_pivoter, direction_bateau_courant);
					SDL_RenderPresent(moteur);
				}
			}
			else if (etat_jeu == BATAILLE && joueur == HUMAIN)
			{
				if (souris_dans_element(Rect_grille_ordi, mouseX, mouseY))
				{
					/**
					 * On a tiré
					*/
					int col, ligne;
					convertir_coord_relatives(Rect_grille_ordi, mouseX, mouseY, &col, &ligne);
					int c = col + ligne * TAILLE_GRILLE;
					if (attaque(joueur, grilles, flottes, tables, c, moteur, texture, fenetre, Rect_grille_ordi, placements[ORDI], NULL))
					{
						joueur = 1 - joueur;
						rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables, etat_jeu);
						afficher_message_tour(moteur, joueur);
					}
				}
			}
			else if (etat_jeu == FIN)
			{
				if (souris_dans_element(Bouton_rejouer, mouseX, mouseY))
				{
					/**
					 * On a cliqué sur le bouton retour au menu, on réinitialise le jeu
					*/
					etat_jeu = ACCUEIL;
					init_jeu(grilles, flottes, tables, placements);
					afficher_accueil(moteur, &Bouton_niv_0, &Bouton_niv_1);
					bateau_courant = 0;
					direction_bateau_courant = HORIZONTAL;
					coord_souris_courante = 0;
				}
			}
			break;
		case SDL_MOUSEMOTION:
			if (etat_jeu == PLACEMENT && souris_dans_element(Rect_grille_humain, mouseX, mouseY))
			{
				/**
				 * Gestion de l'affichage du bateau de placement
				*/
				int col, ligne;
				convertir_coord_relatives(Rect_grille_humain, mouseX, mouseY, &col, &ligne);
				if (col + ligne * TAILLE_GRILLE != coord_souris_courante)
				{
					rafraichir(moteur, texture, placements[HUMAIN], Rect_grille_humain, Rect_grille_ordi, tables, etat_jeu);
					afficher_bateau_sous_souris(moteur, texture, Rect_grille_humain, col, ligne, bateau_courant, direction_bateau_courant);
					afficher_bouton_direction(moteur, &Bouton_pivoter, direction_bateau_courant);
					coord_souris_courante = col + ligne * 10;
				}
			}
			break;
		case SDL_KEYDOWN:
			if (etat_jeu == PLACEMENT && event.key.keysym.sym == SDLK_SPACE)
			{
				/**
				 * On a pivoté depuis le clavier
				*/
				direction_bateau_courant = 1 - direction_bateau_courant;
				afficher_bouton_direction(moteur, &Bouton_pivoter, direction_bateau_courant);
				SDL_RenderPresent(moteur);
			}
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(moteur);
	SDL_DestroyWindow(fenetre);

	return 0;
}
