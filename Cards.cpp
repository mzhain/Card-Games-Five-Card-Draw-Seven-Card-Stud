// file name: Cards.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: ten methods to accompany Card objects and compliment the program's functionality

#include "stdafx.h"
#include "Cards.h"

Card::Card() {

}

Card::Card(Rank rank, Suit suit) {
	currentRank = rank;
	currentSuit = suit;
	show = true;
}

/*
* validValue takes in a string and returns a bool as to whether the string contains valid
* rank and suit information
*/
bool validValue(string curCard) {
	int curCardSuit;
	//if the size of the current card isn't two or three
	if (curCard.size() != cardLengthThree && curCard.size() != cardLengthTwo) {
		return false;
	}
	if (curCard.size() == cardLengthThree) {
		curCardSuit = 2;
		//check if the card's rank is 10, because if it isn't, it's an invalid card
		if (!(curCard[curCardRank] == '1' && curCard[curCardRankTwo] == '0')) {
			return false;
		}
	}
	else {	//if (curCard.size() == 2)
		curCardSuit = 1;
		//here a logical negation operator is used because otherwise the if would return a null statement
		//and the else would return false -- makes more sense to use the logical negation operator
		if (!(curCard[curCardRank] == '2' || curCard[curCardRank] == '3' || curCard[curCardRank] == '4' || curCard[curCardRank] == '5'
			|| curCard[curCardRank] == '6' || curCard[curCardRank] == '7' || curCard[curCardRank] == '8' || curCard[curCardRank] == '9'
			|| curCard[curCardRank] == 'J' || curCard[curCardRank] == 'j' || curCard[curCardRank] == 'Q' || curCard[curCardRank] == 'q'
			|| curCard[curCardRank] == 'K' || curCard[curCardRank] == 'k' || curCard[curCardRank] == 'a' || curCard[curCardRank] == 'A')) {	//if(curCard contains an invalid value)
			return false;
		}
	}
	if (curCard[curCardSuit] == 'C' || curCard[curCardSuit] == 'c' || curCard[curCardSuit] == 'D' || curCard[curCardSuit] == 'd'
		|| curCard[curCardSuit] == 'H' || curCard[curCardSuit] == 'h' || curCard[curCardSuit] == 'S' || curCard[curCardSuit] == 's') {
		return true;
	}
	else {
		return false;
	}
}


/*
* getRankAndSuit accepts a string and a Card, returning a Card value
*	-we already know the string being passed in contains valid rank and suit information about a Card
*	-this information is extracted and the Card curCard is assigned the correct Rank and Suit,
*		and then returned
*/
Card getRankAndSuit(string cardString, Card curCard) {
	int curCardSuit = cardString.size() - 1;
	switch (cardString[curCardRank]) {
	case '1': curCard.currentRank = Card::ten; break;
	case '2': curCard.currentRank = Card::two; break;
	case '3': curCard.currentRank = Card::three; break;
	case '4': curCard.currentRank = Card::four; break;
	case '5': curCard.currentRank = Card::five; break;
	case '6': curCard.currentRank = Card::six; break;
	case '7': curCard.currentRank = Card::seven; break;
	case '8': curCard.currentRank = Card::eight; break;
	case '9': curCard.currentRank = Card::nine; break;
	case 'j': curCard.currentRank = Card::jack; break;
	case 'J': curCard.currentRank = Card::jack; break;
	case 'q': curCard.currentRank = Card::queen; break;
	case 'Q': curCard.currentRank = Card::queen; break;
	case 'k': curCard.currentRank = Card::king; break;
	case 'K': curCard.currentRank = Card::king; break;
	case 'a': curCard.currentRank = Card::ace; break;
	case 'A': curCard.currentRank = Card::ace; break;
	}
	switch (cardString[curCardSuit]) {
	case 'c': curCard.currentSuit = Card::clubs; break;
	case 'C': curCard.currentSuit = Card::clubs; break;
	case 'd': curCard.currentSuit = Card::diamonds; break;
	case 'D': curCard.currentSuit = Card::diamonds; break;
	case 'h': curCard.currentSuit = Card::hearts; break;
	case 'H': curCard.currentSuit = Card::hearts; break;
	case 's': curCard.currentSuit = Card::spades; break;
	case 'S': curCard.currentSuit = Card::spades; break;
	}
	return curCard;
}


