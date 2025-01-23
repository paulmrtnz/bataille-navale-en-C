#define TAILLE_GRILLE 10

#define NB_BATEAU 5

// définition du type joueur_t, qui représente un joueur
typedef enum
{
    HUMAIN,
    ORDI
} joueur_t;


// définition du type direction_t, qui représente une direction de bateau
typedef enum
{
    VERTICAL,
    HORIZONTAL
} direction_t;


// définition du type coord_t, qui représente une coordonnée
typedef int coord_t;

// définition de la structure celluleB (pour "Cellule de bateau")
typedef struct celluleB
{
    coord_t coord;
    struct celluleB *suiv;
} celluleB;

// définition du type bateau_t, qui est une liste chaînée de celluleB
typedef celluleB *bateau_t;

// définition du type flotte_t, qui représente la flotte d'un joueur
// c'est un tableau de bateaux
typedef bateau_t flotte_t[NB_BATEAU];

// définition du type grille_t, qui représente le plateau de jeu du joueur, où il place ses bateaux
typedef int grille_t[TAILLE_GRILLE*TAILLE_GRILLE];

// tableaux contenant les données pour chaque joueur, accessible avec le type joueur_t 
typedef grille_t Grilles[2];
typedef flotte_t Flottes[2];

/**
 * Initialise une flotte
 * 
 * - f : la flotte à initialiser
*/
void init_flotte(flotte_t f);

// évaluations de position

/**
 * Renvoie la distance entre deux coordonnées
 * 
 * distance() calcule la norme du vecteur de coordonnées (x,y)
 * x et y sont calculés d'après les coordonnées a et b en entrée
 *  x = a / 10 - b / 10
 *  y = a % 10 - b % 10
 * 
 * - a : la coordonnée de la première case 
 * - b : la coordonnée de la seconde case
*/
int distance(coord_t a, coord_t b);

/**
 * Retourne la distance minimum entre un bateau et tous les bateaux de la flotte
 * 
 * distance_min() évalue, pour un bateau, la distance de chacune de ses cases avec celles de chacun des bateaux de la flotte
 * Elle retourne la valeur la plus petite 
 * 
 * - bateau : le bateau dont on souhaite évaluer la distance minimum à la flotte
 * - flotte : la flotte 
*/
int distance_min(bateau_t bateau, flotte_t flotte);

/**
 * Evalue si un bateau sors de la grille même si sa coordonnée de départ est à l'intérieur
 * 
 * hors_grille() prend en entrée la coordonnée, la direction et la taille du bateau 
 * puis compare sa première et sa dernière case pour savoir si elles sont dans 
 * la même ligne ou la même colonne en fonction de la direction.
 * Renvoie 1 si le bateau dépasse de la grille, 0 sinon
 * 
 * - coord : la coordonnée de départ du bateau
 * - dir : la direction du bateau
 * - taille : la taille du bateau
*/
int hors_grille(coord_t coord, direction_t dir, int taille);

// gestion de flotte

/**
 * Crée un bateau avec une coordonnée, une taille et une direction, et le retourne
 * 
 * creer_bateau() prend en entrée une coordonnée de départ, la taille du bateau, et son orientation
 * Cette fonction crée un bateau (qui est une liste chaînée) et le retourne
 * 
 * - coord : la coordonnée de départ du bateau, sur laquelle on l'a placé
 * - taille : la taille du bateau que l'on crée
 * - dir : la direction du bateau: `HORIZONTAL` ou `VERTICAL`
*/
bateau_t creer_bateau(coord_t coord, int taille, direction_t dir);

/**
 * Modifie le bateau en entrée
 * 
 * modifie_bateau() modifie les coordonnées des cases du bateau grâce à la coordonnée et la direction données en entrée
 * 
 * - bateau : le bateau à modifier
 * - coord : la nouvelle coordonnée
 * - dir : la nouvelle direction
*/
void modifier_bateau(bateau_t bateau, coord_t coord, direction_t dir);

/**
 * Place un bateau dans la grille
 * 
 * placer_bateau() place le bateau courant dans la flotte du joueur donné, à la coordonnée donnée
 * Si c'est l'ORDI qui place un bateau, il doit générer un nouveau placement tant qu'il est mauvais, renvoie toujours 1
 * Si c'est l'HUMAIN qui place un bateau, on renvoie 1 si le bateau a été placé, 0 sinon
 * 
 * - flotte : la flotte du joueur
 * - taille : la taille du bateau
 * - bateau_courant : le numéro du bateau que l'on souhaite placer
 * - joueur : le joueur
 * - grille : la grille du joueur
 * - c : la coordonnée à laquelle placer le bateau
 * - d : la direction du bateau
 * - tab_placement : le tableau de placement à modifier
*/
int placer_bateau(flotte_t flotte, int taille, int bateau_courant, joueur_t joueur, grille_t grille, coord_t c, direction_t d, int* tab_placement);

/**
 * Place automatiquement la totalité des bateaux de l'ordi
 * 
 * creer_flotte_ordi() place un a un les bateaux de l'ordi dans sa grille, en appelant placer_bateau()
 * 
 * - F : la flotte de l'ordi
 * - G : la grille de l'ordi
 * - P : le tableau de placement de l'ordi
*/
void creer_flotte_ordi(flotte_t flotte, grille_t grille, int* placement);

// generation d'aléatoire

/**
 * Génère un nombre, 0 ou 1
*/
void generer_direction(direction_t *p_dir);
/**
 * Génère une coordonnée entre 0 et 99
*/
void generer_coord(coord_t *p_coord);
/**
 * Génère un placement constitué d'une coordonnée et une direction
*/
void generer_placement(coord_t *p_coord, direction_t *p_dir);