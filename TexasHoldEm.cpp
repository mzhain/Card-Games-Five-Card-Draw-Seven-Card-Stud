// file name: TexasHoldEm.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: six methods to accompany TexasHoldEm objects and compliment the program's functionality
//		includes default constructor

#include "stdafx.h"
#include "TexasHoldEm.h"

//public default constructor - calls the constructor for the Game base class, which does the following:
//	initializes the dealer position member variable to be 0, and then iterates through all of the 52 valid combinations
//	of suit and rank as defined in the Card class enumerations and passes a Card corresponding to each combination into
//	a call to the add_card method of the main deck member variable that was inherited from the Game base class
TexasHoldEm::TexasHoldEm() : Game() {

}


//	NOTE ABOUT TEXASHOLD'EM
//	-----------------------
//	first round: 2 cards dealt (face down)
//		betting phase
//	second round: 3 cards dealt (face up) - THESE ARE SHARED BY ALL PLAYERS (FLOP)
//		betting phase
//	third round: 1 card dealt (face up) - SHARED BY ALL PLAYERS (TURN)
//		betting phase
//	fourth round: 1 card dealt (face up) - SHARED BY ALL PLAYERS (RIVER)
//		betting phase



//the beforeTurn method should print out the current player's hand (without *'s) and the cards that are shared amongst
//	all of the players
int TexasHoldEm::beforeTurn(Player &p) {
	// if the player has less than zero chips, that means they've folded, so they don't to go through this method
	if (p.chips < 0) {
		return success;
	}
	vector<int> cardsToHide;
	//first we have to go through and set all of the current player's cards to be shown (because we're printing to them)
	for (int i = 0; i < p.hand.getHandSize(); ++i) {
		if (p.hand.myHand[i].show == false) {
			p.hand.myHand[i].show = true;
			//save the places of the cards that were hidden beforehand
			cardsToHide.push_back(i);
		}
	}
	cout << endl << "THIS IS WHAT " << p.name << " SEES BEFORE BETTING:" << endl << endl;
	//then print out the current player's hand
	cout << p.name << "'s Hand: " << p.hand << endl;
	//then print out the cards that are shared amongst all of the players
	cout << "Shared Cards: ";
	for (size_t i = 0; i < sharedCards.size(); ++i) {
		cout << (sharedCards[i]).cardToString() << " ";
	}
	cout << endl << endl;
	//finally, reset the cards of the current player to be hidden
	for (size_t i = 0; i < cardsToHide.size(); ++i) {
		p.hand.myHand[cardsToHide[i]].show = false;
	}
	return success;
}



//the turn method doesn't need to actually do anything in our implementation of TexasHoldEm,
//	so it can simply return success
int TexasHoldEm::turn(Player &p) {
	return success;
}



//the beforeRound method calls the shuffle method on the mainDeck, then collects one coin ante (forced bet) from every
//	player in the game, then deals the first two cards to each player (both face down), calls every players' beforeTurn
//	method to print out what each of them will see before betting, and then runs through the first round of betting
int TexasHoldEm::beforeRound() {
	int playersSizeToInt = static_cast<int>(players.size());
	//first, shuffle the main deck
	mainDeck.shuffleDeck();
	//if the players vector is somehow empty, we don't need to do anything in this method
	if (playersSizeToInt == 0) {
		return success;
	}
	int dealerAt = currentDealerPosition + 1;

	//if the current position of the dealer is somehow out of the bounds of the players vector, return unsuccessfulRun
	if (dealerAt > playersSizeToInt || dealerAt < 0) {
		return unsuccessfulRun;
	}

	//the players each give one chip (an ante (forced bet)) at the beginning of the round   LAB4
	for (int i = 0; i < playersSizeToInt; ++i) {
		(*players[i]).chips = (*players[i]).chips - 1;
		potSize++;
	}


	//run through the first round of dealing cards (two face down)

	//before starting to deal cards to the sharedCards vector, make sure it is cleared out
	sharedCards.clear();


	//FIRST ROUND: EVERYONE IS DEALT TWO CARDS, BOTH FACE DOWN
	//if the dealer is at the end of the players vector, the dealer will start dealing cards to the first player
	//	in the vector
	if (dealerAt == playersSizeToInt) {
		for (int j = 0; j < playersSizeToInt; ++j) {
			//check if the player has 0 or more chips - only deal cards to them if they have 0 or more chips
			if ((*players[j]).chips >= 0) {
				(*players[j]).hand << mainDeck;
				(*players[j]).hand << mainDeck;
				//make sure that both of the cards that were dealt are marked as "show = false;" aka they are hidden
				(*players[j]).hand.myHand[firstCardInHand].show = false;
				(*players[j]).hand.myHand[secondCardInHand].show = false;
			}
		}
	}
	//if the dealer is not at the end of the players vector, deal to everyone after the dealer in the players
	//	vector and then to everyone before the dealer in the players vector
	else {
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			if ((*players[j]).chips >= 0) {
				(*players[j]).hand << mainDeck;
				(*players[j]).hand << mainDeck;
				//make sure that both of the cards that were dealt are marked as "show = false;" aka they are hidden
				(*players[j]).hand.myHand[firstCardInHand].show = false;
				(*players[j]).hand.myHand[secondCardInHand].show = false;
			}
		}
		for (int k = 0; k < dealerAt; ++k) {
			if ((*players[k]).chips >= 0) {
				(*players[k]).hand << mainDeck;
				(*players[k]).hand << mainDeck;
				//make sure that both of the cards that were dealt are marked as "show = false;" aka they are hidden
				(*players[k]).hand.myHand[firstCardInHand].show = false;
				(*players[k]).hand.myHand[secondCardInHand].show = false;
			}
		}
	}

	//once every player has been dealt their first two cards, call the beforeTurn method for every player
	if (dealerAt == playersSizeToInt) {
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}

	//now that the players have their first two cards, run a round of betting
	potSize = potSize + bettingRound(true);
	return success;
}


