/**
 * @file ipc.h
 * @brief Fichier d'en-tête pour les fonctions d'IPC (Inter-Process Communication).
 */

#ifndef _IPC_H_
#define _IPC_H_

#define KEY_MEMORY 119
#define KEY_SEMAPHORE 112
#define PERM 0666
#define NSEM 1
#define VAL 1

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "structure.h"
#include "utils_v1.h"

/**
 * @brief Crée un segment de mémoire partagée pour le classement des joueurs.
 *
 * PRE : Un tableau de joueurs et sa taille sont passés en argument.
 * POST : Un segment de mémoire partagée est créé pour le classement des joueurs et son ID est renvoyé.
 *
 * @param joueurs Le tableau de joueurs.
 * @param nbrJoueurs Le nombre de joueurs.
 */
void creerClassement(Joueur *joueurs);

/**
 * @brief Trie le classement des joueurs dans la mémoire partagée.
 *
 * PRE : Les ID du segment de mémoire partagée et du sémaphore, ainsi que le nombre de joueurs sont passés en argument.
 * POST : Le classement des joueurs dans la mémoire partagée est trié.
 *
 * @param nbrJoueurs Le nombre de joueurs.
 */
void trierClassement(int nbrJoueurs);

/**
 * @brief Écrit le score d'un joueur dans la mémoire partagée.
 *
 * PRE : Les ID du segment de mémoire partagée et du sémaphore, le score du joueur, son pseudo et son index sont passés en argument.
 * POST : Le score du joueur est écrit dans la mémoire partagée.
 *
 * @param score Le score du joueur.
 * @param pseudo Le pseudo du joueur.
 * @param index L'index du joueur.
 */
void ecrireScore(int score, char *pseudo, int index);

/**
 * @brief Lit le classement des joueurs dans la mémoire partagée.
 *
 * PRE : Les ID du segment de mémoire partagée et du sémaphore, un tableau pour copier le classement et le nombre de joueurs sont passés en argument.
 * POST : Le classement des joueurs est lu de la mémoire partagée et copié dans le tableau.
 *
 * @param copieClassement Le tableau pour copier le classement.
 * @param nbrJoueurs Le nombre de joueurs.
 */
void lireClassement(Joueur *copieClassement, int nbrJoueurs);

/**
 * @brief Supprime le segment de mémoire partagée pour le classement des joueurs.
 *
 * PRE : L'ID du segment de mémoire partagée est passé en argument.
 * POST : Le segment de mémoire partagée pour le classement des joueurs est supprimé.
 */
void supprimerClassement();

/**
 * @brief Crée un sémaphore.
 *
 * PRE : Aucun argument n'est passé.
 * POST : Un sémaphore est créé et son ID est renvoyé.
 */
void creerSemaphore();

/**
 * @brief Supprime un sémaphore.
 *
 * PRE : L'ID du sémaphore est passé en argument.
 * POST : Le sémaphore est supprimé.
 */
void supprimerSemaphore();

#endif
