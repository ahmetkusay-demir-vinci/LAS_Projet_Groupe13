#ifndef _STRUCTURE_H
#define _STRUCTURE_H

#define MAX_PSEUDO 256
#define MAX_TEXT 256
#define NBR_MAX_TUILE_PAR_TABLEAU 20

typedef struct Joueur
{
	char pseudo[MAX_PSEUDO];
	int sockfd;
	int tableau[NBR_MAX_TUILE_PAR_TABLEAU];
    int score;
} Joueur;

typedef struct StructMessage
{
  char messageText[MAX_PSEUDO];
  int code;
} StructMessage;

#endif