//the round method runs through the second, third, and fourth turns of distributing three cards face up to the vector of
//	shared cards that are shared amongst all of the players in the game for the second turn and then one card face up
//	to the sharedCards vector for the third and fourth turns
//	-after every one of these turns, a bettingRound takes place, resulting in four betting rounds total for TexasHoldEm
//		-this includes the bettingRound that takes place at the end of the beforeRound method
int TexasHoldEm::round() {
	int playersSizeToInt = static_cast<int>(players.size());
	int dealerAt = currentDealerPosition + 1;

	//
	//TURN 2: deal three cards to the sharedCards vector, call each player's beforeTurn method, & hold a round of betting
	//

	//remove three cards from the mainDeck and put them into the sharedCards vector of cards
	sharedCards.push_back(mainDeck.remove_card());
	sharedCards.push_back(mainDeck.remove_card());
	sharedCards.push_back(mainDeck.remove_card());

	if (dealerAt == playersSizeToInt) {
		//call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	//betting after third, fourth, & fifth cards are dealt (SHARED CARD)
	potSize = potSize + bettingRound(true);



	//
	//TURN 3: deal one card to the sharedCards vector, call each player's beforeTurn method, & hold a round of betting
	//

	//remove a card from the mainDeck and put it into the sharedCards vector of cards
	sharedCards.push_back(mainDeck.remove_card());

	if (dealerAt == playersSizeToInt) {
		//call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	//betting after sixth card is dealt (SHARED CARD)
	potSize = potSize + bettingRound(true);



	//
	//TURN 4: deal one card to the sharedCards vector, call each player's beforeTurn method, & hold a round of betting
	//

	//remove a card from the mainDeck and put it into the sharedCards vector of cards
	sharedCards.push_back(mainDeck.remove_card());

	if (dealerAt == playersSizeToInt) {
		//call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	//betting after seventh card is dealt (SHARED CARD)
	potSize = potSize + bettingRound(true);


	//CARD DEALING IS NOW FINISHED FOR THIS ROUND OF TEXAXHOLD'EM

	//for every player who hasn't folded, fill the rest of their hand with the cards from the sharedCards vector
	for (int i = 0; i < playersSizeToInt; ++i) {
		if ((*players[i]).hand.getHandSize() == cardsInHandTHE) {
			//fill the rest of the player's hand with the cards from the sharedCards vector
			for (size_t j = 0; j < sharedCards.size(); ++j) {
				(*players[i]).hand.myHand.push_back(sharedCards[j]);
			}
		}
	}

	//find every player's best set of five cards from their hand of seven

	vector<Hand> curPlayerCombinations;
	Hand tmp;
	vector<Hand> bestHands(playersSizeToInt);

	//fill tmp.myHand with random Cards as fillers
	for (int i = 0; i < numCardsInHand; ++i) {
		tmp.myHand.push_back(Card(Card::two, Card::clubs));
	}

	for (int i = 0; i < playersSizeToInt; ++i) {
		if ((*players[i]).hand.getHandSize() == cardsInHandSCS) {
			//calling combinations gets all of the combinations (21) into the curPlayerCombinations vector
			combinations(0, (*players[i]), tmp, curPlayerCombinations, 0, (*players[i]).hand.getHandSize() - 1);
			//sort all of the hands and pick the highest one (last element)
			sort(curPlayerCombinations.begin(), curPlayerCombinations.end(), pokerRankCompare);
			//push back the best hand into the bestHands vector
			bestHands[i] = (curPlayerCombinations[curPlayerCombinations.size() - 1]);
			//replace the seven card hand of the current player with their best five card hand
			(*players[i]).hand = bestHands[i];
			//clear out the curPlayersCombinations vector for the next player
			curPlayerCombinations.clear();
		}
	}

	return success;
}