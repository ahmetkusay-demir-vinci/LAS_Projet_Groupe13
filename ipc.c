#include "ipc.h"
#include "structure.h"

int creerClassement(int nbrJoueurs)
{
    int shm_id = sshmget(KEY_MEMORY, sizeof(Joueur )* , IPC_CREAT | PERM);
    return shm_id;
}

void lireClassement(int shm_id)
{
    // tableau de Joueur
    Joueur* classement = sshmat(shm_id, NULL, 0);
    
    for (int i = 0; i < sizeof(classement); i++)
    {
        printf("%d : Joueur %s : %s\n", i+1, classement[i].nom, classement[i].score);
    }
    sshmdt(classement);
}

int ecrireClassement(int shm_id)
{
    Joueur* classement = sshmat(shm_id, NULL, 0);
    for (int i = 0; i < sizeof(classement); i++)
    {
        
    }
    // mettre un down ici pour le semaphore
    sshmdt(classement);
    return 0;
}






int creerSemaphore(){
    int sem_id = sem_create(KEY_SEMAPHORE,NSEM,PERM,VAL);
    
    return sem_id;
}

void supprimerSemaphore(int sem_id){
    sem_delete(sem_id);
}