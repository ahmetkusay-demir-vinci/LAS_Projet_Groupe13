#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network.h"
#include "utils_v1.h"
#include "structure.h"

#define SERVER_PORT 9555
#define BACKLOG 10

int initSocketServeur(int port) {
    int sockfd = ssocket(); // Création du socket

    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)); // Configuration pour éviter "Address Already in Use"

    sbind(port, sockfd); // Liaison du socket au port spécifié

    slisten(sockfd, BACKLOG); // Configuration du socket pour écouter les connexions entrantes

    return sockfd; // Retourne le descripteur de fichier du socket
}

int initSocketClient(char *serverIP, int serverPort)
{
	int sockfd = ssocket();
	sconnect(serverIP, serverPort, sockfd);
	return sockfd;
}

void deconnecterJoueur (Joueur* tableauJoueurs, int tailleLogique) {
	for (int i = 0; i < tailleLogique; i++) {
		sclose(tableauJoueurs[i].sockfd);
	}
}
