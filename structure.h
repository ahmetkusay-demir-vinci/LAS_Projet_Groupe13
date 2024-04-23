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

typedef struct Joueur
{
	char pseudo[MAX_PSEUDO];
	int sockfd;
	int plateau[NBR_MAX_TUILE_PAR_PLATEAU];
  int score;
  int shot;
} Joueur;

typedef struct StructMessage
{
  char messageText[MAX_PSEUDO];
  int code;
} StructMessage;

#endif
