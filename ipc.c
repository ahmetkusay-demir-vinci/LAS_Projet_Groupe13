#include <stdio.h>
#include <stdlib.h>

#include "ipc.h"
#include "jeu.h"

int shm_id = -1;
int sem_id = -1;

void creerClassement(Joueur *joueurs)
{
    shm_id = sshmget(KEY_MEMORY, sizeof(Joueur) * MAX_JOUEURS, IPC_CREAT | PERM);

    Joueur *classement = sshmat(shm_id);

    memcpy(classement, joueurs, sizeof(Joueur) * MAX_JOUEURS);

    sshmdt(classement);

    creerSemaphore();
}

void trierClassement(int nbrJoueurs)
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

void ecrireScore(int score, char *pseudo, int index)
{
 
    Joueur *classement = sshmat(shm_id);
    sem_down(sem_id, 0);

    strcpy(classement[index].pseudo, pseudo);
    classement[index].score = score;

    sem_up(sem_id, 0);
    sshmdt(classement);
}

void lireClassement(Joueur *copieClassement, int nbrJoueurs)
{
    
    Joueur *classement = sshmat(shm_id);
    sem_down(sem_id, 0);

    memcpy(copieClassement, classement, sizeof(Joueur) * nbrJoueurs);

    sem_up(sem_id, 0);
    sshmdt(classement);
}

// Pour la mémoire Partagée
void supprimerClassement()
{
    if (shm_id!=-1)
    {
        sshmdelete(shm_id);
    }
}

void creerSemaphore()
{
    sem_id = sem_create(KEY_SEMAPHORE, NSEM, PERM, VAL);
}

void supprimerSemaphore()
{
    if (sem_id!=-1)
    {
        sem_delete(sem_id);
    }   
}
