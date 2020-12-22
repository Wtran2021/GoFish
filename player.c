#include "player.h"
#include <stdio.h>

/* Function: add_card
 * -------------------
 *  Add a new card to the player's hand. 
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card){
	struct hand* newLLCard; //new linked list entry for hand
	newLLCard = (struct hand*)malloc(sizeof(struct hand));
	
	if(newLLCard == NULL){
		return -1;
	}
	newLLCard->top = *new_card;	//https://stackoverflow.com/questions/2575048/arrow-operator-usage-in-c
	newLLCard->next = target->card_list; //next points to current linked list
	target->card_list = newLLCard; //now newLLCard is most recent LL
	
	//did we form a book
	//char book = check_add_book(target); //check for books
	check_add_book(target); //check for books
	return 0;
}



/* Function: remove_card
 * ---------------------
 *  Remove a card from the player's hand. 
 *
 *  target: the target player
 *  old_card: pointer to the old card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 *///joshuateixei Remove
int remove_card(struct player* target, struct card* old_card){
	struct hand* temp = target->card_list; //temp LL to step through 
	struct hand* previous = NULL; //remember previous card
	
	if(temp == NULL){
		return -1; //if hand is empty, we cant remove card
	}
	// while we havnt found a card that is both the same suit and same rank as old...
	while( (temp->top.rank[0] != old_card->rank[0]) || (temp->top.suit != old_card->suit) ){
		previous = temp; //remem previous card
		temp = temp->next; //now next card
		if( temp == NULL ){
			return -1; //card not found
		}
	}		
	if( previous != NULL ){
		previous->next = temp->next; //now prev card points to one after old_card, skipping it
	}else{
		target->card_list = temp->next; //there is no prev card, just 'remove' first one
	}
	free(temp); //free spot from removed card in mem
	return 0;
}



/* Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 *  target: pointer to the player to check
 *  
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 *///wtran check_add_book
char check_add_book(struct player* target){
	
	struct hand* oldHand = target->card_list;
	struct card removeCard;
	char removeRank;

	
	int cardList[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //create master list for each card rank, count at 0
	char cardListChar[14] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
	
	while(oldHand != NULL){
		if(oldHand->top.rank[0] == '2') {
			cardList[0]++;
		}else if (oldHand->top.rank[0] == '3') {
			cardList[1]++;
		}else if (oldHand->top.rank[0] == '4') {
			cardList[2]++;
		}else if (oldHand->top.rank[0] == '5') {
			cardList[3]++;
		}else if (oldHand->top.rank[0] == '6') {
			cardList[4]++;
		}else if (oldHand->top.rank[0] == '7') {
			cardList[5]++;
		}else if (oldHand->top.rank[0] == '8') {
			cardList[6]++;
		}else if (oldHand->top.rank[0] == '9') {
			cardList[7]++;
		}else if (oldHand->top.rank[0] == 'T') {
			cardList[8]++;
		}else if (oldHand->top.rank[0] == 'J') {
			cardList[9]++;
		}else if (oldHand->top.rank[0] == 'Q') {
			cardList[10]++;
		}else if (oldHand->top.rank[0] == 'K') {
			cardList[11]++;
		}else if (oldHand->top.rank[0] == 'A') {
			cardList[12]++;
		}
		
		oldHand = oldHand->next;
	}//close while
	int i;
	for(i=0; i<13; i++){
	    if(cardList[i] == 4){
		removeRank = cardListChar[i]; //set the rank to remove
		removeCard.rank[0] = removeRank;
	        removeCard.suit = 'C';
	        remove_card(target, &removeCard);
	        removeCard.suit = 'D';
	        remove_card(target, &removeCard);
	        removeCard.suit = 'H';
	        remove_card(target, &removeCard);
	        removeCard.suit = 'S';
	        remove_card(target, &removeCard);
			     
	        for(i=0; i<7; i++){
		    if(target->book[i]==0){
		        target->book[i]=removeRank;
		        return removeRank; //added book
		        //break;
		    }//close if
	        }//close for
	    }//close if
	}//close for
	return 0; //no book added
}



/* Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *  
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 *///wtran search
int search(struct player* target, char rank){
	struct hand* temp = target->card_list;
	
	while(temp != NULL){
		if(temp->top.rank[0] == rank){
			return 1;
		}else if (temp->next == NULL){
			break;
		}
		temp = temp->next;
	}
	return 0;
	
}



/* Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's 
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *   
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise 
 *   return value indicates number of cards transferred
 */   
int transfer_cards(struct player* src, struct player* dest, char rank){
	if(src->card_list == NULL){
		return -1; //no cards transfered bc src has no cards at all...
	}
	int numCards = 0; //number of cards transferred
	struct hand* srcHand = src->card_list; //source hand to look through
	
	while( srcHand != NULL ){ //transfer all cards of rank "rank"
		if( srcHand->top.rank[0] == rank ){
			add_card(dest, &srcHand->top);
			remove_card(src, &srcHand->top);
			numCards++;
		}
		srcHand = srcHand->next;
	}
	return numCards; //return number of cards transfered (0 if none)
}



/* Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *   
 *   Return: 1 if game is over, 0 if game is not over
 *///joshuateixei game_over
int game_over(struct player* target){
	int numBooks = 0;
	int i;
	for( i=0; i<7; i++){
		if(target->book[i] != 0){
			numBooks++;
		}
	}
	if(numBooks >=7 ){
		return 1;
	}else{
		return 0;
	}
}



/*  Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 * 
 *   target: player to reset
 * 
 *   Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target) {
	int i,j;
	//struct deck resetDeck;
	for(i = 0; i < 7; i++){ //reset books
		target->book[i] = 0;
	}
	if( target->card_list == NULL){
		return 0; //already no cards in hand
	}
	for(j=0; j<52; j++){
		deck_instance.list[j].suit = 0;	
	}
	while( target->card_list != NULL ){
		remove_card(target, &(target->card_list->top));
	}
	//deck_instance = resetDeck;
	return 0;	
}



/* Function: computer_play
 * -----------------------
 *
 *   Select a rank randomly to play this turn. The player must have at least
 *   one card of the selected rank in their hand.
 *
 *   target: the player's hand to select from
 *
 *   Rank: return a valid selected rank
 */
char computer_play(struct player* target){
	if( target->card_list == NULL ){
		return 0; //no cards to pick...
	}
	char rank = '0'; //rank to return
	while( search(target, rank) != 1 ){
		int randN = (rand() % 13)+2; //[2,14]
		if( randN < 10 ){
			rank = '0' + randN;
		}else{
			switch(randN){
				case 10:
					rank = 'T';
					break;
				case 11:
					rank = 'J';
					break;
				case 12:
					rank = 'Q';
					break;
				case 13:
					rank = 'K';
					break;
				case 14:
					rank = 'A';
					break;
				default:
					printf("This shouldnt print... error in computer_play");	
			}
		}
	}
	return rank;
}



/*  Function: user_play
 * -------------------
 *
 *   Read standard input to get rank user wishes to play.  Must perform error
 *   checking to make sure at least one card in the player's hand is of the 
 *   requested rank.  If not, print out "Error - must have at least one card from rank to play"
 *   and then re-prompt the user.
 *
 *   target: the player's hand to check
 * 
 *   returns: return a valid selected rank
 */
char user_play(struct player* target){
    if( target->card_list == NULL ){
        return 0; //no cards to pick...
    }
    int flag = 1;
    char string[50] = {'0'};
    char input;

    while( flag ){
        string[0] = '0';
		scanf("%s", string);

        if (sizeof(string)>50){
            printf("Whyd you try so many chars? Now the program will not work... \n");
            continue;
        }
        if (string[0] == '1' && string[1]== '0'){ // if second char is not empty (if they say 10)
            input = 'T';
        }else {
            input = string[0]; 
        }
        if(search(target, input)){
            break;
        }
        printf("Error - must have at least one card from rank to play \n"); //if search unsuccessful
        continue;
    }
    return input;
}


