// file name: Deck.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: six methods to accompany Deck objects and compliment the program's functionality
//			includes constructor and destructor

#include "stdafx.h"
#include "Deck.h"

//because I don't use a member initialization list here, I'm using the fact that the private member
//	variable "myDeck" is being default initialized to be empty
Deck::Deck()
{

}

Deck::Deck(string fileName) {
	//if the load function does not return success, it returned "couldNotOpenFile" so this exception
	//is thrown here and will be caught by the main function
	if (loadFile(fileName) != success) {
		throw couldNotOpenFile;
	}
}
Deck::~Deck() {

}
/*
* the loadFile function takes in a fileName
*	-the fileName is used to open an input stream and read valid Card values and poker hands from the file
*		-if the fileName is invalid or the stream cannot be opened, the method returns couldNotOpenFile
*	-the getRankAndSuit function is then called to translate the Card value string into a rank
*		and suit that can be assigned to the Card object itself
*	-the Card is then added to the end of the myDeck vector, and returns success
*	-parseFile only continues operations on valid values read from the file
*		-all invalid cards will be skipped and all invalid hands will be skipped
*	-if an invalid card and/or hand is encountered, a warning message will be printed to the output stream
*/
int Deck::loadFile(string fileName) {
	string input = fileName;
	ifstream ifs;
	ifs.open(input);
	if (!ifs.is_open()) {
		cout << fileName << " had an error: could not open file for reading" << endl;
		return couldNotOpenFile;
	}
	else {
		string currentLine;
		string currentCard;
		while (getline(ifs, currentLine)) {
			istringstream iss(currentLine);
			while (iss >> currentCard) {
				bool endOfLine = false;
				if (currentCard.find(comment) != string::npos) {
					//if the current card contains a comment, check where the comment is
					int commentLocation = currentCard.find(comment);
					//if it's at position 0, then break and move on to the next line
					if (commentLocation == zeroIndex) {
						break;
					}
					//if there's a potential card in front of the comment (comment appended immediately),
					//get rid of the comment, and continue on to verify the card
					if (commentLocation == twoIndex || commentLocation == threeIndex) {
						currentCard = currentCard.substr(zeroIndex, commentLocation);
						endOfLine = true;
					}
				}
				if (validValue(currentCard)) {
					Card newCard;
					//initialize newCard with random values so that it can be passed to getRankAndSuit,
					//where the correct rank and suit will be assigned to newCard
					newCard.currentRank = Card::ace;
					newCard.currentSuit = Card::hearts;
					newCard = getRankAndSuit(currentCard, newCard);
					myDeck.push_back(newCard);
					//if it's the end of the line, don't continue to assess new potential "card values"
					if (endOfLine) {
						break;
					}
				}
				else {
					//if the current card is not valid, print an error message, skip it, and continue if there are more potential cards
					invalidCardMessage(currentCard);
					if (endOfLine) {
						break;
					}
					continue;
				}
			}
		}
		return success;
	}
}

//the shuffleDeck method takes all the Cards in the Deck and rearranges them completely randomly
void Deck::shuffleDeck() {
	random_device rand;
	mt19937 myRand(rand());
	shuffle(myDeck.begin(), myDeck.end(), myRand);
}

//getDeckSize returns an integer that represents the number of Cards that the Deck holds
int Deck::getDeckSize() const {
	return myDeck.size();
}

Deck Deck::add_card(Card card)
{
	myDeck.push_back(card);
	return *this;
}

Card Deck::remove_card()
{
	Card temp = myDeck[myDeck.size() - 1];
	myDeck.pop_back();
	return temp;
}

//this insertion operator allows us to easily print Deck objects to the output stream
ostream &operator<< (ostream &o, const Deck &d) {
	for (int i = 0; i < d.getDeckSize(); ++i) {
		o << d.myDeck[i].cardToString() << endl;
	}
	return o;
}