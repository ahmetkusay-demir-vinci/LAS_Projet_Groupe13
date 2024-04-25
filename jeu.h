#ifndef _JEU_H
#define _JEU_H

#include "structure.h"

#define NBR_MAX_TUILE 40
#define NBR_MAX_TUILE_PAR_PLATEAU 20

void creerEnsembleTuiles(int* ensembleTuiles);

void initierPlateauJoueur(Joueur* joueur);

int tirerTuile(int* ensembleTuiles,int* tailleLogique);

void placerTuile(int* plateau,int position,int* taillelogique,int tuile);

void calculerScore(const int* plateau, const int* scores, int* scoreTotal);

void afficherPlateau(const int* plateau);

#endif
