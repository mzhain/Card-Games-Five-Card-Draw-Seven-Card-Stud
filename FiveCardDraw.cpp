// file name: FiveCardDraw.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: five methods to accompany FiveCardDraw objects and compliment the program's functionality
//		includes default constructor

#include "stdafx.h"
#include "FiveCardDraw.h"
#include "Cards.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>

//public default constructor - calls the constructor for the Game base class, which does the following:
//	initializes the dealer position member variable to be 0, and then iterates through all of the 52 valid combinations
//	of suit and rank as defined in the Card class enumerations and passes a Card corresponding to each combination into
//	a call to the add_card method of the main deck member variable that was inherited from the Game base class
FiveCardDraw::FiveCardDraw() : Game() {

}

//the beforeTurn method prints out the player's name and the contents of their hand, then uses cout to print out a
//	prompt asking the user which (if any) of their cards to discard, and then use cin to obtain a response containing
//	valid size_t values for the positions of the cards to discard from their hand
int FiveCardDraw::beforeTurn(Player &p) {
	// if the player has less than zero chips, that means they've folded, so they don't to go through this method
	if (p.chips < 0) {
		return success;
	}
	cout << endl;
	cout << p.name << "'s Hand: " << p.hand << endl;
	cout << discardCardPrompt << endl;
	string cardsToDiscard;

	getline(cin, cardsToDiscard);
	//what
	size_t curCard;
	istringstream iss(cardsToDiscard);
	//if they don't want to remove any cards, return success
	if (cardsToDiscard == "") {
		return success;
	}

	set<size_t> removeThese;
	//read every Card that the user wants to get rid of into the removeThese set (gets rid of duplicates)
	//here we use a boolean to keep track of whether the value being currently evaluated is an integer or not
	bool isInt = false;
	while (iss >> curCard) {
		isInt = true;
		removeThese.insert(curCard);
	}
	if (isInt == false) {
		cout << "Please only enter integer values" << endl;
		return beforeTurn(p);
	}

	vector<int> removeThese2(removeThese.begin(), removeThese.end());



	//the set is already sorted from least to greatest
	//if the user wants to get rid of more cards than are in their hand OR if the greatest card they
	//	want to get rid of is greater than the hand size OR they input any negative values, prompt them again
	size_t sizeOfCurHand = p.hand.getHandSize();
	int sizeOfCurHand2 = p.hand.getHandSize();

	if ((removeThese2.size() > sizeOfCurHand) || (removeThese2[removeThese2.size() - 1] > sizeOfCurHand2) || (removeThese2[0] < 1)) {
		cout << useMessageHandRefOutOfBounds << endl;
		return beforeTurn(p);
	}

	for (int i = removeThese2.size(); i > 0; --i) {
		size_t cardToRemove = removeThese2[i - 1];
		cardToRemove--;
		discardedDeck.add_card(p.hand[cardToRemove]);
		p.hand.remove_card(cardToRemove);
	}
	return success;
}

//the turn method deals the player's hand as many cards from the main deck as the player had discarded
//	(so that the player again ends up with 5 cards in their hand)
//	also properly deals with the possible instance that one of the decks runs out of cards
int FiveCardDraw::turn(Player &p) {
	// if the player has less than zero chips, that means they've folded, so they don't to go through this method
	if (p.chips < 0) {
		return success;
	}
	int sizeOfCurHand = p.hand.getHandSize();
	if (mainDeck.getDeckSize() + discardedDeck.getDeckSize() < (numCardsInHand - sizeOfCurHand)) {
		return notEnoughCards;
	}
	if (mainDeck.getDeckSize() < (numCardsInHand - sizeOfCurHand)) {
		for (int i = 0; i < mainDeck.getDeckSize(); ++i) {
			if (p.chips >= 0) {
				p.hand << mainDeck;
			}
		}
		sizeOfCurHand = p.hand.getHandSize();
		for (int i = sizeOfCurHand; i < numCardsInHand; ++i) {
			discardedDeck.shuffleDeck();
			if (p.chips >= 0) {
				p.hand << discardedDeck;
			}
		}
	}
	else {
		for (int i = sizeOfCurHand; i < numCardsInHand; ++i) {
			if (p.chips >= 0) {
				p.hand << mainDeck;
			}
		}
	}
	return success;
}

//the beforeRound method assumes that the mainDeck will not run out of cards when dealing to all of the
//	players in the players vector
int FiveCardDraw::beforeRound() {
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


	//Betting before cards drawn
	potSize = potSize + bettingRound(false);

	//if the dealer is at the end of the players vector, the dealer will start dealing cards to the first player
	//	in the vector
	if (dealerAt == playersSizeToInt) {
		for (int i = 0; i < numCardsInHand; ++i) {
			for (int j = 0; j < playersSizeToInt; ++j) {
				if ((*players[j]).chips >= 0) {
					(*players[j]).hand << mainDeck;
				}
			}
		}
	}
	//if the dealer is not at the end of the players vector, deal to everyone after the dealer in the players
	//	vector and then to everyone before the dealer in the players vector
	else {
		for (int i = 0; i < numCardsInHand; ++i) {
			for (int j = dealerAt; j < playersSizeToInt; ++j) {
				if ((*players[j]).chips >= 0) {
					(*players[j]).hand << mainDeck;
				}
			}
			for (int k = 0; k < dealerAt; ++k) {
				if ((*players[k]).chips >= 0) {
					(*players[k]).hand << mainDeck;
				}
			}
		}
	}
	//once every player has been dealt their five cards, call the beforeTurn method for every player
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
	return success;
}

//call each players turn and afterTurn method
int FiveCardDraw::round() {
	int playersSizeToInt = static_cast<int>(players.size());

	int dealerAt = currentDealerPosition + 1;
	if (dealerAt == playersSizeToInt) {
		for (int j = 0; j < playersSizeToInt; ++j) {
			//because our turn method only return success or notEnoughCards, if it isn't successful, then we can
			//	return notEnoughCards
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
			afterTurn(*players[j]);
		}
	}
	else {
		for (int j = dealerAt; j < playersSizeToInt; ++j) {
			if (turn(*players[j]) != success) {
				return notEnoughCards;
			}
			afterTurn(*players[j]);
		}
		for (int k = 0; k < dealerAt; ++k) {
			if (turn(*players[k]) != success) {
				return notEnoughCards;
			}
			afterTurn(*players[k]);
		}
	}

	//Betting after cards drawn and hands have been finalized 
	potSize = potSize + bettingRound(true);

	return success;
}