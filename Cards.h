// file name: Cards.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions correlating with the Cards.cpp file

#pragma once
using namespace std;
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

//defines the enums with values for ranks and suits that a playing card can possess
struct Card {
	enum Suit { clubs, diamonds, hearts, spades };
	enum Rank { two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
	Suit currentSuit;
	Rank currentRank;
	bool show;
	Card();
	Card(Rank rank, Suit suit);
	bool const operator< (const Card & card1) const;
	bool operator ==(const Card other) const;
	string cardToString() const;
};

//defines an enum that contains return values for success and various errors (see ReadMe for return statement explanations)
enum returns {
	success, couldNotOpenFile, unsuccessfulRun, noArgs, HandReferencingOutOfBounds, instanceNotAvailable,
	gameAlreadyStarted, unknownGame, noGameInProgress, alreadyPlaying, notEnoughCards, notEnoughArgs
};

//defines an enum that contains values for the number of cards in a valid poker hand
enum cardsInHand { firstCardInHand, secondCardInHand, thirdCardInHand, fourthCardInHand, fifthCardInHand, sixthCardInHand, seventhCardInHand };

//defines an enum that contains the possible poker hand rankings
//	"flush_" is used instead of "flush" because "flush" is a function of the STL, so using "flush" here would be ambiguous
enum handRanks { straightFlush, fourOfAKind, fullHouse, flush_, straight, threeOfAKind, twoPairs, onePair, noRank };

enum command_line_arguments { program_name, gameName, player1, player2 };

//the following seven declarations are used in place of hard-coding constants in the Cards.cpp file
//these enums and consts are used in functions throughout Cards.cpp and some other files
const int numCardsInHand = 5;
const int SCSfirstTurn = 3;
const int SCSotherTurns = 1;
const int expectedArgs = 4;
const int cardsInHandSCS = 7;
const int cardsInHandTHE = 2;
const string comment = "//";
const string fiveCardDraw = "FiveCardDraw";
const string sevenCardStud = "SevenCardStud";
const string texasHoldEm = "TexasHoldEm";
const string discardCardPrompt = "Which (if any) of your cards do you want to discard?\nIf you want to remove the first card and third card, type: '1 3'\nPlease include a SPACE between each card you want to remove\nIf you don't want to remove any cards, simply hit the ENTER or RETURN key";
const string newGameOrQuit = "The current game is finished. Do you want to play another game?\n Type \"yes\" to play another game. Type \"no\" to quit the program.";
enum cardRankIndex { curCardRank, curCardRankTwo };
enum validCardSizes { cardLengthTwo = 2, cardLengthThree };
enum indices { zeroIndex, oneIndex, twoIndex, threeIndex };

//these are all usage messages that are used throughout the program
const string useMessageHandRefOutOfBounds = "You input an incorrect value for a card in your Hand, please try again";
const string useMessageWrongNumberOfArgs = "You need to specifiy at least 4 parameters, the executable, the game name (include \"FiveCardDraw\" or \"SevenCardStud\"), and the name of at least 2 players\nexample of correct run: \"lab3.exe myFiveCardDraw Emme Matt\"";
const string useMessageInstanceNotAvailable = "system error: static pointer member variable of class Game is singular - cannot create instance of new Game";
const string useMessageGameAlreadyStarted = "system error: static pointer member variable of class Game is not singular - this game has already been started and cannot be created again";
const string useMessageUnknownGame = "The second parameter you specify (first one after the executable file) must contain the phrase \"FiveCardDraw\" or \"SevenCardStud\"\nexample of correct run: \"lab3.exe myFiveCardDraw Emme Matt\"";
const string useMessageNoGameInProgress = "system error: there is no game in progress";

bool validValue(string curCard);

Card getRankAndSuit(string cardString, Card curCard);

int printCards(const vector<Card> &cardRef);

int usageMessage(string progName, string message);

void invalidCardMessage(string message);

void invalidHandMessage(string currentHand);

