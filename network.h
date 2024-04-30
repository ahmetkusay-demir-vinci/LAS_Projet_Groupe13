/**
 * @file network.h
 * @brief Fichier d'en-tête pour les fonctions de réseau.
 */

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "utils_v1.h"
#include "structure.h"


#define SERVER_IP "127.0.0.1" /* localhost */

/**
 * @brief Initialise un socket serveur.
 *
 * PRE : Le port sur lequel le serveur doit être initialisé est passé en argument.
 * POST : Un socket serveur est initialisé et son descripteur est renvoyé. En cas d'erreur, une valeur négative est renvoyée.
 *
 * @param port Le port sur lequel initialiser le serveur.
 * @return Le descripteur du socket serveur, ou une valeur négative en cas d'erreur.
 */
int initSocketServeur(int port);

/**
 * @brief Initialise un socket client.
 *
 * PRE : L'adresse IP du serveur et le port du serveur sont passés en argument.
 * POST : Un socket client est initialisé et son descripteur est renvoyé. En cas d'erreur, une valeur négative est renvoyée.
 *
 * @param serverIP L'adresse IP du serveur.
 * @param serverPort Le port du serveur.
 * @return Le descripteur du socket client, ou une valeur négative en cas d'erreur.
 */
int initSocketClient(char *serverIP, int serverPort);

/**
 * @brief Déconnecte un joueur.
 *
 * PRE : Un tableau de joueurs et sa taille logique sont passés en argument.
 * POST : Le joueur est déconnecté et le tableau de joueurs est mis à jour.
 *
 * @param tableauJoueurs Le tableau de joueurs.
 * @param tailleLogique La taille logique du tableau de joueurs.
 */
void deconnecterJoueur(Joueur *tableauJoueurs, int tailleLogique);

#endif
