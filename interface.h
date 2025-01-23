#include <SDL.h>

#define TAILLE_GRILLE 10

#define NB_BATEAU 5

#define LARGEUR 1280 // largeur de la fenêtre de jeu
#define HAUTEUR 720  // hauteur de la fenêtre de jeu

#define TAILLE_CASE_PX 32 // taille d'une case en pixels

// chemin d'acces du fichier de texture
#define FICHIER_TEXTURES "ressource/texture.bmp"

// définition des positions des textures sur le tileset
#define TEXTURE_BATEAU_STD_Y 0
#define TEXTURE_BATEAU_FANTOME_Y 160
#define TEXTURE_BATEAU_COULE_Y 320
#define TEXTURE_TIR_X 160
#define TEXTURE_TAILLES_X = 160
#define TEXTURE_TAILLES_Y = 64

// définition des chemins des fichiers de textures
#define CHEMIN_BOUTON_DIR_H "ressource/bouton_dir_h.bmp"
#define CHEMIN_BOUTON_DIR_V "ressource/bouton_dir_v.bmp"
#define CHEMIN_BOUTON_REJOUER "ressource/bouton_retour.bmp"
#define CHEMIN_BOUTON_NIV_O "ressource/bouton_niv_0.bmp"
#define CHEMIN_BOUTON_NIV_1 "ressource/bouton_niv_1.bmp"
#define CHEMIN_TEXTE_TOUR_O "ressource/tour_ordi.bmp"
#define CHEMIN_TEXTE_TOUR_H "ressource/tour_humain.bmp"
#define CHEMIN_TITRE "ressource/titre.bmp"
#define CHEMIN_INSTRUCTIONS_PLACEMENT "ressource/texte_placement.bmp"
#define CHEMIN_FOND "ressource/fond.bmp"
#define CHEMIN_GRILLE "ressource/grille.bmp"
#define CHEMIN_ICONE "ressource/icone.bmp"

typedef enum
{
    T_VIDE,
    T_PLOUF,
    T_TOUCHE
} etat_table_t;

typedef etat_table_t table_t[100];
typedef table_t Tables[2];

typedef enum
{
    PLOUF,
    TOUCHE,
    TOUCHE_COULE
} resultat_t;

#define VAL_PLOUF 999

typedef enum
{
    ACCUEIL,
    PLACEMENT,
    BATAILLE,
    FIN
} etat_jeu_t;

// tableau contenant les coordonnées et orientations des bateaux (affichage uniquement)
typedef int placement_t[NB_BATEAU];
typedef placement_t Placements[2];

/**
 * Initialise un tableau de placement
 */
void init_placement(placement_t placement);

// affichage principal

/**
 * Charge les textures depuis la tileset, dont le chemin est spécifié dans la macro-définition `FICHIER_TEXTURES`
 *
 * Retourne la texture chargée en mémoire
 */
SDL_Texture *charger_textures(SDL_Renderer *moteur);

/**
 * Rafraichit l'affichage à l'écran
 *
 */
void rafraichir(SDL_Renderer *moteur, SDL_Texture *texture, int *placement, SDL_Rect rect_grille_humain, SDL_Rect rect_grille_ordi, Tables tables, etat_jeu_t etat_jeu);

/**
 * Affiche l'image de fond
 */
void remplir_fond(SDL_Renderer *moteur);

/**
 * Affiche la page d'accueil du jeu
 *
 * afficher_accueil() affiche le titre du jeu, ainsi que deux bouton pour jouer ou pour lire les règles
 *
 * - Bouton_jouer : pointeur sur la structure SDL_Rect du bouton "JOUER"
 * - Bouton_regles : pointeur sur la structure SDL_Rect du bouton "REGLES"
 */
void afficher_accueil(SDL_Renderer *moteur, SDL_Rect *Bouton_jouer, SDL_Rect *Bouton_regles);

// gestion des entrées de la souris

/**
 * Vérifie si la souris se trouve dans un SDL_Rect
 *
 * souris_dans_element() récupère un élement SDL_Rect ainsi que les coordonnées X et Y de la souris (int)
 * elle retourne 1 si la souris se trouve dans cet élement, 0 sinon.
 *
 * - elem : l'élément à controler
 * - mouseX : la coordonnée en X de la souris
 * - mouseY : la coordonnée en Y de la souris
 */
int souris_dans_element(SDL_Rect elem, int mouseX, int mouseY);

/**
 * Traduit les coordonnées de la souris en coordonnées d'une grille
 *
 * recup_coord_clic() récupère les coordonnées du clic de la souris (X et Y) dans un élement (SDL_Rect)
 * et les traduit relativement à la grille de jeu, à une dimension, entre 0 et 99 (int)
 * elle modifie la valeur par pointeur passée en premier argument
 *
 * - coord : le pointeur sur la coordonnée que l'on souhaite modifier
 * - mouseX : la coordonnée en X de la souris
 * - mouseY : la coordonnée en Y de la souris
 * - grille : la grille dans laquelle on souhaite convertir les coordonnées
 */
