#include <stdio.h>
#include <stdlib.h>

#include "ipc.h"

int creerClassement(Joueur* joueurs, int nbrJoueurs)
{
    int shm_id = sshmget(KEY_MEMORY, sizeof(Joueur )*nbrJoueurs, IPC_CREAT | PERM);
    return shm_id;

    Joueur* classement = sshmat(shm_id);
    printf("Mémoire partagée attachée !\n");

    memcpy(classement, &joueurs, sizeof(Joueur)*nbrJoueurs);
    printf("Classement initialisé avec succès !\n");

    sshmdt(classement);
    printf("Mémoire partagée détachée !\n");
    return shm_id;
}

void trierClassement(int shm_id, int sem_id, int nbrJoueurs)
{   
    Joueur** classement = sshmat(shm_id);
    sem_down(sem_id, 1);

    for (int i = 0; i < nbrJoueurs - 1; i++)
    {
        for (int j = i + 1; j < nbrJoueurs; j++)
        {
            if (classement[j]->score > classement[i]->score)
            {
                Joueur *temp = classement[i];
                classement[i] = classement[j];
                classement[j] = temp;
            }
        }
    }

    sem_up(shm_id, 1);
    sshmdt(classement);
}

void ecrireScore(int shm_id, int sem_id, int score, int index)
{
    Joueur* classement = sshmat(shm_id);
    sem_down(sem_id, 1);

    classement[index].score = score;

    sem_up(sem_id, 1);
    sshmdt(classement);
}

void lireClassement(int shm_id, int sem_id, Joueur* copieClassement, int nbrJoueurs) {
    Joueur* classement = sshmat(shm_id);
    sem_down(sem_id, 1);
    
    memcpy(copieClassement, classement, sizeof(Joueur)*nbrJoueurs);

    sem_up(sem_id, 1);
    sshmdt(classement);
}

//Pour la mémoire Partagée
void supprimerClassement(int shm_id)
{
    sshmdelete(shm_id);
    printf("Classement supprimé avec succès !\n");
}

int creerSemaphore(){
    int sem_id = sem_create(KEY_SEMAPHORE,NSEM,PERM,VAL);
    printf("Ensemble de sémaphores initialisé avec succès !\n");
    return sem_id;
}

void supprimerSemaphore(int sem_id){
    sem_delete(sem_id);
    printf("Sémaphores détruits avec succès !\n");
}
