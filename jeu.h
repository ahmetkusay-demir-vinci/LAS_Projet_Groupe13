#ifndef _JEU_H
#define _JEU_H

#include <stdbool.h>
#include "structure.h"
#include "utils_v1.h"

#define bool _Bool
#define NBR_MAX_TUILE 40
#define NBR_MAX_TUILE_PAR_PLATEAU 20
#define MIN_PLAYERS 2
#define MAX_PLAYERS 4

void creerEnsembleTuiles(int* ensembleTuiles,bool presenceDUnFichier,char* nomDuFichier);

void initierPlateauJoueur(Joueur* joueur);

int tirerTuile(int* ensembleTuiles,int* tailleLogique);

bool placerTuile(int* plateau,int tuile);

int calculerScore(const int* plateau);

void afficherPlateau(const int* plateau);

#endif
