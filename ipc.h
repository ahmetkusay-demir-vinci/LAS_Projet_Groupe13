#ifndef _IPC_H_
#define _IPC_H_

#define KEY_MEMORY 114
#define KEY_SEMAPHORE 112
#define PERM 0600
#define NSEM 1
#define VAL 1

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "structure.h"
#include "utils_v1.h"

//permet de créer la mémoire partagé
int creerClassement(Joueur* joueurs, int nbrJoueurs);

void trierClassement(int shm_id, int sem_id, int nbrJoueurs);

void ecrireScore(int shm_id, int sem_id, int score, char* pseudo, int index);

void lireClassement(int shm_id, int sem_id, Joueur* copieClassement, int nbrJoueurs);

void supprimerClassement(int shm_id);

int creerSemaphore();

void supprimerSemaphore(int sem_id);

#endif
