#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils_v1.h"
#include "structure.h"
#include "network.h"
#include "jeu.h"

#define MIN_PLAYERS 2
#define MAX_PLAYERS 4
#define TIME_INSCRIPTION 15

volatile sig_atomic_t end_inscriptions = 0;
Joueur tabPlayers[MAX_PLAYERS];
int nbPLayers = 0;
int ensembleTuiles[NBR_MAX_TUILE];
// creerEnsembleTuile(ensembleTuiles);

int scores[] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};
int scoreTotal = 0;
int nbr_tours=0;
// calculerScore(plateau, scores, &scoreTotal);
int ** tablePipeEcriture;
int ** tablePipeLecture;

void endServerHandler(int sig)
{
    end_inscriptions = 1;
}

void childServerProcess(int socketPlayer,int tablePipeEcriture,int tablePipeLecture){
	while (nbr_tours<=1)
	{
		int buffer;
		sread(tablePipeLecture,&buffer,sizeof(int));
		printf("la tuile recuperer dans child process %d \n",buffer);
	}
	
	

}

void createPipes(int i){

	int pipefdEcriture[2];
	int pipefdLecture[2];

	spipe(pipefdEcriture);
	spipe(pipefdLecture);

	sclose(pipefdEcriture[0]);
	sclose(pipefdLecture[1]);

	tablePipeEcriture[i]=pipefdEcriture;
	tablePipeLecture[i]=pipefdLecture;

}


int main(int argc, char const *argv[])
{
    int sockfd, newsockfd, i;
	StructMessage msg;
	int ret;
	struct pollfd fds[MAX_PLAYERS];
	
	// Gestionnaire pour SIGALRM
	ssigaction(SIGALRM, endServerHandler);

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

				if (nbPLayers < MAX_PLAYERS)
				{
					msg.code = INSCRIPTION_OK;
					nbPLayers++;
					if (nbPLayers == MAX_PLAYERS)
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
				printf("Nb Inscriptions : %i\n", nbPLayers);
			}
		}
	}
	printf("FIN DES INSCRIPTIONS\n");
	if (nbPLayers < MIN_PLAYERS)
	{
		printf("PARTIE ANNULEE .. PAS ASSEZ DE JOUEURS\n");
		msg.code = CANCEL_GAME;
		for (i = 0; i < nbPLayers; i++)
		{
			swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
		}
		deconnecterJoueur(tabPlayers, nbPLayers);
		sclose(sockfd);
		exit(0);
	}

	printf("PARTIE VA DEMARRER ... \n");
	msg.code = START_GAME;
	for (i = 0; i < nbPLayers; i++){
		swrite(tabPlayers[i].sockfd,&tabPlayers[i],sizeof(Joueur));
		swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
		
	}
	creerEnsembleTuiles(ensembleTuiles);
	int tailleLogiqueEnsemble=NBR_MAX_TUILE;

	printf("Voici l'ensemble des tuiles \n");
	for (int i = 0; i < NBR_MAX_TUILE; i++)
	{
		printf("%d|",ensembleTuiles[i]);
	}
	printf("\n");
	
	
	for (int i = 0; i < nbPLayers; i++)
	{
		createPipes(i);
		fork_and_run3(childServerProcess,&tabPlayers[i].sockfd,&tablePipeEcriture[i],&tablePipeLecture[i]);		
	}

	while (nbr_tours<=1)
	{
		int tuileTirer = tirerTuile(ensembleTuiles,&tailleLogiqueEnsemble);
		printf("la tuile piocher est %d \n",tuileTirer);
		for (int i = 0; i < nbPLayers; i++)
		{
			swrite(*tablePipeEcriture[i],&tuileTirer,sizeof(int));
		}
		nbr_tours++;
		
	}
	
	
	







	// GAME PART
	int nbPlayersAlreadyPlayed = 0;

	// init poll
	for (i = 0; i < nbPLayers; i++)
	{
		fds[i].fd = tabPlayers[i].sockfd;
		fds[i].events = POLLIN;
	}
	// loop game


	while (nbPlayersAlreadyPlayed < nbPLayers)
	{
		// poll during 1 second
		ret = poll(fds, MAX_PLAYERS, 1000);
		checkNeg(ret, "server poll error");

		if (ret == 0)
			continue;

		// check player something to read
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				ret = sread(tabPlayers[i].sockfd, &msg, sizeof(msg));
				// tester si la connexion du client a été fermée: close(sockfd) ==> read renvoie 0
				// OU utiliser un tableau de booléens fds_invalid[i] pour indiquer
				// qu'un socket a été traité et ne doit plus l'être (cf. exemple19_avec_poll)
				// printf("poll detected POLLIN event on client socket %d (%s)... %s", tabPlayers[i].sockfd, tabPlayers[i].pseudo, ret == 0 ? "this socket is closed!\n" : "\n");

				if (ret != 0)
				{
					tabPlayers[i].shot = msg.code;
					printf("Joueur %s a joué \n", tabPlayers[i].pseudo);
					nbPlayersAlreadyPlayed++;
				}
			}
		}
	}

	deconnecterJoueur(tabPlayers, nbPLayers);
	sclose(sockfd);
	return 0;
}
