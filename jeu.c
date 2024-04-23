#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>


int* creerEnsembleTuile() {
    int* ensembleTuile = (int*)malloc(NBR_MAX_TUILE * sizeof(int));

    // Création des tuiles numérotées de 1 à 30 (avec doublons pour 11-19) et un joker (31)
    int index = 0;
    for (int i = 1; i <= 30; i++) {
        ensembleTuile[index++] = i;
        if (i >= 11 && i <= 19) {
            ensembleTuile[index++] = i; // Doublon pour 11-19
        }
    }
    ensembleTuile[index] = 31; // Joker

    return ensembleTuile;
}

void initierPlateauJoueur(Joueur* joueur) {
    // Initialisation du plateau du joueur avec des valeurs par défaut (0)
    for (int i = 0; i < NBR_MAX_TUILE_PAR_PLATEAU; i++) {
        joueur->plateau[i] = 0;
    }
}

int tirerTuile(int* tuiles, int* tailleLogique) {
    if (*tailleLogique <= 0) {
        return -1; // Si le nombre de tuiles est insuffisant
    }
    int index = rand() % *tailleLogique; // Tirage aléatoire d'une tuile
    int tuile = tuiles[index];
    
    // Suppression de la tuile tirée de l'ensemble de tuiles
    for (int i = index; i < (*tailleLogique - 1); i++) {
        tuiles[i] = tuiles[i + 1];
    }
    (*tailleLogique)--;

    return tuile;
}

void placerTuile(int* plateau, int position, int* tailleLogique, int tuile) {
    if (position < 0 || position >= NBR_MAX_TUILE_PAR_PLATEAU) {
        printf("Position invalide.\n");
        return;
    }

    // Si la position est déjà occupée, la tuile sera placée à droite de la position demandée
    while (plateau[position] != 0) {
        position++;
        if (position >= NBR_MAX_TUILE_PAR_PLATEAU) {
            printf("Impossible de placer la tuile à la position spécifiée.\n");
            return;
        }
    }

    plateau[position] = tuile;
    (*tailleLogique)++;
}

int calculerScore(const int* plateau) {
    int score = 0;
    int suiteLength = 0;
    int prevTuile = 0;

    for (int i = 0; i < NBR_MAX_TUILE_PAR_PLATEAU; i++) {
        if (plateau[i] != 0) {
            if (plateau[i] > prevTuile) {
                suiteLength++;
            } else {
                score += (suiteLength * (suiteLength + 1)) / 2; // Formule pour calculer le score d'une suite
                suiteLength = 1;
            }
            prevTuile = plateau[i];
        }
    }
    score += (suiteLength * (suiteLength + 1)) / 2; // Ajout du score de la dernière suite

    return score;
}

void afficherPlateau(const int* plateau) {
    printf("Plateau du joueur :\n");
    for (int i = 0; i < NBR_MAX_TUILE_PAR_PLATEAU; i++) {
        if (plateau[i] != 0) {
            printf("%d ", plateau[i]);
        } else {
            printf("- ");
        }
    }
    printf("\n");
}
