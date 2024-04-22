#ifndef _JEU_H
#define _JEU_H

#include "structure.h"
#define NBR_MAX_TUILE 40

int* creerEnsembleTuile(int nbrTuiles);

void initierPlateauJoueur(Joueur* joueur);

int tirerTuile(int* tuiles,int* tailleLogique);

void placerTuile(int* plateau,int position,int* taillelogique,int tuile);

int calculerScore(const int* plateau);

void afficherPlateau(const int* plateau);

#endif
