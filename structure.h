/**
 * @file structure.h
 * @brief Fichier d'en-tête pour les structures de données du jeu.
 */

#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#define MAX_PSEUDO 256
#define MAX_TEXT 256
#define NBR_MAX_TUILE_PAR_PLATEAU 20

#define INSCRIPTION_REQUEST 10
#define INSCRIPTION_OK 11
#define INSCRIPTION_KO 12
#define START_GAME 13
#define CANCEL_GAME 14

/**
 * @struct Joueur
 * @brief Structure pour un joueur.
 *
 * Cette structure contient le pseudo du joueur, le descripteur de socket pour la communication avec le joueur,
 * le plateau de jeu du joueur, le score du joueur et le nombre de coups du joueur.
 */
typedef struct Joueur
{
  char pseudo[MAX_PSEUDO];                /**< Le pseudo du joueur. */
  int sockfd;                             /**< Le descripteur de socket pour la communication avec le joueur. */
  int plateau[NBR_MAX_TUILE_PAR_PLATEAU]; /**< Le plateau de jeu du joueur. */
  int score;                              /**< Le score du joueur. */
  int shot;                               /**< Le nombre de coups du joueur. */
} Joueur;

/**
 * @struct StructMessage
 * @brief Structure pour un message.
 *
 * Cette structure contient le texte du message et un code indiquant le type de message.
 */
typedef struct StructMessage
{
  char messageText[MAX_PSEUDO]; /**< Le texte du message. */
  int code;                     /**< Le code indiquant le type de message. */
} StructMessage;

#endif
