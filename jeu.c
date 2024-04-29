#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "jeu.h"

#define SCORES_SIZE 20

const int SCORES[SCORES_SIZE] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};
int nbr_Partie = 0;

void creerEnsembleTuiles(int *ensembleTuiles, bool presenceDUnFichier, char *nomDuFichier)
{
    if (presenceDUnFichier == false)
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
    else
    {
        int fd = sopen(nomDuFichier, O_RDONLY, 0200); // O_RDWR pour lecture et écriture

        if (fd == -1)
        {
            perror("Erreur lors de l'ouverture du fichier");
            exit(0);
        }

        char **tableau = readFileToTable(fd);

        int indice = 0;

        for (int i = 20 * nbr_Partie + 19; i >= 20 * nbr_Partie; i--)
        {
            ensembleTuiles[indice] = atoi(tableau[i]);
            indice++;
        }
        nbr_Partie++;

        sclose(fd); // Fermeture du descripteur de fichier après utilisation
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

    if (position < 0 || position >= NBR_MAX_TUILE_PAR_PLATEAU)
    {
        printf("Position invalide !\n");
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

        if (tuile == 31)
        {
            tuile = tuilePrecedente + 1;
        }

        if (tuilePrecedente == 31)
        {
            tuilePrecedente = tuile - 1;
        }

        if (tuile > tuilePrecedente)
        {
            longueurSuite++;
        }
        else
        {
            scoreTotal += SCORES[longueurSuite];
            longueurSuite = 0;
        }
    }

    // Ajouter le score de la dernière suite
    if (longueurSuite > 0)
    {
        scoreTotal += SCORES[longueurSuite];
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
        else if (plateau[i] == 31)
        {
            printf("J ");
        }
        else
        {
            printf("- ");
        }
    }

    printf("\n");
}
