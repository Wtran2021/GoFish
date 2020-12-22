#include <stdio.h>
#include <stdlib.h>
#include "gofish.h"


int main(int args, char* argv[]) 
{
	//fprintf(stdout, "Put your code here.");
	int replay = 1; //for when the user selects play again or no (0)
	int userturn = 1; //user goes first
  
	while(replay){//keep playing while replay is true
	
		//Game set up...
		userturn = 1; //for replay, reset turn to user
	  
		//set each player's books to empty again...
		reset_player(&user);
		reset_player(&computer);
	  
		shuffle(); //set up deck
		printf("Shuffling deck...\n"); //from sample output...
		
		//deal a hand to each player
		deal_player_cards(&user);
		deal_player_cards(&computer);
		
		
		//Turns code
		//while neither player has one, continue to take turns...
		while( !(game_over(&user)) && !(game_over(&computer)) ){ 
			printf( "Player 1's Hand - " ); //from sample output
			print_hand(&user);
			printf( "Player 1's Book - " );
			print_books(&user);
			//
			//printf( "Player 2's Hand - " ); //debug~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//print_hand(&computer);
			//
			printf( "Player 2's Book - " );
			print_books(&computer);
			///////////////////////////////
			//Player 1: User's Turn----------------------------------------------------
			if(userturn){
				if( user.card_list == NULL){
					printf( "Player 1 has no cards. They may draw one and continue their turn\n" ); //https://bicyclecards.com/how-to-play/go-fish/
					add_card(&user, next_card());
					continue;
				}
				printf( "Player 1's turn, enter a Rank:\t" );
				char userplay = user_play(&user); //returns a checked, valid, user input
				// If successful turn
				if( search(&computer, userplay) ){ //if player 2 has what player 1 asked
					printf( "\t-Player 2 has " );
					print_turn(&computer, userplay);
					//if a book is formed \/
					if( (card_count(&user, userplay) + card_count(&computer, userplay)) == 4 ){
						printf( "\t-Player 1 has " );
						print_turn(&user, userplay);
						if( userplay == 'T' ){
							printf( "Player 1 books 10\n" ); //actual booking is in add_card
						}else{
							printf( "Player 1 books  %c\n" );
						}
						printf( "Player 1 gets another turn\n\n" );
						transfer_cards(&computer, &user, userplay); //(src, dest, rank);
						userturn = 1; //still player 1, user's, turn
						continue; //next iter of game loop
					}
					transfer_cards(&computer, &user, userplay);
					userturn = 0;
					printf( "\t-Player 2's turn\n\n" );
					continue;
				}//if unsuccessful turn
				else{
					if( userplay == "T" ){
						printf( "\t-Player 2 has no 10's\n" );
					}else{
						printf( "\t-Player 2 has no  %c's\n", userplay);
					}
					char newCardSuit = deck_instance.list[deck_instance.top_card].suit;
					char newCardRank = deck_instance.list[deck_instance.top_card].rank[0];
					add_card(&user, next_card());
					if( newCardRank == 'T' ){
						printf( "\t-Go fish. Player 1 draws 10%c\n\n", newCardSuit);
					}else{
						printf( "\t-Go fish. Player 1 draws %c%c\n\n", newCardRank, newCardSuit);
					}
					//if player forms a book with the card they drew
					if( card_count(&user, newCardRank) == 4){
						if( newCardRank == 'T' ){
							printf( "\t-Player 1 books 10\n" );
						}else{
							printf( "\t-Player 1 books %c\n", newCardRank);
						}
						printf( "\t-Player 1 gets another turn\n\n" );
						userturn = 1;
						continue;
					}
					//if player draws what they guessed, they get a free turn i guess
					if( newCardRank == userplay ){
						printf( "\t-Player 1 has drawn the card they guessed, they get a free turn\n\n" );
						userturn = 1;
						continue;
					}
					userturn = 0;
					continue;
				}
				
			}//end user turn section
			//Player 2: Computer's Turn--------------------------------------------------
			else if(!userturn){
				if( &computer.card_list == NULL){
					printf( "Player 2 has no cards. They may draw one and continue their turn\n" ); //https://bicyclecards.com/how-to-play/go-fish/
					add_card(&computer, next_card());
					continue;
				}
				printf( "Player 2's turn, enter a Rank:\t" );
				char compplay = computer_play(&computer);
				if( compplay == 'T' ){
					printf( " 10\n" );
				}else{
					printf( "  %c\n", compplay );
				}
				
				// If successful turn
				if( search(&user, compplay) ){ //if player 1 has what player 2 asked
					printf( "\t-Player 1 has " );
					print_turn(&user, compplay);
					//if a book is formed \/
					if( (card_count(&computer, compplay) + card_count(&user, compplay)) == 4 ){
						printf( "\t-Player 2 has " );
						print_turn(&computer, compplay);
						if( compplay == 'T' ){
							printf( "Player 2 books 10\n" ); //actual booking is in add_card
						}else{
							printf( "Player 2 books  %c\n" );
						}
						printf( "Player 2 gets another turn\n\n" );
						transfer_cards(&user, &computer, compplay); //(src, dest, rank);
						userturn = 0; //still player 2, computer's, turn
						continue; //next iter of game loop
					}
					transfer_cards(&user, &computer, compplay);
					userturn = 1; //back to p1
					printf( "\t-Player 1's turn\n\n" );
					continue;
				}//if unsuccessful turn -----------
				else{
					if( compplay == "T" ){
						printf( "\t-Player 1 has no 10's\n" );
					}else{
						printf( "\t-Player 1 has no  %c's\n", compplay);
					}
					char newCardSuit = deck_instance.list[deck_instance.top_card].suit;
					char newCardRank = deck_instance.list[deck_instance.top_card].rank[0];
					add_card(&computer, next_card());
					if( newCardRank == 'T' ){
						//printf( "\t-Go fish. Player 2 draws 10%c\n\n", newCardSuit); //for debug~~~~~~~~~~~~~~~~~~
						printf( "\t-Go fish. Player 2 draws a card.\n\n"); //final code
					}else{
						//printf( "\t-Go fish. Player 2 draws  %c%c\n\n", newCardRank, newCardSuit); //for debug~~~~~~~~~~~~~~~~~~
						printf( "\t-Go fish. Player 2 draws a card.\n\n"); //final code
					}
					//if player forms a book with the card they drew
					if( card_count(&computer, newCardRank) == 4){
						if( newCardRank == 'T' ){
							printf( "\t-Player 2 books 10\n" );
						}else{
							printf( "\t-Player 2 books  %c\n", newCardRank);
						}
						printf( "\t-Player 2 gets another turn\n\n" );
						userturn = 0;
						continue;
					}
					//if player draws what they guessed, they get a free turn i guess
					if( newCardRank == compplay ){
						printf( "\t-Player 2 has drawn the card they guessed, they get a free turn\n\n" );
						userturn = 0;
						continue;
					}
					userturn = 1;
					continue;
				}//end else (unsuccessful turn)
				
			
				
			}//end player 2
			else{
				printf("This should never be executed, error in turn boolean (int named userturn)");
			}
		
		}
	  
	  
		//Game end code
		//if either player has reached win conditon
		if( (game_over(&user)) || (game_over(&computer)) ){  
			int userB = 0; int compB = 0; int i = 0;
			//numbers for the required score print
			for(i=0; i<7; i++){
				if( user.book[i] != 0 ){
					userB++; 
				}
				if( computer.book[i] != 0 ){
					compB++;
				}
			}
			//order of print output
			if( game_over(&user) ){
				printf( "Player 1 Wins! %d-%d\n", userB, compB);
			}else{
				printf( "Player 2 Wins! %d-%d\n", compB, userB);
			}
			char playAgainChar[2] = {'0'}; //for user input
			while(1){
				printf( "Do you want to play again [Y/N]\t" ); 
				scanf( "%s", playAgainChar);
				if( sizeof(playAgainChar) > 2 ){
					printf( "Please only enter one char, Y or N\nTry Again\n" );
				} else if( (playAgainChar[0] == 'Y') || (playAgainChar[0] == 'y') ){
					replay = 1;
					break;
				} else if( (playAgainChar[0] == 'N') || (playAgainChar[0] == 'n') ){
					replay = 0; //likely not necessary..?
					printf("Exiting.\n"); //from sample output
					exit(0); //exit program
				} else {
					printf("Please enter a valid char.");
				}
			}	
		}
		
	}
	  
	return 0;
  }//close main
  
  /* Function: card_count
 * ---------------------------
 *  count all the cards a player has of a certain rank
 *
 *  target: pointer to the player to be printed
 *	rankR: char for rank of card(s)
 *
 *  returns: int for num of card of rankR
 */
