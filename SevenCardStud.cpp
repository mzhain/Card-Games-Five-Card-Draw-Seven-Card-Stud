// file name: SevenCardStud.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: six methods to accompany SevenCardStud objects and compliment the program's functionality
//		includes default constructor

#include "stdafx.h"
#include "SevenCardStud.h"

//public default constructor - calls the constructor for the Game base class, which does the following:
//	initializes the dealer position member variable to be 0, and then iterates through all of the 52 valid combinations
//	of suit and rank as defined in the Card class enumerations and passes a Card corresponding to each combination into
//	a call to the add_card method of the main deck member variable that was inherited from the Game base class
SevenCardStud::SevenCardStud() : Game(), turnNumber{ 2 } {

}

//the beforeTurn method should print out the current player's hand (without *'s) and everyone else's hands (with *'s)
int SevenCardStud::beforeTurn(Player &p) {
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
	//then print out everyone's hands (not just the current player)
	for (size_t i = 0; i < players.size(); ++i) {
		cout << (*players[i]).name << "'s Hand: " << (*players[i]).hand << endl;
	}
	cout << endl;
	//finally, reset the cards of the current player to be hidden
	for (size_t i = 0; i < cardsToHide.size(); ++i) {
		p.hand.myHand[cardsToHide[i]].show = false;
	}

	return success;
}



//the turn method runs through the various turns involved in SevenCardStud
//	ASSUMPTION: we assume that the mainDeck will not run out of Cards
//	first round: 3 cards dealt (two face down & THEN one face up)
//		betting phase
//	second round: 1 card dealt (face up)
//		betting phase
//	third round: 1 card dealt (face up)
//		betting phase
//	fourth round: 1 card dealt (face up)
//		betting phase
//	fifth round: 1 card dealt (face down)
//		betting phase
//
//the turn method should only deal cards to the ONE player that is passed in
//shoutout to Puneet for listening to my thoughts on this method and helping me come up with a better solution
int SevenCardStud::turn(Player &p) {

	// if the player has less than zero chips, that means they've folded, so they don't to go through this method
	if (p.chips < 0) {
		return success;
	}
	int dealerAt = currentDealerPosition + 1;

	//if we're on the fifth turn, deal the card face down
	if (turnNumber == 5) {
		//FIFTH ROUND: EVERYONE IS DEALT ONE CARD FACE DOWN
		for (int i = 0; i < SCSotherTurns; ++i) {
			if (p.chips >= 0) {
				p.hand << mainDeck;
				p.hand.myHand[seventhCardInHand].show = false;
			}
		}
	}
	//for all other rounds, deal the card face up (shown) as normal
	else {
		//SECOND, THIRD, & FOURTH ROUND: EVERYONE IS DEALT ONE CARD FACE UP
		for (int i = 0; i < SCSotherTurns; ++i) {
			if (p.chips >= 0) {
				p.hand << mainDeck;
			}
		}
	}

	return success;
}



//the beforeRound method calls the shuffle method on the mainDeck, then collects one coin ante (forced bet) from every
//	player in the game, then deals the first three cards to each player, calls every players' beforeTurn method
//	to print out what each of them will see before betting, and then runs through the first round of betting
int SevenCardStud::beforeRound() {
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


	//run through the first round of dealing cards (one face up & two face down)


	//FIRST ROUND: EVERYONE IS DEALT THREE CARDS, TWO FACE DOWN & ONE FACE UP
	//if the dealer is at the end of the players vector, the dealer will start dealing cards to the first player
	//	in the vector
	if (dealerAt == playersSizeToInt) {
		for (int j = 0; j < playersSizeToInt; ++j) {
			//check if the player has 0 or more chips - only deal cards to them if they have 0 or more chips
			if ((*players[j]).chips >= 0) {
				(*players[j]).hand << mainDeck;
				(*players[j]).hand << mainDeck;
				(*players[j]).hand << mainDeck;
				//make sure that the first two cards that were dealt are marked as "show = false;" aka they are hidden
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
				(*players[j]).hand << mainDeck;
				//make sure that the first two cards that were dealt are marked as "show = false;" aka they are hidden
				(*players[j]).hand.myHand[firstCardInHand].show = false;
				(*players[j]).hand.myHand[secondCardInHand].show = false;
			}
		}
		for (int k = 0; k < dealerAt; ++k) {
			if ((*players[k]).chips >= 0) {
				(*players[k]).hand << mainDeck;
				(*players[k]).hand << mainDeck;
				(*players[k]).hand << mainDeck;
				//make sure that the first two cards that were dealt are marked as "show = false;" aka they are hidden
				(*players[k]).hand.myHand[firstCardInHand].show = false;
				(*players[k]).hand.myHand[secondCardInHand].show = false;
			}
		}
	}

	//once every player has been dealt their first three cards, call the beforeTurn method for every player
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

	//now that the players have their first three cards, run a round of betting
	potSize = potSize + bettingRound(true);
	return success;
}


//the round method calls each player's beforeTurn and turn methods - this process happens four times, once for each of
//	the times that one card is dealt (turn 2, 3, 4, & 5) and also calls bettingRound four times, once per turn & afterTurn
int SevenCardStud::round() {
	int playersSizeToInt = static_cast<int>(players.size());
	int dealerAt = currentDealerPosition + 1;


	//
	//TURN 2: call turn and THEN beforeTurn for every player
	//

	if (dealerAt == playersSizeToInt) {
		//call turn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			//because our turn method only return success or notEnoughCards, if it isn't successful, then we can
			//	return notEnoughCards
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//first call the turn method for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		for (int k = 0; k < dealerAt; ++k) {
			if (turn(*players[k]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	turnNumber++;
	//betting after fourth card is dealt
	potSize = potSize + bettingRound(true);


	//
	//TURN 3: call turn and afterTurn for every player
	//

	if (dealerAt == playersSizeToInt) {
		//call turn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			//because our turn method only return success or notEnoughCards, if it isn't successful, then we can
			//	return notEnoughCards
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//first call the turn method for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		for (int k = 0; k < dealerAt; ++k) {
			cout << (*players[k]).name << endl;
			if (turn(*players[k]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	turnNumber++;
	//betting after fifth card is dealt
	potSize = potSize + bettingRound(true);


	//
	//TURN 4: call turn and afterTurn for every player
	//

	if (dealerAt == playersSizeToInt) {
		//call turn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			//because our turn method only return success or notEnoughCards, if it isn't successful, then we can
			//	return notEnoughCards
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//first call the turn method for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		for (int k = 0; k < dealerAt; ++k) {
			if (turn(*players[k]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	turnNumber++;
	//betting after sixth card is dealt
	potSize = potSize + bettingRound(true);


	//
	//TURN 5: call turn and afterTurn for every player
	//

	if (dealerAt == playersSizeToInt) {
		//call turn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			//because our turn method only return success or notEnoughCards, if it isn't successful, then we can
			//	return notEnoughCards
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = 0; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
	}
	else {
		//first call the turn method for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
		}
		for (int k = 0; k < dealerAt; ++k) {
			if (turn(*players[k]) != success) {
				return notEnoughCards;
			}
		}
		//then call beforeTurn for every player in the game
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			beforeTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			beforeTurn(*players[k]);
		}
	}
	turnNumber = 2;
	//betting after seventh card is dealt
	potSize = potSize + bettingRound(true);

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