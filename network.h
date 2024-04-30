#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "utils_v1.h"
#include "structure.h"

#define SERVEUR_IP "127.0.0.1" /* localhost */

/**
 * PRE : port est le numéro de port sur lequel le serveur doit écouter.
 * POST : Initialise un socket serveur sur le port spécifié et renvoie le descripteur de fichier du socket.
 * RETURN : Le descripteur de fichier du socket.
 */
int initSocketServeur(int port);

/**
 * PRE : serveurIP est l'adresse IP du serveur, serveurPort est le numéro de port du serveur.
 * POST : Initialise un socket client pour se connecter au serveur à l'adresse IP et au port spécifiés, et renvoie le descripteur de fichier du socket.
 * RETURN : Le descripteur de fichier du socket.
 */
int initSocketClient(char *serveurIP, int serveurPort);

/**
 * PRE : tableauJoueurs est un pointeur vers un tableau de structures Joueur, tailleLogique est la taille logique du tableau de joueurs.
 * POST : Déconnecte un joueur du tableau de joueurs et met à jour la taille logique.
 */
void deconnecterJoueur(Joueur *tableauJoueurs, int tailleLogique);

#endif
