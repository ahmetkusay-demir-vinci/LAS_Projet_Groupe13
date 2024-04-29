#include <stdio.h>
#include <stdlib.h>

#include "ipc.h"

int creerClassement(int nbrJoueurs)
{
    int shm_id = sshmget(KEY_MEMORY, sizeof(Joueur )* nbrJoueurs, IPC_CREAT | PERM);
    return shm_id;
}

void lireClassement(int shm_id, int nbrJoueurs)
{
    Joueur* classement = sshmat(shm_id, NULL, 0);
    
    for (int i = 0; i < nbrJoueurs; i++) 
    {
        printf("%d : Joueur %s : %s\n", i+1, classement[i].pseudo, classement[i].score);
    }

    sshmdt(classement);
}

int ecrireClassement(int shm_id, int nbJoueurs)
{
    Joueur* classement = sshmat(shm_id, NULL, 0);

    for (int i = 0; i < nbJoueurs; i++)
    {
        //mettre à jour le classement ici
        //printf("Joueur %s : %s\n", classement[i].nom, classement[i].score);
    }
    // mettre un down ici pour le semaphore
    // sem_down(sem_id);

    sshmdt(classement);
    return 0;
}

// Ajouter par moi (kusay) - Pour la mémoire partagée
int supprimerClassement(int shm_id)
{
    shmdelete(shm_id);
    printf("Classement supprimé avec succès !\n");
}

int creerSemaphore(key_t key,int nsems,int perm,int val){
    int sem_id = sem_create(KEY_SEMAPHORE,NSEM,PERM,VAL);
    printf("Ensemble de sémaphores initialisé avec succès !\n");
    return sem_id;
}

void supprimerSemaphore(int sem_id){
    sem_delete(sem_id);
    printf("Sémaphores détruits avec succès !\n");
}
