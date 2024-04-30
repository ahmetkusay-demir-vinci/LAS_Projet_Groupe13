#ifndef _JEU_H
#define _JEU_H

#include <stdbool.h>
#include "structure.h"
#include "utils_v1.h"

#define bool _Bool
#define NBR_MAX_TUILE 40
#define NBR_MAX_TUILE_PAR_PLATEAU 20
#define MIN_JOUEURS 2
#define MAX_JOUEURS 4

/**
 * PRE : ensembleTuiles est un pointeur vers un tableau d'entiers, presenceDUnFichier est un booléen indiquant si un fichier est présent, nomDuFichier est le nom du fichier.
 * POST : Crée un ensemble de tuiles à partir d'un fichier si presenceDUnFichier est vrai, sinon crée un ensemble de tuiles aléatoires.
 */
void creerEnsembleTuiles(int *ensembleTuiles, bool presenceDUnFichier, char *nomDuFichier);

/**
 * PRE : joueur est un pointeur vers une structure Joueur.
 * POST : Initie le plateau du joueur avec des valeurs par défaut.
 */
void initierPlateauJoueur(Joueur *joueur);

/**
 * PRE : ensembleTuiles est un pointeur vers un tableau d'entiers, tailleLogique est un pointeur vers la taille logique de l'ensemble de tuiles.
 * POST : Tire une tuile de l'ensemble de tuiles et met à jour la taille logique.
 * RETURN : La tuile tirée.
 */
int tirerTuile(int *ensembleTuiles, int *tailleLogique);

/**
 * PRE : plateau est un pointeur vers un tableau d'entiers, tuile est la tuile à placer, presenceDUnFichier est un booléen indiquant si un fichier est présent, tableauFichier est un tableau de chaînes de caractères représentant le fichier, tailleLogique est un pointeur vers la taille logique du plateau.
 * POST : Place une tuile sur le plateau si possible et met à jour la taille logique.
 * RETURN : Vrai si la tuile a été placée, faux sinon.
 */
bool placerTuile(int *plateau, int tuile, bool presenceDUnFichier, char **tableauFichier, int *tailleLogique);

/**
 * PRE : plateau est un pointeur vers un tableau d'entiers.
 * POST : Calcule et renvoie le score du plateau.
 * RETURN : Le score du plateau.
 */
int calculerScore(const int *plateau);

/**
 * PRE : plateau est un pointeur vers un tableau d'entiers.
 * POST : Affiche le plateau.
 */
void afficherPlateau(const int *plateau);

#endif
