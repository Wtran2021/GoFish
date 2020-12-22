#ifndef GOFISH_H
#define GOFISH_H
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "deck.h"
#include "card.h"

/*
   Define any prototype functions
   for gofish.h here.
*/

/* Function: print_hand
 * ---------------------------
 *  Print all the cards in a players hand 
 *
 *  target: pointer to the player to be printed
 *
 *  returns: N/A
 */
void print_hand(struct player* target);

/* Function: print_books
 * ---------------------------
 *  Print all the books a player has
 *
 *  target: pointer to the player to be printed
 *
 *  returns: N/A
 */
void print_books(struct player* target);

/* Function: print_turn
 * ---------------------------
 *  Print all the cards a player has of a certain rank
 *
 *  target: pointer to the player to be printed
 *	rankR: char for rank of card(s)
 *
 *  returns: N/A
 */
void print_turn(struct player* target, char rankR);

/* Function: card_count
 * ---------------------------
 *  count all the cards a player has of a certain rank
 *
 *  target: pointer to the player to be printed
 *	rankR: char for rank of card(s)
 *
 *  returns: int for num of card of rankR
 */
int card_count(struct player* target, char rankR);

#endif
