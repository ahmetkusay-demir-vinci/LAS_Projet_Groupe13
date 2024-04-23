#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "utils_v1.h"
#include "structure.h"

#define SERVER_PORT 9555
#define SERVER_IP "127.0.0.1" /* localhost */

int initSocketServeur (int port);

int initSocketClient (char *serverIP, int serverPort);

void deconnecterJoueur (Joueur* tableauJoueurs,int tailleLogique);

#endif
