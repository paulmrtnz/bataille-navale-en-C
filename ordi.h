/**
 * Ce module gère les attaques de l'ordinateur
 */

#define ORDI_H

// définition de l'état de jeu dans lequel se trouve l'ordinateur
typedef enum
{
  VIDE,
  UNE_TOUCHE,
  DEUX_TOUCHES
} ordi_etat;

// définition de la structure de l'ordinateur
typedef struct ordi_t
{
  int niveau;                  // niveau de l'ordi
  int derniere_coord_touche;   // dernière coordonnée sur laquelle on a eu le résultat `TOUCHE`
  ordi_etat etat;              // état de jeu de l'ordi
  direction_t direction;       // direction dans laquelle tirer si on la connait, NULL sinon
  int sens;                    // signe de la direction dans laquelle tirer
  resultat_t dernier_resultat; // dernier résultat obtenu en tirant
} ordi_t;

/**
 * Initialise l'ordinateur
*/
void init_ordi(ordi_t *);

/**
 * Réinitialise l'ordinateur
*/
void reset_ordi(ordi_t *);

/**
 * Etablit la direction si on ne la cherchait et que l'on a touché
*/
void ordi_etablir_direction(coord_t coord, ordi_t *ordi);

/**
 * Met à jour les résultats de l'ordi en fonction des derniers résultats
 * 
 * maj_resultat_ordi() prend en entrée le dernier résultat et l'évalue pour
 * mettre à jour l'état de l'ordinateur.
 * 
 * - coord : la coordonnée visée précédemment
 * - resultat : le dernier résultat
*/
void maj_resultat_ordi(coord_t coord, resultat_t resultat, ordi_t *ordi);

/**
 * Gère l'attaque de l'ordinateur
 * 
 * attaque_ordi() évalue les meilleures possibilités de tir en fonction des résultats
 * précédents, et génère une coordonnée adaptée
 * 
 * - table : la table d'attaque de l'ordinateur
 * - p_coord : le pointeur sur coordonnée pour la visée
*/
void attaque_ordi(table_t table, coord_t *p_coord, ordi_t* ordi);