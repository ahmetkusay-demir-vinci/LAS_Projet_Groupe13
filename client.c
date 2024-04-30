#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "structure.h"
#include "utils_v1.h"
#include "network.h"
#include "jeu.h"


void inverserTableau(char **tableau, int taille);

int main(int argc, char *argv[])
{
	Joueur joueur;
	char pseudo[MAX_PSEUDO];
	int socketJoueur;
	int ret;
	char nomDuFichier[MAX_TEXT];
	char **tableauFichier;
	bool presenceDUnFichier = false;
	int tailleLogique = 0;

	StructMessage msg;

	if (argc == 3)
	{
		strcpy(nomDuFichier, argv[2]);

		int fd = sopen(nomDuFichier, O_RDONLY, 0200); // O_RDWR pour lecture et écriture

		if (fd == -1)
		{
			perror("Erreur lors de l'ouverture du fichier");
			exit(0);
		}
		tableauFichier = readFileToTable(fd);
		tailleLogique = NBR_MAX_TUILE_PAR_PLATEAU;
		// Inverser le tableau
    	inverserTableau(tableauFichier, tailleLogique);

		presenceDUnFichier = true;
	}

	/* retrieve player name */
	printf("Bienvenue dans le programe d'inscription au serveur de jeu\n");
	printf("Pour participer entrez votre nom :\n");
	if (presenceDUnFichier == false)
	{
		ret = sread(0, pseudo, MAX_PSEUDO);
		checkNeg(ret, "read client error");
		pseudo[ret - 1] = '\0';
	}else{
		strcpy(pseudo,tableauFichier[NBR_MAX_TUILE_PAR_PLATEAU]);
		tailleLogique--;
	}

	strcpy(msg.messageText, pseudo);
	msg.code = INSCRIPTION_REQUEST;

	socketJoueur = initSocketClient(SERVEUR_IP, atoi(argv[1]));

	swrite(socketJoueur, &msg, sizeof(msg));

	/* wait server response */
	sread(socketJoueur, &msg, sizeof(msg));

	switch (msg.code)
	{
	case INSCRIPTION_OK:
		printf("Réponse du serveur : Inscription acceptée\n");
		sread(socketJoueur, &joueur, sizeof(Joueur));
		printf("Joueur %s \n\n", joueur.pseudo);
		break;
	case INSCRIPTION_KO:
		printf("Réponse du serveur : Inscription refusée\n");
		sclose(socketJoueur);
		exit(0);
	default:
		printf("Réponse du serveur non prévue %d\n", msg.code);
		break;
	}

	/* wait start of game or cancel */
	sread(socketJoueur, &msg, sizeof(msg));

	if (msg.code == START_GAME)
	{
		printf("DEBUT JEU\n");
		initierPlateauJoueur(&joueur);

		int tuile;
		bool encours = true;

		while (encours)
		{

			sread(socketJoueur, &tuile, sizeof(int));
			if (tuile == -1)
			{
				printf("FIN DES TOURS\n\n");
				encours = false;
				break;
			}

			if (tuile == 31)
			{
				printf("La tuile à placer est le joker\n");
			}
			else
			{
				printf("La tuile à placer est la suivante: %d\n", tuile);
			}
			if (placerTuile(joueur.plateau, tuile, presenceDUnFichier, tableauFichier, &tailleLogique))
			{
				afficherPlateau(joueur.plateau);
				printf("\n");
			}

			bool resultat = true;
			swrite(socketJoueur, &resultat, sizeof(bool));
		}
		int scoreFinal = calculerScore(joueur.plateau);
		printf("Vous avez obtenu un score de : %d \n\n", scoreFinal);
		swrite(socketJoueur, &scoreFinal, sizeof(int));

		Joueur copieClassementFinal[MAX_JOUEURS];
		sread(socketJoueur, copieClassementFinal, sizeof(Joueur) * MAX_JOUEURS);
		printf("\t\t  VOICI LES RESULTATS !\n");
		for (int i = 0; i < MAX_JOUEURS; i++)
		{
			if(strlen(copieClassementFinal[i].pseudo)!=0){
				printf("\t%d => %s avec un score de %d points\n", i + 1, copieClassementFinal[i].pseudo, copieClassementFinal[i].score);
			}
			
		}
	}
	else
	{
		printf("PARTIE ANNULEE\n");
		sclose(socketJoueur);
	}
	return 0;
}

void inverserTableau(char **tableau, int taille) {
    int debut = 0;
    int fin = taille;

    while (debut < fin) {
        // Échange les éléments du début avec ceux de la fin
        char *temp = tableau[debut];
        tableau[debut] = tableau[fin];
        tableau[fin] = temp;
        debut++;
        fin--;
    }	
}