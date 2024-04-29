/**
 * @file jeu.h
 * @brief Fichier d'en-tête pour les fonctions de jeu.
 */

#ifndef _JEU_H
#define _JEU_H

#include <stdbool.h>
#include "structure.h"
#include "utils_v1.h"

#define bool _Bool
#define NBR_MAX_TUILE 40
#define NBR_MAX_TUILE_PAR_PLATEAU 20
#define MIN_PLAYERS 2
#define MAX_PLAYERS 4

/**
 * @brief Crée un ensemble de tuiles.
 *
 * PRE : Un pointeur vers un tableau d'entiers, un booléen indiquant la présence d'un fichier et le nom du fichier sont passés en argument.
 * POST : Un ensemble de tuiles est créé et stocké dans le tableau d'entiers.
 *
 * @param ensembleTuiles Le tableau dans lequel stocker l'ensemble de tuiles.
 * @param presenceDUnFichier Booléen indiquant si un fichier est présent.
 * @param nomDuFichier Le nom du fichier à utiliser pour créer l'ensemble de tuiles.
 */
void creerEnsembleTuiles(int *ensembleTuiles, bool presenceDUnFichier, char *nomDuFichier);

/**
 * @brief Initialise le plateau d'un joueur.
 *
 * PRE : Un pointeur vers un joueur est passé en argument.
 * POST : Le plateau du joueur est initialisé.
 *
 * @param joueur Le joueur dont le plateau doit être initialisé.
 */
void initierPlateauJoueur(Joueur *joueur);

/**
 * @brief Tire une tuile de l'ensemble de tuiles.
 *
 * PRE : Un pointeur vers un ensemble de tuiles et un pointeur vers sa taille logique sont passés en argument.
 * POST : Une tuile est tirée de l'ensemble de tuiles et sa valeur est renvoyée.
 *
 * @param ensembleTuiles L'ensemble de tuiles.
 * @param tailleLogique La taille logique de l'ensemble de tuiles.
 * @return La valeur de la tuile tirée.
 */
int tirerTuile(int *ensembleTuiles, int *tailleLogique);

/**
 * @brief Place une tuile sur le plateau.
 *
 * PRE : Un pointeur vers un plateau et une tuile sont passés en argument.
 * POST : La tuile est placée sur le plateau et un booléen indiquant si l'opération a réussi est renvoyé.
 *
 * @param plateau Le plateau sur lequel placer la tuile.
 * @param tuile La tuile à placer.
 * @return Un booléen indiquant si l'opération a réussi.
 */
bool placerTuile(int *plateau, int tuile);

/**
 * @brief Calcule le score d'un plateau.
 *
 * PRE : Un pointeur vers un plateau est passé en argument.
 * POST : Le score du plateau est calculé et renvoyé.
 *
 * @param plateau Le plateau dont le score doit être calculé.
 * @return Le score du plateau.
 */
int calculerScore(const int *plateau);

/**
 * @brief Affiche un plateau.
 *
 * PRE : Un pointeur vers un plateau est passé en argument.
 * POST : Le plateau est affiché à l'écran.
 *
 * @param plateau Le plateau à afficher.
 */
void afficherPlateau(const int *plateau);

#endif
