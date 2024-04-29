#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "structure.h"
#include "utils_v1.h"
#include "network.h"
#include "jeu.h"

int main(int argc, char **argv)
{
	Joueur joueur;
	char pseudo[MAX_PSEUDO];
	int socketPlayer;
	int ret;

	StructMessage msg;

	/* retrieve player name */
	printf("Bienvenue dans le programe d'inscription au serveur de jeu\n");
	printf("Pour participer entrez votre nom :\n");
	ret = sread(0, pseudo, MAX_PSEUDO);
	checkNeg(ret, "read client error");
	pseudo[ret - 1] = '\0';
	strcpy(msg.messageText, pseudo);
	msg.code = INSCRIPTION_REQUEST;

	socketPlayer = initSocketClient(SERVER_IP, SERVER_PORT);

	swrite(socketPlayer, &msg, sizeof(msg));

	/* wait server response */
	sread(socketPlayer, &msg, sizeof(msg));

	switch (msg.code)
	{
	case INSCRIPTION_OK:
		printf("Réponse du serveur : Inscription acceptée\n");
		sread(socketPlayer, &joueur, sizeof(Joueur));
		printf("test joueur %s \n", joueur.pseudo);
		break;
	case INSCRIPTION_KO:
		printf("Réponse du serveur : Inscription refusée\n");
		sclose(socketPlayer);
		exit(0);
	default:
		printf("Réponse du serveur non prévue %d\n", msg.code);
		break;
	}

	/* wait start of game or cancel */
	sread(socketPlayer, &msg, sizeof(msg));

	if (msg.code == START_GAME)
	{
		printf("DEBUT JEU\n");
		initierPlateauJoueur(&joueur);
		afficherPlateau(joueur.plateau);
		int tuile;
		bool encours = true;

		while (encours)
		{

			sread(socketPlayer, &tuile, sizeof(int));
			if (tuile == -1)
			{
				printf("Fin des tours\n");
				
				afficherPlateau(joueur.plateau);
				
				encours = false;
				break;
			}

			if (tuile == 31)
			{
				printf("La tuile a placé est le joker\n");
			} else {
				printf("La tuile a placée est la suivante: %d \n", tuile);
			}		
			if (placerTuile(joueur.plateau, tuile))
			{
				printf("Placement de la tuile reussit\n ");
				afficherPlateau(joueur.plateau);
			}
			
			bool result = true;
			swrite(socketPlayer, &result, sizeof(bool));
		}
		int scoreFinal = calculerScore(joueur.plateau);
		printf("Vous avez obtenu un score de : %d \n", scoreFinal);
		swrite(socketPlayer, &scoreFinal, sizeof(int));

		Joueur copieClassementFinale[MAX_PLAYERS];
		sread(socketPlayer,copieClassementFinale,sizeof(Joueur)*MAX_PLAYERS);
		printf("Voici les resultats !!\n");
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			printf("%d : Joueur => %s avec un score de %d points\n",i+1,copieClassementFinale[i].pseudo,copieClassementFinale[i].score);
		}
		
	}
	else
	{
		printf("PARTIE ANNULEE\n");
		sclose(socketPlayer);
	}
	return 0;
}