void recup_coord_clic(int *coord, int mouseX, int mouseY, SDL_Rect grille);

/**
 * Convertit les coordonnées de la souris en coordonnées de la grille en temps réel
 *
 * - mouseX : la coordonnée en X de la souris
 * - mouseY : la coordonnée en Y de la souris
 * - col : le numéro de colonne
 * - ligne : le numéro de ligne
 */
void convertir_coord_relatives(SDL_Rect Rect, int mouseX, int mouseY, int *col, int *ligne);

// affichage d'éléments statiques

/**
 * Affiche la grille du joueur passé en second argument et renvoie un SDL_Rect
 *
 * dessiner_grille() charge la texture de grille et la place sur la zone d'affichage en fonction du joueur.
 * Elle retourne un SDL_Rect qui représente la zone remplie par la grille
 *
 * - joueur : le joueur dont on souhaite afficher la grille
 */
SDL_Rect dessiner_grille(SDL_Renderer *moteur, int joueur);

// affichage d'éléments dynamiques

/**
 * Affiche les bateaux sur la grille d'un joueur
 *
 * afficher_bateaux() affiche les bateaux sur la grille dont le contour est passé en argument,
 * les positions des bateaux sont également passées en argument
 *
 * - tableau_placements : le tableau contenant les positions des bateaux
 * - Rect_grille : le SDL_Rect représentant la grille à remplir
 */
void afficher_bateaux(SDL_Renderer *moteur, SDL_Texture *texture, placement_t tableau_placements, SDL_Rect Rect_grille);

/**
 * Affiche le bateau sous la souris pendant la période de placement des bateaux
 *
 * - rect_grille : la structure SDL_Rect de la grille du joueur
 * - col : la colonne dans laquelle se situe la souris
 * - ligne : la ligne dans laquelle se situe la souris
 * - num_bateau : le numéro du bateau que l'on est en train de placer
 * - dir_bateau : la direction courante du bateau
 */
void afficher_bateau_sous_souris(SDL_Renderer *moteur, SDL_Texture *texture, SDL_Rect rect_grille, int col, int ligne, int num_bateau, int dir_bateau);

/**
 * Affiche un résultat dans une grille
 *
 * dessiner_resultat_attaque() affiche le resultat dans la case a la coordonée donnée, sur la grille donnée.
 *
 * - Rect : la structure SDL_Rect de la grille
 * - coord : la coordonée sur la grille
 * - resultat : le resultat à afficher
 */
void dessiner_resultat_attaque(SDL_Renderer *moteur, SDL_Texture *texture, SDL_Rect Rect, int coord, int resultat);

/**
 * Affiche le resultat des tirs effectués sur une grille
 *
 * afficher_resultats() récupère les tirs effectués par le paramètre table
 * puis les affiche dans la grille souhaitée grâce à la fonction dessiner_resultat_attaque()
 *
 * - Rect : la structure SDL_Rect de la grille
 * - table : le tableau de resultats d'attaque
 */
void afficher_resultats(SDL_Renderer *moteur, SDL_Texture *texture, SDL_Rect Rect, table_t table);

/**
 * Affiche quel joueur doit jouer son tour
 *
 * - joueur : le joueur dont c'est le tour
 */
void afficher_message_tour(SDL_Renderer *moteur, int joueur);

/**
 * Affiche le texte d'instructions lors du placement
 *
 * - Grille_ordi : la grille de l'ordi à la place de laquelle le texte s'affiche
 */
void afficher_instructions_placement(SDL_Renderer *moteur, SDL_Rect Grille_ordi);

// affichage de boutons

/**
 * Affiche le bouton passé en paramètre
 *
 * afficher_bouton() récupère un SDL_Rect qui contient la position et la taille du bouton,
 * ainsi que le chemin d'accès au fichier de texture du bouton
 *
 * - Bouton : la structure SDL_Rect du bouton
 * - chemin : le chemin d'accès de la texture
 */
void afficher_bouton(SDL_Renderer *moteur, SDL_Rect *Bouton, char *chemin);

/**
 * Affiche le bouton de changement de direction lors du placement
 *
 * afficher_bouton_direction() affiche le bouton de direction dont la position est passée en paramètre,
 * et adapte la texture à la direction courante que l'utilisateur a sélectionné.
 *
 * - rect : la structure SDL_Rect contenant la position et la taille du bouton
 * - direction : la direction courante
 */
void afficher_bouton_direction(SDL_Renderer *moteur, SDL_Rect *rect, int direction);