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
	int sockfd;
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

	sockfd = initSocketClient(SERVER_IP, SERVER_PORT);

	swrite(sockfd, &msg, sizeof(msg));

	/* wait server response */
	sread(sockfd, &msg, sizeof(msg));

	switch (msg.code)
	{
	case INSCRIPTION_OK:
		printf("Réponse du serveur : Inscription acceptée\n");
		sread(sockfd, &joueur, sizeof(Joueur));
		printf("test joueur %s \n",joueur.pseudo);
		break;
	case INSCRIPTION_KO:
		printf("Réponse du serveur : Inscription refusée\n");
		sclose(sockfd);
		exit(0);
	default:
		printf("Réponse du serveur non prévue %d\n", msg.code);
		break;
	}

	/* wait start of game or cancel */
	sread(sockfd, &msg, sizeof(msg));

	if (msg.code == START_GAME)
	{
		printf("DEBUT JEU\n");
		initierPlateauJoueur(&joueur);
		afficherPlateau(joueur.plateau);
/*int* tuile;
		sread(sockfd, &tuile, sizeof(int));
		printf("la tuile est arrive a bon port %d \n",*tuile);*/
		
		
	}
	else
	{
		printf("PARTIE ANNULEE\n");
		sclose(sockfd);
	}
	return 0;
}
