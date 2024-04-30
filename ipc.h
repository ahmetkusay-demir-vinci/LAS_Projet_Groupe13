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
 * PRE : joueurs est un pointeur vers un tableau de structures Joueur.
 * POST : Crée un classement des joueurs.
 */
void creerClassement(Joueur *joueurs);

/**
 * PRE : nbrJoueurs est le nombre de joueurs.
 * POST : Trie le classement des joueurs.
 */
void trierClassement(int nbrJoueurs);

/**
 * PRE : score est le score du joueur, pseudo est le pseudonyme du joueur, index est l'index du joueur dans le tableau.
 * POST : Écrit le score du joueur dans le classement à l'index spécifié.
 */
void ecrireScore(int score, char *pseudo, int index);

/**
 * PRE : copieClassement est un pointeur vers un tableau de structures Joueur, nbrJoueurs est le nombre de joueurs.
 * POST : Lit le classement des joueurs dans copieClassement.
 */
void lireClassement(Joueur *copieClassement, int nbrJoueurs);

/**
 * PRE : Aucune.
 * POST : Supprime le classement des joueurs.
 */
void supprimerClassement();

/**
 * PRE : Aucune.
 * POST : Crée un sémaphore.
 */
void creerSemaphore();

/**
 * PRE : Aucune.
 * POST : Supprime le sémaphore.
 */
void supprimerSemaphore();

#endif
