#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#define MAX_PSEUDO 256
#define MAX_TEXT 256
#define NBR_MAX_TUILE_PAR_PLATEAU 20

#define INSCRIPTION_REQUEST 10
#define INSCRIPTION_OK 11
#define INSCRIPTION_KO 12
#define START_GAME 13
#define CANCEL_GAME 14

/**
 * Structure : Joueur
 * Description : Représente un joueur dans le jeu.
 *
 * pseudo : Le pseudonyme du joueur. Doit être une chaîne de caractères de longueur inférieure à MAX_PSEUDO.
 * sockfd : Le descripteur de fichier du socket du joueur.
 * plateau : Le plateau du joueur, représenté par un tableau d'entiers. Chaque entier représente une tuile. Le nombre maximum de tuiles par plateau est NBR_MAX_TUILE_PAR_PLATEAU.
 * score : Le score du joueur.
 */
typedef struct Joueur
{
  char pseudo[MAX_PSEUDO];
  int sockfd;
  int plateau[NBR_MAX_TUILE_PAR_PLATEAU];
  int score;
} Joueur;

/**
 * Structure : StructMessage
 * Description : Représente un message qui peut être envoyé entre le client et le serveur.
 *
 * messageText : Le texte du message. Doit être une chaîne de caractères de longueur inférieure à MAX_TEXT.
 * code : Le code du message, qui indique le type de message. Les codes possibles sont définis par les constantes INSCRIPTION_REQUEST, INSCRIPTION_OK, INSCRIPTION_KO, START_GAME, CANCEL_GAME.
 */
typedef struct StructMessage
{
  char messageText[MAX_PSEUDO];
  int code;
} StructMessage;

#endif
