// initialisation

/**
 * Initialise une grille
*/
void init_grille(grille_t grille);

/**
 * Initialise une table
*/
void init_table(table_t table);

/**
 * Initalise le jeu entièrement
 * 
 * init_jeu() appelle les fonctions d'initialisation de tous les modules
 * toutes les variables importantes du jeu sont remises à leur valeur par défaut
 */
void init_jeu(Grilles grilles, Flottes flottes, Tables tables, Placements placements);

// évaluation des flottes

/**
 * Evalue si une flotte est vide
 * Renvoie 1 si elle l'est, 0 sinon
*/
int estVide(flotte_t);

/**
 * Evalue si le jeu est fini, en évaluant les deux flottes par la fonction estVide()
*/
int estFini(Flottes);

// gestion de l'attaque

/**
 * Evalue le résultat de l'attaque d'un joueur
 * 
 * gerer_attaque() prend en entrée une coordonnée et évalue si le joueur a touché ou non
 * Cette fonction regarde la valeur de la grille du joueur adverse à la coordonnée donnée
 * et indique si le joueur a tiré dans l'eau, auquel cas elle renvoie 0
 * Si le joueur a touché, elle renverra 1, et mettra à jour le pointeur sur résultat dans
 * les cas `TOUCHE` et `TOUCHE_COULE`.
 * Cette fonction adapte aussi le placement dans ce dernier cas, pour l'affichage
 * 
 * - grilles : les grilles des deux joueurs
 * - tables : les tables des deux joueurs
 * - flottes : les flottes des deux joueurs
 * - joueur : le joueur a l'origine de l'attaque
 * - x : la coordonnée ciblée par l'attaque
 * - p_resultat : le pointeur sur résultat
 * - placement_adverse : le placement de l'adversaire
*/
int gerer_attaque(Grilles grilles, Tables tables, Flottes flottes, joueur_t joueur, coord_t x, resultat_t *p_resultat, placement_t placement_adverse);

/**
 * Gère l'attaque complète des deux joueurs
 * 
 * attaque() est la fonction principale d'attaque dans l'écosystème du jeu.
 * Elle fait appel à la fonction gérer_attaque() pour évaluer le résultat d'une attaque.
 * Si c'est l'ordi qui attaque, elle fera appel aux fonctions attaque_ordi() et maj_resultat_ordi()
 * pour faire fonctionner l'ordinateur intelligent en fonction du niveau souhaité au lancement.
 * Lorsque l'ordinateur joue, on boucle tant que l'ordi n'a pas tiré dans l'eau.
 * Lorsque que c'est l'humain, on retourne 0 tant qu'il a le droit de jouer, et 1 sinon.
 * La fonction affiche le dernier résultat dans tous les cas.
 * 
 * - joueur : le joueur à l'origine de l'attaque
 * - coord : la coordonnée ciblée par l'attaque
 * - rect : la structure SDL_Rect représentant la grille ciblée
 * - placement_adverse : le placement de l'adversaire
 * 
 * Retourne 1 lorsque le tour est fini (donc que le joueur a tiré dans l'eau), 0 sinon
*/
int attaque(joueur_t joueur, Grilles grilles, Flottes flottes, Tables tables, coord_t coord, SDL_Renderer *moteur, SDL_Texture *texture, SDL_Window *fenetre, SDL_Rect rect, placement_t placement_adverse, ordi_t *ordi);

/**
 * Retire la case touchée de la liste chaînée du bateau 
 * 
 * bateau_touche() supprime une cellule de la liste chaînée du bateau touché à la case touchée
 * et inverse la valeur dans la grille, pour les futurs tirs
 * 
 * - flotte : la flotte du joueur ciblé
 * - grille : la grille du joueur ciblé
 * - x : la coordonnée ciblée
 * - num : le numéro du bateau dans la flotte
*/
void bateau_touche(flotte_t flotte, grille_t grille, coord_t x, int num);