/*
* printCards takes in a reference to a constant vector of Cards, returning an int defining success or failure
*	-for each Card in the vector (cardRef.size()), its Rank and Suit are translated into more readable
*		strings and printed direclty to the output stream
*/
int printCards(const vector<Card> &cardRef) {
	for (size_t i = 0; i < cardRef.size(); ++i) {
		switch (cardRef[i].currentRank) {
		case Card::ten: cout << "ten of "; break;
		case Card::two: cout << "two of "; break;
		case Card::three: cout << "three of "; break;
		case Card::four: cout << "four of "; break;
		case Card::five: cout << "five of "; break;
		case Card::six: cout << "six of "; break;
		case Card::seven: cout << "seven of "; break;
		case Card::eight: cout << "eight of "; break;
		case Card::nine: cout << "nine of "; break;
		case Card::jack: cout << "jack of "; break;
		case Card::queen: cout << "queen of "; break;
		case Card::king: cout << "king of "; break;
		case Card::ace: cout << "ace of "; break;
		}
		switch (cardRef[i].currentSuit) {
		case Card::clubs: cout << "clubs" << endl; break;
		case Card::diamonds: cout << "diamonds" << endl; break;
		case Card::hearts: cout << "hearts" << endl; break;
		case Card::spades: cout << "spades" << endl; break;
		}
	}
	return success;
}


/*
* usageMessage accepts a string that represents the name of the program and returns unsuccessfulRun
*	-this method prints a message directly to the output stream, informing the user about how to
*		properly run the program and avoid errors
*/
int usageMessage(string progName, string message) {
	cout << "Program Name: " << progName << endl;
	cout << message << endl;
	return unsuccessfulRun;
}


/*
* this method allows the program to use the less than operator (<) in regards to comparing two Cards
*	by defining how two cards should be compared
*	-they are first compared by rank, and if their ranks are the same, they are compared by suit
*/
bool const Card::operator< (const Card &card1) const {
	return (currentRank < card1.currentRank) || ((currentRank == card1.currentRank) && (currentSuit < card1.currentSuit));
}

/*
* this method allows the program to compare the equality of two Card objects
*/
bool Card::operator ==(const Card other) const {
	return((currentRank == other.currentRank) && (currentSuit == other.currentSuit));
}


/*
* invalidCardMessage simply takes in a string containing the invalid Card and prints a warning message
*	to the output stream to alert the user that an invalid card has been found and is being skipped over
*/
void invalidCardMessage(string message) {
	cout << "\"" << message << "\"" << " is not a valid card, so it will be skipped" << endl;
}


/*
* invalidHandMessage simply takes in a String containing the invalid hand and prints a warning message
*	to the output stream to alert the user that an invalid hand has been found and is being skipped over
*/
void invalidHandMessage(string currentHand) {
	cout << "\"" << currentHand << "\"" << " is not a valid hand, so it will be skipped" << endl;
}

string Card::cardToString() const {
	if (show == false) {
		return "*";
	}

	string result = "";

	switch (currentRank) {
	case Card::ten: result += "10"; break;
	case Card::two: result += "2"; break;
	case Card::three: result += "3"; break;
	case Card::four: result += "4"; break;
	case Card::five: result += "5"; break;
	case Card::six: result += "6"; break;
	case Card::seven: result += "7"; break;
	case Card::eight: result += "8"; break;
	case Card::nine: result += "9"; break;
	case Card::jack: result += "J"; break;
	case Card::queen: result += "Q"; break;
	case Card::king: result += "K"; break;
	case Card::ace: result += "A"; break;
	}
	switch (currentSuit) {
	case Card::clubs: result += "c"; break;
	case Card::diamonds: result += "d"; break;
	case Card::hearts: result += "h"; break;
	case Card::spades: result += "s"; break;
	}

	return result;
}