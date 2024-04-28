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

#define MIN_PLAYERS 2
#define MAX_PLAYERS 4
#define TIME_INSCRIPTION 15

volatile sig_atomic_t end_inscriptions = 0;
// volatile sig_atomic_t sigint_pressed = 0;

Joueur tabPlayers[MAX_PLAYERS];
int nbPlayers = 0;
int ensembleTuiles[NBR_MAX_TUILE];
// creerEnsembleTuile(ensembleTuiles);

int scores[] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};
int scoreTotal = 0;
int nbr_tours = 0;

// calculerScore(plateau, scores, &scoreTotal);
int tablePipeEcritureDuPere[4][2];
int tablePipeEcritureDuFils[4][2];

// création d'un masque (ensemble) pour les signaux
// sigset_t set;

// Initialisation des fonctions
bool contientEntier(int *tableau, int element, int tailleLogique);

void endRegisterHandler(int sig)
{
	end_inscriptions = 1;
}

/*void sigintHandler(int sig)
{
	sigint_pressed = 1;
}*/

void childServerProcess(void *arg0, void *arg1, void *arg2)
{
	int socketPlayer = *((int *)arg0);
	int *pipeEcritureDuPere = (int *)arg1;
	int *pipeEcritureDuFils = (int *)arg2;

	bool encours = true;

	// struct pollfd fdsPipe[MAX_PLAYERS * 2];

	// init poll
	// fdsPipe[0].fd = pipeEcritureDuPere[0];
	// fdsPipe[0].events = POLLIN;

	// struct pollfd fdsSocket[MAX_PLAYERS];

	// init poll
	// fdsSocket[0].fd = socketPlayer;
	// fdsSocket[0].events = POLLIN;

	// int ret;

	sclose(pipeEcritureDuPere[1]);
	sclose(pipeEcritureDuFils[0]);

	while (encours)
	{

		/*ret = poll(fdsPipe, MAX_PLAYERS, 1000);
		checkNeg(ret, "server poll error");

		if (ret == 0)
			continue;

		if (fdsPipe[0].revents & POLLIN)
		{
			int tuile;
			sread(pipeEcritureDuPere[0], &tuile, sizeof(int));

			printf("La tuile récupérée dans le pro");
			swrite(socketPlayer, &tuile, sizeof(int));
		}

		ret = poll(fdsSocket, MAX_PLAYERS, 1000);
		checkNeg(ret, "server poll error");

		if (ret == 0)
			continue;

		if (fdsSocket[0].revents & POLLIN)
		{
			bool reponse;
			sread(fdsSocket[0].fd, &reponse, sizeof(bool));

			printf(" La tuile a ete poser dans le plateau ! \n");

			swrite(pipeEcritureDuFils[1], &reponse, sizeof(bool));
		}

		encours = false;*/

		int tuile;
		sread(pipeEcritureDuPere[0], &tuile, sizeof(int));

		printf("La tuile récupérée du serveur est %d \n", tuile);
		swrite(socketPlayer, &tuile, sizeof(int));

		bool reponse;
		sread(socketPlayer, &reponse, sizeof(bool));

		printf(" La tuile a ete poser dans le plateau ! \n");

		swrite(pipeEcritureDuFils[1], &reponse, sizeof(bool));

		encours = true;
	}

	printf("c %d \n", pipeEcritureDuFils[0]);

	// on cloture les pipe a la fin
	sclose(pipeEcritureDuPere[0]);
	sclose(pipeEcritureDuFils[1]);
}

void createPipes(int i)
{
	spipe(tablePipeEcritureDuPere[i]);
	spipe(tablePipeEcritureDuFils[i]);
}

int main(int argc, char const *argv[])
{
	int sockfd, newsockfd, i;
	StructMessage msg;
	int ret;
	struct pollfd fds[MAX_PLAYERS * 2];

	// ssigemptyset(&set);
	// ssigaddset(&set, SIGINT);

	// Gestionnaire pour SIGALRM
	ssigaction(SIGALRM, endRegisterHandler);
	// ssigaction(SIGINT, sigintHandler);

	// Initialisation du socket serveur
	sockfd = initSocketServeur(SERVER_PORT);
	printf("Le serveur tourne sur le port : %i \n", SERVER_PORT);

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

	printf("FIN DES INSCRIPTIONS\n");
	if (nbPlayers < MIN_PLAYERS /*|| sigint_pressed == 1*/)
	{
		/*if (sigint_pressed == 1)
		{
			printf("PARTIE ANNULEE .. CTRL-C ÉFFECTUÉ \n");
		}
		else
		{
			printf("PARTIE ANNULEE .. PAS ASSEZ DE JOUEURS\n");
		}*/

		msg.code = CANCEL_GAME;

		for (i = 0; i < nbPlayers; i++)
		{
			swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
		}

		deconnecterJoueur(tabPlayers, nbPlayers);
		sclose(sockfd);
		exit(0);
	}

	// Blocage des signaux (SIGINT)
	// sigprocmask(SIG_BLOCK, &set, NULL);

	printf("PARTIE VA DEMARRER ... \n");
	msg.code = START_GAME;

	for (i = 0; i < nbPlayers; i++)
	{
		swrite(tabPlayers[i].sockfd, &tabPlayers[i], sizeof(Joueur));
		swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
	}

	creerEnsembleTuiles(ensembleTuiles);
	int tailleLogiqueEnsemble = NBR_MAX_TUILE;

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

	while (nbr_tours < 1)
	{
		int tuileTirer = tirerTuile(ensembleTuiles, &tailleLogiqueEnsemble);

		printf("la tuile pioché est %d \n", tuileTirer);

		for (int i = 0; i < nbPlayers; i++)
		{
			swrite(tablePipeEcritureDuPere[i][1], &tuileTirer, sizeof(int));
		}

		int compteur = 0;
		int listeIndicesDejaRepondus[MAX_PLAYERS];
		printf("attente des joueurs \n");

		while (compteur <= nbPlayers)
		{
			ret = poll(fds, nbPlayers, 1000);
			checkNeg(ret, "server poll error");

			if (ret == 0)
				continue;

			for (int i = 0; i < nbPlayers; i++)
			{
				if (!contientEntier(listeIndicesDejaRepondus, i, compteur) && fds[i].revents & POLLIN)
				{
					listeIndicesDejaRepondus[compteur] = i;
					compteur++;
					printf("%d joueurs ont repondu\n", compteur);
				}
			}
		}

		nbr_tours++;
	}

	// FIN DE PARTIE

	// Déblocage des signaux (SIGINT)
	// sigprocmask(SIG_UNBLOCK, &set, NULL);

	// on cloture les pipe
	sclose(tablePipeEcritureDuPere[i][1]);
	sclose(tablePipeEcritureDuFils[i][0]);

	deconnecterJoueur(tabPlayers, nbPlayers);
	sclose(sockfd);
	return 0;
}

bool contientEntier(int *tableau, int element, int tailleLogique)
{
	bool resultat = false;

	for (int j = 0; j < tailleLogique; j++)
	{
		if (tableau[j] == element)
		{
			resultat = true;
			break;
		}
	}

	return resultat;
}
