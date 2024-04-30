#include <stdio.h>
#include <stdlib.h>

#include "ipc.h"

int creerClassement(Joueur *joueurs, int nbrJoueurs)
{
    int shm_id = sshmget(KEY_MEMORY, sizeof(Joueur) * 4, IPC_CREAT | PERM);

    printf("Nombre de joueurs : %d \n", nbrJoueurs);

    Joueur *classement = sshmat(shm_id);
    printf("Mémoire partagée attachée !\n");

    memcpy(classement, joueurs, sizeof(Joueur) * 4);
    printf("Classement initialisé avec succès !\n");

    sshmdt(classement);
    printf("Mémoire partagée détachée !\n");
    return shm_id;
}

void trierClassement(int shm_id, int sem_id, int nbrJoueurs)
{
    Joueur *classement = sshmat(shm_id);
    if (classement == NULL) {
        printf("Erreur : Impossible d'attacher la mémoire partagée.\n");
        return;
    }

    sem_down(sem_id, 0);

    for (int i = 0; i < nbrJoueurs - 1; i++)
    {
        for (int j = i + 1; j < nbrJoueurs; j++)
        {
            if (classement[i].score < classement[j].score)
            {
                Joueur temp = classement[i];
                classement[i] = classement[j];
                classement[j] = temp;
            }
        }
    }

    sem_up(sem_id, 0);
    sshmdt(classement);
}

void ecrireScore(int shm_id, int sem_id, int score, char *pseudo, int index)
{
    Joueur *classement = sshmat(shm_id);
    sem_down(sem_id, 0);

    strcpy(classement[index].pseudo, pseudo);
    classement[index].score = score;

    sem_up(sem_id, 0);
    sshmdt(classement);
}

void lireClassement(int shm_id, int sem_id, Joueur *copieClassement, int nbrJoueurs)
{
    Joueur *classement = sshmat(shm_id);
    sem_down(sem_id, 0);

    memcpy(copieClassement, classement, sizeof(Joueur) * nbrJoueurs);

    sem_up(sem_id, 0);
    sshmdt(classement);
}

// Pour la mémoire Partagée
void supprimerClassement(int shm_id)
{
    sshmdelete(shm_id);
    printf("Classement supprimé avec succès !\n");
}

int creerSemaphore()
{
    int sem_id = sem_create(KEY_SEMAPHORE, NSEM, PERM, VAL);
    printf("Ensemble de sémaphores initialisé avec succès !\n");
    return sem_id;
}

void supprimerSemaphore(int sem_id)
{
    sem_delete(sem_id);
    printf("Sémaphores détruits avec succès !\n");
}
