#ifndef _JEU_H
#define _JEU_H

#define NBR_MAX_TUILE 40

int* creerEnsembleTuile(int nbrTuiles);

int tirerTuile(int* tuiles,int tailleLogique);
// il pas mis la fonction creerPlateurJoueur car je doute
void placerTuile(int* plateau,int position,int taillelogique,int tuile);

int calculerScore(const int* plateau);

void afficherPlateau(const int* plateau);

#endif