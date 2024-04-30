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

#define TIME_INSCRIPTION 15

volatile sig_atomic_t end_inscriptions = 0;
volatile sig_atomic_t sigint_pressed = 0;

Joueur tabPlayers[MAX_PLAYERS];
int nbPlayers = 0;
int ensembleTuiles[NBR_MAX_TUILE];
int nbr_tours = 0;

int tablePipeEcritureDuPere[MAX_PLAYERS][2];
int tablePipeEcritureDuFils[MAX_PLAYERS][2];

// création d'un masque (ensemble) pour les signaux
sigset_t set;


void endRegisterHandler(int sig)
{
	end_inscriptions = 1;
}

void sigintHandler(int sig)
{
	sigint_pressed = 1;
}

void childServerProcess(void *arg0, void *arg1, void *arg2)
{
	int socketPlayer = *((int *)arg0);
	int *pipeEcritureDuPere = (int *)arg1;
	int *pipeEcritureDuFils = (int *)arg2;

	bool encours = true;

	sclose(pipeEcritureDuPere[1]);
	sclose(pipeEcritureDuFils[0]);

	while (encours)
	{
		int tuile;
		sread(pipeEcritureDuPere[0], &tuile, sizeof(int));

		if (tuile != -1)
		{
			printf("La tuile récupérée du serveur est %d \n", tuile);
		}
		swrite(socketPlayer, &tuile, sizeof(int));

		if (tuile != -1)
		{
			bool reponse;
			sread(socketPlayer, &reponse, sizeof(bool));

			printf("La tuile a été posée dans le plateau ! \n");

			swrite(pipeEcritureDuFils[1], &reponse, sizeof(bool));
		}
		else
		{
			encours = false;
		}
	}

	encours = true;

	while (encours)
	{
		int score;

		sread(socketPlayer, &score, sizeof(int));
		printf("Le score a été recu au niveau du serveur fils\n");

		swrite(pipeEcritureDuFils[1], &score, sizeof(int));
		printf("Envoie du score au serveur parent \n");

		// Dawid
		bool classementFini;
		sread(pipeEcritureDuPere[0], &classementFini, sizeof(bool));

		Joueur copieClassement[MAX_PLAYERS];
		lireClassement(copieClassement, MAX_PLAYERS);

		swrite(socketPlayer, copieClassement, sizeof(Joueur) * MAX_PLAYERS);

		encours = false;
		// mettre classement ici
	}

	// on cloture les pipe a la fin
	sclose(pipeEcritureDuPere[0]);
	sclose(pipeEcritureDuFils[1]);
}

