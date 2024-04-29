#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"

#define SCORES_SIZE 20
const int SCORES[SCORES_SIZE] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};

void creerEnsembleTuiles(int *ensembleTuiles)
{
    // Création des tuiles numérotées de 1 à 30 (avec doublons pour 11-19) et un joker (31)
    int index = 0;
    for (int i = 1; i <= 30; i++)
    {
        ensembleTuiles[index++] = i;
        if (i >= 11 && i <= 19)
        {
            ensembleTuiles[index++] = i; // Doublon pour 11-19
        }
    }
    ensembleTuiles[index] = 31; // Joker

    // Mélange des tuiles avec l'algorithme de Fisher-Yates
    srand(time(NULL));
    for (int i = NBR_MAX_TUILE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = ensembleTuiles[i];
        ensembleTuiles[i] = ensembleTuiles[j];
        ensembleTuiles[j] = temp;
    }
}

void initierPlateauJoueur(Joueur *joueur)
{
    // Initialisation du plateau du joueur avec des valeurs par défaut (0)
    for (int i = 0; i < NBR_MAX_TUILE_PAR_PLATEAU; i++)
    {
        joueur->plateau[i] = 0;
    }
}

int tirerTuile(int *ensembleTuiles, int *tailleLogique)
{
    if (*tailleLogique <= 0)
    {
        return -1; // Si le nombre de tuiles est insuffisant
    }

    // Tirage de la dernière tuile de l'ensemble
    int tuile = ensembleTuiles[*tailleLogique - 1];
    (*tailleLogique)--;

    return tuile;
}

bool placerTuile(int *plateau, int tuile)
{

    char resultat[4]; // 2 chiffres + \n + \0
    printf("Veuillez introduire la position souhaitée :\n");
    int ret = readLimitedLine(resultat, 4);
    if (ret == -1)
    {
        return false;
    }

    int position = atoi(resultat) - 1;
    printf("\nposition choisi : %d", position);

    if (position < 0 || position >= NBR_MAX_TUILE_PAR_PLATEAU)
    {
        printf("Position invalide.\n");
        return false;
    }

    // Si la position est déjà occupée, la tuile sera placée à droite de la position demandée
    while (plateau[position] != 0)
    {
        position++;
        if (position >= NBR_MAX_TUILE_PAR_PLATEAU)
        {
            position = 0;
        }
    }

    plateau[position] = tuile;
    return true;
}

int calculerScore(const int *plateau)
{
    int longueurSuite = 0;
    int scoreTotal = 0;

    for (int i = 1; i < NBR_MAX_TUILE_PAR_PLATEAU; i++)
    {
        int tuilePrecedente = plateau[i - 1];
        int tuile = plateau[i];

        if (tuile == 0)
        {
            tuile = tuilePrecedente + 1;
        }

        if (tuilePrecedente == 0)
        {
            tuilePrecedente = tuile - 1;
        }
        
        if (tuile > tuilePrecedente)
        {
            longueurSuite++;
        }
        else
        {
            scoreTotal += SCORES[longueurSuite - 1]; // -1 car les indices commencent à 0
            longueurSuite = 0;
        }
    }

    // Ajouter le score de la dernière suite
    if (longueurSuite > 0)
    {
        scoreTotal += SCORES[longueurSuite - 1]; // -1 car les indices commencent à 0
    }

    return scoreTotal;
}

void afficherPlateau(const int *plateau)
{
    printf("Plateau du joueur :\n");
    for (int i = 0; i < NBR_MAX_TUILE_PAR_PLATEAU; i++)
    {
        if (plateau[i] != 0)
        {
            printf("%d ", plateau[i]);
        }
        else
        {
            printf("- ");
        }           
    }
    printf("\n");
}
