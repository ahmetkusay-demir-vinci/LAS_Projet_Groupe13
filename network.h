#ifndef _NETWORK_H
#define _NETWORK_H

#include "structure.h"
#define SERVER_PORT 9555


int initSocketServeur (int port);

int initSocketClient (int port);

void deconnecterJoueur (Joueur* tableauJoueurs,int tailleLogique);

#endif