	#include "deck.h"
	#include <time.h> //use srand(time(NULL)); for random games

	#define RANDSEED 1
//--------------BEGIN FUNCTIONS----------------

/* Function: shuffle
 * --------------------
 *  Initializes deck_instance and shuffles it.
 *  Resets the deck if a prior game has been played.
 * 
 *  returns: 0 if no error, and non-zero on error
 */ 
	int shuffle( ){
		int s, r, rNum;    //Suit and rank incrementors

		char suit[5] = { 'C', 'D', 'H', 'S'};
		char rank;
		deck_instance.top_card = 51; //deck is currently full...51 = 52 cards in deck

		//Initialize all members of suits in order
		for(s=0; s<4; s++){
		//Initialize all ranks in order
			for(r=2; r<15; r++){
				if(r < 10){
					rank='0' + r; //ascii
				}
				else{
					switch(r){
						case 10:
							rank = 'T'; //ten
							break;
						case 11:
							rank = 'J'; //jack
							break;
						case 12:
							rank = 'Q'; //queen
							break;
						case 13:
							rank = 'K'; //king
							break;
						case 14:
							rank = 'A'; //ace
							break;
						default:
							printf("This shouldnt show up..."); //uh oh
							break;  
					}//close switch
				}//close else
			rNum = cleanRand(r*(s+1)); //try storing in location r of iteration s
			deck_instance.list[rNum].suit = suit[s]; //assign suit value
			deck_instance.list[rNum].rank[0] = rank; //assigns rank value to rank array
			
			}//close rank for loop
		}//close suit for loop	
/* ------ DEBUG --------

int bug;
for(bug=0; bug < 51; bug++){
	printf("%c %c \n" , deck_instance.list[bug].suit, deck_instance.list[bug].rank[0]);
}
	
------ END DEBUG -------- */  
  return 0;
}//close shuffle

/* Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck. 
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card( ){
	if( deck_instance.top_card < 0 ){ //if deck is empty
		return NULL;
	}else{
		return (&deck_instance.list[deck_instance.top_card--]);
	}
}

/* Function: size
 * --------------
 *  Return the number of cards left in the current deck.
 *
 *  returns: number of cards left in the deck.
 */
size_t deck_size( ){
	return (deck_instance.top_card);
}

/* Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck. 
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */
int deal_player_cards(struct player* target) {
	int i;
	for(i=0; i<7; i++){ //deal 7 cards, [0,6]
		if( deck_size()<0 ){ //cant deal card if deck empty
			return -1;
		}
		add_card(target, next_card());
	}
	return 0;
}


/* Function: cleanRand
 * -------------------
 * Return a rand index in deck.list [0, 51] that isnt taken already
 * (should give us a rand num without collision for shuffling)
 * returns: ^^^
 */
int cleanRand(int randN){
	//srand(time(NULL)); //for different shuffle every time
	//srand(RANDSEED); //for the same shuffle every time
	if( (randN > -1) && (randN < 52) && (deck_instance.list[randN].suit == 0) ){
		return randN;
	}else{
		randN = rand() % 52; //random num [0, 51]
		return (cleanRand(randN));
	}
	
}
