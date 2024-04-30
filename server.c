#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "utils_v1.h"
#include "structure.h"
#include "network.h"
#include "jeu.h"
#include "ipc.h"

#define TEMPS_INSCRIPTION 15

volatile sig_atomic_t finInscriptions = 0;
volatile sig_atomic_t sigintActive = 0;

Joueur tabJoueurs[MAX_JOUEURS];
int nbrJoueurs = 0;
int ensembleTuiles[NBR_MAX_TUILE];
int nbrTours = 0;

int tablePipeEcritureDuPere[MAX_JOUEURS][2];
int tablePipeEcritureDuFils[MAX_JOUEURS][2];

// création d'un masque (ensemble) pour les signaux
sigset_t set;

void endRegisterHandler(int sig)
{
	finInscriptions = 1;
}

void sigintHandler(int sig)
{
	sigintActive = 1;
}

void childServerProcess(void *arg0, void *arg1, void *arg2)
{
	int socketJoueur = *((int *)arg0);
	int *pipeEcritureDuPere = (int *)arg1;
	int *pipeEcritureDuFils = (int *)arg2;

	bool encours = true;

	sclose(pipeEcritureDuPere[1]);
	sclose(pipeEcritureDuFils[0]);

	while (encours)
	{
		int tuile;
		sread(pipeEcritureDuPere[0], &tuile, sizeof(int));
		
		swrite(socketJoueur, &tuile, sizeof(int));

		if (tuile != -1)
		{
			bool reponse;
			sread(socketJoueur, &reponse, sizeof(bool));

			swrite(pipeEcritureDuFils[1], &reponse, sizeof(bool));
		}
		else
		{
			encours = false;
		}
	}

	int score;

	sread(socketJoueur, &score, sizeof(int));

	swrite(pipeEcritureDuFils[1], &score, sizeof(int));

	bool classementFini;
	sread(pipeEcritureDuPere[0], &classementFini, sizeof(bool));

	Joueur copieClassement[MAX_JOUEURS];
	lireClassement(copieClassement, MAX_JOUEURS);

	swrite(socketJoueur, copieClassement, sizeof(Joueur) * MAX_JOUEURS);		

	// on cloture les pipe a la fin
	sclose(pipeEcritureDuPere[0]);
	sclose(pipeEcritureDuFils[1]);
}

void creerPipes(int i)
{
	spipe(tablePipeEcritureDuPere[i]);
	spipe(tablePipeEcritureDuFils[i]);
}

