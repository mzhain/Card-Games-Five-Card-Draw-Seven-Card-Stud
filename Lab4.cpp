// file name: lab4.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: defines the entry point for the console application

#include "stdafx.h"
#include "Cards.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Game.h"
#include "FiveCardDraw.h"
using namespace std;

/*
* the main function accepts command line arguments
*	-the command line arguments are checked to make sure that the correct number of args (at least 4) are passed and
*		that they contain the proper wording (name of a game including "FiveCardDraw")
*		-specific use messages are printed for each possible error upon running the program
*	-if the command line arguments are passed in correctly, then the program runs as normal
*		-a new game is created and the specified players are added
*		-the game continues to play so long as there are at least two players involved
*/
int main(int argc, char *argv[])
{
	//if less than four command line arguments are specified, return notEnoughArgs
	if (argc < expectedArgs) {
		usageMessage(argv[program_name], useMessageWrongNumberOfArgs);
		return notEnoughArgs;
	}
	try {
		//start the game and make a pointer to the instance of that new game
		Game::startGame(argv[gameName]);
		shared_ptr<Game> ourGame = Game::instance();
		//for all the players that were specified in the command line, add them to the game
		for (int i = player1; i < argc; ++i) {
			(*ourGame).addPlayer(argv[i]);
		}
		//while there are at least two players in the game, continue playing

		(*ourGame).beforeGame();


		while ((*ourGame).players.size() > 1) {
			(*ourGame).beforeRound();
			(*ourGame).round();
			(*ourGame).afterRound();
		}

		//end the current game if there are less than two players involved
		if ((*ourGame).players.size() == 0 || (*ourGame).players.size() == 1) {
			(*ourGame).stopGame();
			(*ourGame).playGame();
		}
		return success;
	}
	//print out error messages for all possibly thrown exceptions and exit the program
	catch (returns e) {
		if (e == notEnoughArgs) {
			usageMessage(argv[program_name], useMessageWrongNumberOfArgs);
			return notEnoughArgs;
		}
		if (e == HandReferencingOutOfBounds) {
			usageMessage(argv[program_name], useMessageHandRefOutOfBounds);
			return HandReferencingOutOfBounds;
		}
		if (e == couldNotOpenFile) {
			return couldNotOpenFile;
		}
		if (e == instanceNotAvailable) {
			usageMessage(argv[program_name], useMessageInstanceNotAvailable);
			return instanceNotAvailable;
		}
		if (e == gameAlreadyStarted) {
			usageMessage(argv[program_name], useMessageGameAlreadyStarted);
			return gameAlreadyStarted;
		}
		if (e == unknownGame) {
			usageMessage(argv[program_name], useMessageUnknownGame);
			return unknownGame;
		}
		if (e == noGameInProgress) {
			usageMessage(argv[program_name], useMessageNoGameInProgress);
			return noGameInProgress;
		}
		if (e == alreadyPlaying) {
			//do nothing here, just continue execution
		}
	}
	return 0;
}