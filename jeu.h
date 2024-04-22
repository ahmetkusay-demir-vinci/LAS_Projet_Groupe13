#ifndef _JEU_H
#define _JEU_H

#define NBR_MAX_TUILE 40

int* creerEnsembleTuile(int nbrTuiles);

// il pas mis la fonction creerPlateurJoueur car je doute
// int* creerPlateauJoueur(int taille);

int tirerTuile(int* tuiles,int tailleLogique);

void placerTuile(int* plateau,int position,int taillelogique,int tuile);

int calculerScore(const int* plateau, int size);

void afficherPlateau(const int* plateau, int size);

#endif