int main(int argc, char *argv[])
{
	int sockfd, nouveauSocketfd, i;
	StructMessage msg;
	int ret;

	int tailleLogiqueEnsemble = NBR_MAX_TUILE;
	bool relancerUnePartie = true;

	ssigemptyset(&set);
	ssigaddset(&set, SIGINT);

	// Gestionnaire pour SIGALRM
	ssigaction(SIGALRM, endRegisterHandler);
	ssigaction(SIGINT, sigintHandler);

	// Initialisation du socket serveur
	sockfd = initSocketServeur(atoi(argv[1]));
	printf("Le serveur tourne sur le port : %d \n", atoi(argv[1]));

	while (relancerUnePartie == true)
	{
		printf("\nPHASE D'INSCRIPTIONS\n");
		struct pollfd fds[MAX_JOUEURS * 2];
		i = 0;

		alarm(TEMPS_INSCRIPTION);

		while (!finInscriptions)
		{

			/* client trt */
			nouveauSocketfd = accept(sockfd, NULL, NULL); 
			if (nouveauSocketfd > 0)					 
			{

				ret = sread(nouveauSocketfd, &msg, sizeof(msg));

				if (msg.code == INSCRIPTION_REQUEST)
				{
					printf("Inscription demandée par le joueur : %s\n", msg.messageText);

					strcpy(tabJoueurs[i].pseudo, msg.messageText);
					tabJoueurs[i].sockfd = nouveauSocketfd;
					i++;

					if (nbrJoueurs < MAX_JOUEURS)
					{
						msg.code = INSCRIPTION_OK;
						nbrJoueurs++;
						if (nbrJoueurs == MAX_JOUEURS)
						{
							alarm(0); // cancel alarm
							finInscriptions = 1;
						}
					}
					else
					{
						msg.code = INSCRIPTION_KO;
					}
					ret = swrite(nouveauSocketfd, &msg, sizeof(msg));
					printf("Nombre Inscriptions : %i\n", nbrJoueurs);
				}
			}
		}

		printf("FIN DES INSCRIPTIONS\n\n");
		if (nbrJoueurs < MIN_JOUEURS || sigintActive == 1)
		{
			if (sigintActive == 1)
			{
				printf("PARTIE ANNULEE .. CTRL-C ÉFFECTUÉ \n");
				
			}
			else
			{
				printf("PARTIE ANNULEE .. PAS ASSEZ DE JOUEURS\n");
			}

			msg.code = CANCEL_GAME;

			for (i = 0; i < nbrJoueurs; i++)
			{
				swrite(tabJoueurs[i].sockfd, &msg, sizeof(msg));
			}

			deconnecterJoueur(tabJoueurs, nbrJoueurs);
			relancerUnePartie = false;
			break;
		}

		// Blocage des signaux (SIGINT)
		sigprocmask(SIG_BLOCK, &set, NULL);
		printf("PARTIE VA DEMARRER ... \n");
		msg.code = START_GAME;

		for (i = 0; i < nbrJoueurs; i++)
		{
			swrite(tabJoueurs[i].sockfd, &tabJoueurs[i], sizeof(Joueur));
			swrite(tabJoueurs[i].sockfd, &msg, sizeof(msg));
		}
		bool presenceDUnFichier = false;

		char *nomDuFichier = argv[2];
		if (argc == 3)
		{
			presenceDUnFichier = true;
			tailleLogiqueEnsemble = NBR_MAX_TUILE_PAR_PLATEAU;
		}

		creerEnsembleTuiles(ensembleTuiles, presenceDUnFichier, nomDuFichier);
		creerClassement(tabJoueurs);

		// Création des processus fils pour servir les clients
		for (int i = 0; i < nbrJoueurs; i++)
		{
			creerPipes(i);
			fds[i].fd = tablePipeEcritureDuFils[i][0];
			fds[i].events = POLLIN;
			fork_and_run3(childServerProcess, &(tabJoueurs[i].sockfd), &tablePipeEcritureDuPere[i], &tablePipeEcritureDuFils[i]);
			sclose(tablePipeEcritureDuPere[i][0]);
			sclose(tablePipeEcritureDuFils[i][1]);
		}

		int compteur;

		while (nbrTours <= 20)
		{
			int tuileTiree;
			if (nbrTours == 20)
			{
				tuileTiree = -1;
				printf("C'est la fin des tours \n");
			}
			else
			{
				tuileTiree = tirerTuile(ensembleTuiles, &tailleLogiqueEnsemble);

				printf("La tuile piochée est %d \n", tuileTiree);
			}

			for (int i = 0; i < nbrJoueurs; i++)
			{
				swrite(tablePipeEcritureDuPere[i][1], &tuileTiree, sizeof(int));
			}
			compteur = 0;

			if (tuileTiree != -1)
			{

				printf("\nAttente des joueurs \n");

				while (compteur < nbrJoueurs)
				{
					ret = spoll(fds, nbrJoueurs, 1000);
					compteur += ret;
				}

				bool reponse;
				for (int i = 0; i < nbrJoueurs; i++)
				{
					sread(tablePipeEcritureDuFils[i][0], &reponse, sizeof(bool));
				}
			}

			nbrTours++;
		}

		// LECTURE DU CLASSEMENT
		while (compteur < nbrJoueurs)
		{
			ret = spoll(fds, nbrJoueurs, 1000);
			compteur += ret;
		}

		int score;

		for (int i = 0; i < nbrJoueurs; i++)
		{
			sread(tablePipeEcritureDuFils[i][0], &score, sizeof(int));
			ecrireScore(score, tabJoueurs[i].pseudo, i);
		}
		trierClassement(MAX_JOUEURS);

		bool classementFini = true;
		for (int i = 0; i < nbrJoueurs; i++)
		{
			swrite(tablePipeEcritureDuPere[i][1], &classementFini, sizeof(bool));
		}
		
		// on cloture les pipes
		for (int i = 0; i < nbrJoueurs; i++)
		{
			sclose(tablePipeEcritureDuPere[i][1]);
			sclose(tablePipeEcritureDuFils[i][0]);
		}

		deconnecterJoueur(tabJoueurs, nbrJoueurs);
		nbrJoueurs = 0;
		nbrTours = 0;
		finInscriptions = 0;
		sigprocmask(SIG_UNBLOCK, &set, NULL);
	}

	// FIN DE JEU
	supprimerClassement();
	supprimerSemaphore();
	sclose(sockfd);
	return 0;
}