void createPipes(int i)
{
	spipe(tablePipeEcritureDuPere[i]);
	spipe(tablePipeEcritureDuFils[i]);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, i;
	StructMessage msg;
	int ret;
	
	int tailleLogiqueEnsemble = NBR_MAX_TUILE;
	bool enpartie=true;

	ssigemptyset(&set);
	ssigaddset(&set, SIGINT);

	// Gestionnaire pour SIGALRM
	ssigaction(SIGALRM, endRegisterHandler);
	ssigaction(SIGINT, sigintHandler);

	// Initialisation du socket serveur
	sockfd = initSocketServeur(atoi(argv[1]));
	printf("Le serveur tourne sur le port : %d \n", atoi(argv[1]));

	while (enpartie==true)
	{
		printf("PHASE D'INSCRIPTIONS\n");
		struct pollfd fds[MAX_PLAYERS * 2];
		i = 0;
		
		// INSCRIPTION PART
		alarm(TIME_INSCRIPTION);

		while (!end_inscriptions)
		{
			
			/* client trt */
			newsockfd = accept(sockfd, NULL, NULL); // saccept() exit le programme si accept a été interrompu par l'alarme
			if (newsockfd > 0)						/* no error on accept */
			{

				ret = sread(newsockfd, &msg, sizeof(msg));

				if (msg.code == INSCRIPTION_REQUEST)
				{
					printf("Inscription demandée par le joueur : %s\n", msg.messageText);

					strcpy(tabPlayers[i].pseudo, msg.messageText);
					tabPlayers[i].sockfd = newsockfd;
					i++;

					if (nbPlayers < MAX_PLAYERS)
					{
						msg.code = INSCRIPTION_OK;
						nbPlayers++;
						if (nbPlayers == MAX_PLAYERS)
						{
							alarm(0); // cancel alarm
							end_inscriptions = 1;
						}
					}
					else
					{
						msg.code = INSCRIPTION_KO;
					}
					ret = swrite(newsockfd, &msg, sizeof(msg));
					printf("Nb Inscriptions : %i\n", nbPlayers);
				}
			}
		}
		
		printf("FIN DES INSCRIPTIONS\n\n");
		if (nbPlayers < MIN_PLAYERS || sigint_pressed == 1)
		{
			if (sigint_pressed == 1)
			{
				printf("PARTIE ANNULEE .. CTRL-C ÉFFECTUÉ \n");
				enpartie=false;
				break;
			}
			else
			{
				printf("PARTIE ANNULEE .. PAS ASSEZ DE JOUEURS\n");
			}

			msg.code = CANCEL_GAME;

			for (i = 0; i < nbPlayers; i++)
			{
				swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
			}

			deconnecterJoueur(tabPlayers, nbPlayers);
			break;
			
		}

		// Blocage des signaux (SIGINT)
		sigprocmask(SIG_BLOCK, &set, NULL);
		printf("PARTIE VA DEMARRER ... \n");
		msg.code = START_GAME;

		for (i = 0; i < nbPlayers; i++)
		{
			swrite(tabPlayers[i].sockfd, &tabPlayers[i], sizeof(Joueur));
			swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
		}
		bool presenceDUnFichier = false;

		char *nomDuFichier = argv[2];
		if (argc == 3)
		{
			presenceDUnFichier = true;
			tailleLogiqueEnsemble = NBR_MAX_TUILE_PAR_PLATEAU;
		}

		creerEnsembleTuiles(ensembleTuiles, presenceDUnFichier, nomDuFichier);
		creerClassement(tabPlayers, nbPlayers);

		// Création des processus fils pour servir les clients
		for (int i = 0; i < nbPlayers; i++)
		{
			createPipes(i);
			fds[i].fd = tablePipeEcritureDuFils[i][0];
			fds[i].events = POLLIN;
			fork_and_run3(childServerProcess, &(tabPlayers[i].sockfd), &tablePipeEcritureDuPere[i], &tablePipeEcritureDuFils[i]);
			sclose(tablePipeEcritureDuPere[i][0]);
			sclose(tablePipeEcritureDuFils[i][1]);
		}

		int compteur;

		while (nbr_tours <= 20)
		{
			int tuileTirer;
			if (nbr_tours == 20)
			{
				tuileTirer = -1;
				printf("C'est la fin des tours \n");
			}
			else
			{
				tuileTirer = tirerTuile(ensembleTuiles, &tailleLogiqueEnsemble);

				printf("La tuile pioché est %d \n", tuileTirer);
			}

			for (int i = 0; i < nbPlayers; i++)
			{
				swrite(tablePipeEcritureDuPere[i][1], &tuileTirer, sizeof(int));
			}
			compteur = 0;

			if (tuileTirer != -1)
			{

				printf("\nAttente des joueurs \n");

				while (compteur < nbPlayers)
				{
					ret = spoll(fds, nbPlayers, 1000);
					compteur += ret;
				}

				bool reponse;
				for (int i = 0; i < nbPlayers; i++)
				{
					sread(tablePipeEcritureDuFils[i][0], &reponse, sizeof(bool));
					printf("Le joueur %s a répondu %d \n", tabPlayers[i].pseudo, reponse);
				}
			}

			nbr_tours++;
		}

		// LECTURE DU CLASSEMENT
		while (compteur < nbPlayers)
		{
			ret = spoll(fds, nbPlayers, 1000);
			compteur += ret;
		}

		int score;

		for (int i = 0; i < nbPlayers; i++)
		{
			sread(tablePipeEcritureDuFils[i][0], &score, sizeof(int));
			ecrireScore(score, tabPlayers[i].pseudo, i);
		}
		trierClassement(MAX_PLAYERS);

		bool classementFini = true;
		for (int i = 0; i < nbPlayers; i++)
		{
			swrite(tablePipeEcritureDuPere[i][1], &classementFini, sizeof(bool));
		}
		end_inscriptions = 0;
		// on cloture les pipe
		for (int i = 0; i < nbPlayers; i++)
		{
			sclose(tablePipeEcritureDuPere[i][1]);
			sclose(tablePipeEcritureDuFils[i][0]);
		}

		deconnecterJoueur(tabPlayers, nbPlayers);
		nbPlayers=0;
		nbr_tours=0;
		sigprocmask(SIG_UNBLOCK, &set, NULL);
	}

	

	// FIN DE JEU

	// Déblocage des signaux (SIGINT)
	
	supprimerClassement();
	supprimerSemaphore();
	sclose(sockfd);
	return 0;
}