int card_count(struct player* target, char rankR){
	struct hand* temp = target->card_list;
	int cardCount = 0;
	while( temp != NULL ){
		if( temp->top.rank[0] == rankR ){
			cardCount++;
		}
		temp = temp->next; //advance next card
	}
	return cardCount;
}
 
  /* Function: print_turn
 * ---------------------------
 *  Print all the cards a player has of a certain rank
 *
 *  target: pointer to the player to be printed
 *	rank: char for rank of card(s)
 *
 *  returns: N/A
 */
void print_turn(struct player* target, char rankR){
	struct hand* temp = target->card_list;
	while( temp != NULL ){
		if( temp->top.rank[0] == rankR ){//problem here?
			if( rankR == 'T' ){
				printf( "10%c  ", temp->top.suit );
			}else{
				printf( " %c%c  ", temp->top.rank[0], temp->top.suit );
			}
		}
		temp = temp->next; //advance next card
	}
	printf( "\n" );
	return;
}



/* Function: print_hand
 * ---------------------------
 *  Print all the cards in a players hand 
 *
 *  target: pointer to the player to be printed
 *
 *  returns: N/A
 */
void print_hand(struct player* target){
	struct hand* temp = target->card_list; //hand to print
	while( temp != NULL ){
		if( temp->top.rank[0] == 'T' ){
			printf( "10%c ", temp->top.suit );
		} else {
			printf( " %c%c ", temp->top.rank[0], temp->top.suit );
		}
		temp = temp->next; //advance
	}//close while
	printf( "\n" );
}//close print_hand



/* Function: print_books
 * ---------------------------
 *  Print all the books a player has
 *
 *  target: pointer to the player to be printed
 *
 *  returns: N/A
 */
void print_books(struct player* target){
	int i;
	for(i=0; i<7; i++){
		if(target->book[i]!=0){
				if(target->book[i]=='T'){
					printf( "10 " );
				}else{
					printf( " %C ", target->book[i] );
				}
		}else{
			printf( "\n" );
			return; //once we reach end of recorded books, no need to keep trying
		}
	}
	printf( "\n" );
	return;
}
