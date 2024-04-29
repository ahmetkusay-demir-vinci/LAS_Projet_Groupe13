#ifndef _IPC_H_
#define _IPC_H_

#define KEY_MEMORY 111
#define KEY_SEMAPHORE 112
#define PERM 0600
#define NSEM 1
#define VAL 1

#include <sys/ipc.h>
#include <sys/shm.h>
#include "structure.h"
#include "utils_v1.h"

//permet de créer la mémoire partagé
int creerClassement(int nbrJoueurs);

void lireClassement(int shm_id, int nbrJoueurs);

int ecrireClassement(int shm_id, int nbrJoueurs);

void supprimerClassement(int shm_id);

int creerSemaphore(key_t key,int nsems,int perm,int val);

void supprimerSemaphore(int sem_id);

#endif
