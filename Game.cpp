// file name: Game.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: eleven methods to accompany Game objects and compliment the program's functionality
//		includes a destructor & two non member functions

#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"

shared_ptr<Game> Game::myGame = nullptr;

//public default constructor
//	initializes the dealer position member variable to be 0, and then iterates through all of the 52 valid combinations
//	of suit and rank as defined in the Card class enumerations and passes a Card corresponding to each combination into
//	a call to the add_card method of the main deck member variable that was inherited from the Game base class
Game::Game() : currentDealerPosition{ 0 } {
	for (int suitInt = Card::clubs; suitInt != Card::spades; ++suitInt) {
		for (int rankInt = Card::two; rankInt != Card::ace; ++rankInt) {
			mainDeck.add_card(Card(static_cast<Card::Rank>(rankInt), static_cast<Card::Suit>(suitInt)));
		}
	}
}


//make an instance of a game object
shared_ptr<Game> Game::instance() {
	if (myGame == nullptr) {
		throw instanceNotAvailable;
	}
	return myGame;
}

//checks to see if the game has already been started and then starts the game accordingly
void Game::startGame(const string &s) {
	if (myGame != nullptr) {
		throw gameAlreadyStarted;
	}
	//if this find function returns string::npos, that means it wasn't found in the string,
	//	so we should throw an unknownGame exception
	if (s.find(fiveCardDraw) == string::npos && s.find(sevenCardStud) == string::npos && s.find(texasHoldEm) == string::npos) {
		throw unknownGame;
	}
	else {
		//if the code gets into this if statement, that means that SevenCardStud was found, not FiveCardDraw or TexasHoldEm
		if (s.find(fiveCardDraw) == string::npos && s.find(texasHoldEm) == string::npos) {
			cout << endl << "SevenCardStud game STARTED" << endl << endl;
			shared_ptr<SevenCardStud> gameP(new SevenCardStud());
			myGame = gameP;
		}
		//otherwise check if we should start a game of FiveCardDraw or TexasHoldEm
		else {
			//if it doesn't find FiveCardDraw, then make a new came of TexasHoldEm
			if (s.find(fiveCardDraw) == string::npos) {
				cout << endl << "TexasHoldEm game STARTED" << endl << endl;
				shared_ptr<TexasHoldEm> gameP(new TexasHoldEm());
				myGame = gameP;
			}
			//otherwise make a game of FiveCardDraw
			else {
				cout << endl << "FiveCardDraw game STARTED" << endl << endl;
				shared_ptr<FiveCardDraw> gameP(new FiveCardDraw());
				myGame = gameP;
			}
		}
	}
}

//stop the game
void Game::stopGame() {
	//if myGame is null (isn't pointing to anything), throw a noGameInProgress exception
	if (myGame == nullptr) {
		throw noGameInProgress;
	}
	else {
		myGame = nullptr;
	}
}

void Game::playGame() {
	//prompt the user to see if they want to play another game or be done
	cout << newGameOrQuit << endl;
	string input = "";
	getline(cin, input);
	//if they said yes, ask them what kind of game they want to play and the names of the players who want to play
	if (input == "yes") {
		//make a new vector to eventually pass back to main with the new arguments
		vector<string> newStuff;
		//fill the first space with the program name
		newStuff.push_back("lab4");
		cout << "What kind of game do you want to play?\nPlease type \"FiveCardDraw\", \"SevenCardStud\", or \"TexasHoldEm\" followed by a space and the names of the players who want to play separated by spaces" << endl << endl;
		string newArgs = "";
		getline(cin, newArgs);
		istringstream iss(newArgs);
		string curArg;
		//fill the rest of the newStuff vector with the other arguments that were just provided by the user
		while (iss >> curArg) {
			newStuff.push_back(curArg);
		}
		if (newStuff.size() < expectedArgs) {
			throw notEnoughArgs;
		}
		//start the game and make a pointer to the instance of that new game
		Game::startGame(newStuff[gameName]);
		shared_ptr<Game> ourGame = Game::instance();
		//for all the players that were specified in the command line, add them to the game
		for (size_t i = player1; i < newStuff.size(); ++i) {
			(*ourGame).addPlayer(newStuff[i]);
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
			playGame();
		}
	}
	else {
		//if they said no, then we're done - just return success, therefore exiting the program
		if (input == "no") {
			return;
		}
		//if they didn't say yes or no, then reprompt them
		else {
			playGame();
		}
	}
}

//add players to the players mamber variable
void Game::addPlayer(const string &s) {
	for (auto curPlayer = players.begin(); curPlayer != players.end(); ++curPlayer) {
		//if the name was already found in the players vector, then throw an alreadyPlaying exception
		if ((**curPlayer).name == s) {
			cout << s << " is already playing in this Game" << endl;
			throw alreadyPlaying;
		}
	}
	//otherwise dynamically allocate a Player with that name and pushback a smart_ptr to that Player to the players vector
	shared_ptr<Player> newPlayer = make_shared<Player>(s.c_str());
	players.push_back(newPlayer);
}

//use this method to see if a player already exists in the players vector member variable
shared_ptr<Player> Game::findPlayer(const string &findMe) {
	for (auto curPlayer = players.begin(); curPlayer != players.end(); ++curPlayer) {
		//if the Player with that name is in the players vector, return the pointer to that Player
		if ((**curPlayer).name == findMe) {
			return *curPlayer;
		}
	}
	//otherwise if they aren't in the game, return a shared_ptr to null
	return make_shared<Player>(nullptr);
}

//destructor
Game::~Game() {

}

//the afterTurn method prints out the player's name and the contents of their hand
int Game::afterTurn(Player &p) {
	cout << p.name << "'s Hand After Turn: " << p.hand << endl;
	return 0;
}

//deals with all the logistics of counting wins and losses after a round is over
//	includes getting the ranks of all of the players and incrementing wins and losses of each player, and then
//	printing out every player's status in the game
//	-also allows the user to specify if any players want to leave the game or join the game
int Game::afterRound() {
	//Sorts the vector of players to see who had the highest hand
	vector<shared_ptr<Player>> tmp(players.size());
	copy(players.begin(), players.end(), tmp.begin());
	sort(tmp.begin(), tmp.end(), pokerRankCompare2);

	// increment the wins of the highest hand
	unsigned int numWin = 1;
	//find out how many people had the highest hand
	while (pokerRankCompare2(tmp[numWin - 1], tmp[tmp.size() - (numWin)]) == false && numWin < (tmp.size() - 1)) {
		numWin++;
	}

	for (unsigned int i = 0; i < numWin; ++i) {
		(*tmp[i]).handsWon++;
	}

	if ((potSize / numWin) % numWin == 0) {
		for (unsigned int i = 0; i < numWin; ++i) {
			(*tmp[i]).chips = (*tmp[i]).chips + (potSize / numWin);
		}
		potSize = 0;
	}
	else {
		for (unsigned int i = 0; i < numWin; ++i) {
			(*tmp[i]).chips = (*tmp[i]).chips + (potSize / numWin);
		}
		potSize = (potSize / numWin) % numWin;
	}



	// increment the losses of the losing hands
	for (unsigned int i = 0; i < tmp.size() - numWin; ++i) {
		(*tmp[tmp.size() - 1 - i]).handsLost++;
	}

	//change all values of chips back to positive numbers
	for (size_t i = 0; i < players.size(); ++i) {
		if ((*players[i]).chips < 0) {
			(*players[i]).chips = (*players[i]).chips * -1;
		}
	}
	//print out everything
	for (size_t i = 0; i < tmp.size(); ++i) {
		cout << endl;
		if ((*tmp[i]).hand.getHandSize() == 0) {
			cout << (*tmp[i]) << "Folded" << endl;
		}
		else {
			//before we print a player's winning/losing hand at the end of a round, make all the cards visible
			for (int j = 0; j < (*tmp[i]).hand.getHandSize(); ++j) {
				(*tmp[i]).hand.myHand[j].show = true;
			}
			//sort the player's final hand before printing it out to make it clearer as to what hand rank they got
			sort((*tmp[i]).hand.myHand.begin(), (*tmp[i]).hand.myHand.end());
			cout << (*tmp[i]) << (*tmp[i]).hand << endl;
		}
	}

	//Move all the cards back to the main deck
	//first move all the card in peoples hands

	for (size_t i = 0; i < tmp.size(); ++i) {

		int lengthOfHand = (*tmp[i]).hand.getHandSize();
		for (int j = 0; j < lengthOfHand; ++j) {
			Card temp = (*tmp[i]).hand[(*tmp[i]).hand.getHandSize() - 1];
			mainDeck.add_card(temp);
			(*tmp[i]).hand.remove_card((*tmp[i]).hand.getHandSize() - 1);
		}

	}
	//then move the cards from the discard deck

	for (int i = 0; i < discardedDeck.getDeckSize(); ++i) {
		Card temp = discardedDeck.remove_card();
		mainDeck.add_card(temp);
	}

	//check to see if the players in the game have any chips left  LAB4
	//	if they don't, remove them from the game

	vector<string> toRemove = haveCoin(tmp);

	for (size_t i = 0; i < toRemove.size(); ++i)
	{
		//erase both pointers
		int j = 0;
		while ((*players[j]).name != toRemove[i]) {
			j++;
		}
		players.erase(players.begin() + j);
		int k = 0;
		while ((*tmp[k]).name != toRemove[i]) {
			k++;
		}
		tmp.erase(tmp.begin() + k);
	}


	//Asking if players want to leave the game
	bool proceed = false;
	while (proceed == false) {
		cout << endl;
		cout << "Please enter the names of any players who want to LEAVE the game\nHit the enter or return key after each player's name" << endl;
		cout << "If no players want to LEAVE, enter 'no'" << endl;
		string readIn;
		cin >> readIn;

		if (readIn == "no") {
			proceed = true;
		}
		else {
			for (unsigned int i = 0; i < tmp.size(); ++i) {
				if ((*tmp[i]).name == readIn) {
					string fileName = readIn + ".txt";
					fstream ofs;
					//if the file existed i wanted to clear out the old data
					ofs.open(fileName, ios::out | ios::trunc);
					ofs.close();
					ofs.open(fileName);
					//write the data back into the file
					if (ofs.is_open()) {
						ofs << (*tmp[i]).name << " " << (*tmp[i]).handsWon << " " << (*tmp[i]).handsLost << " " << (*tmp[i]).chips << endl;
						ofs.close();
					}
					else {
						ofstream outputFile(fileName);
						outputFile << (*tmp[i]).name << " " << (*tmp[i]).handsWon << " " << (*tmp[i]).handsLost << " " << (*tmp[i]).chips << endl;
						outputFile.close();
					}

					//erase both pointers
					int i = 0;
					while ((*players[i]).name != readIn) {
						i++;
					}
					players.erase(players.begin() + i);
					while ((*tmp[i]).name != readIn) {
						i++;
					}
					tmp.erase(tmp.begin() + i);
				}
			}
		}
	}
	//Asking if players want to join the game

	bool proceed2 = false;
	while (proceed2 == false) {
		cout << endl;
		cout << "Please enter the names of any players who want to JOIN the game\nHit the enter or return key after each player's name" << endl;
		cout << "If no players want to JOIN, enter 'no'" << endl;
		string readIn;
		cin >> readIn;
		if (readIn == "no") {
			proceed2 = true;
		}
		else {
			bool inGame = false;

			if (tmp.size() == 0) {
				addPlayer(readIn);
			}
			else {
				for (unsigned int i = 0; i < tmp.size(); i++) {

					if ((*tmp[i]).name == readIn) {
						cout << readIn << " is already in the game" << endl;
						inGame = true;
					}
				}
				if (inGame == false) {
					addPlayer(readIn);
					if ((*players[players.size()]).chips <= 0) {
						bool proceed3 = false;
						while (proceed3 == false) {
							string readIn;
							cin >> readIn;
							if (readIn == "yes") {
								(*players[players.size()]).chips = 20;
								proceed3 = true;
							}
							if (readIn == "no") {
								string fileName = readIn + ".txt";
								fstream ofs;
								//if the file existed i wanted to clear out the old data
								ofs.open(fileName, ios::out | ios::trunc);
								ofs.close();
								ofs.open(fileName);
								//write the data back into the file
								if (ofs.is_open()) {
									ofs << (*players[players.size()]).name << " " << (*players[players.size()]).handsWon << " " << (*players[players.size()]).handsLost << " " << (*players[players.size()]).chips << endl;
									ofs.close();
								}
								else {
									ofstream outputFile(fileName);
									outputFile << (*players[players.size()]).name << " " << (*players[players.size()]).handsWon << " " << (*players[players.size()]).handsLost << " " << (*players[players.size()]).chips << endl;
									outputFile.close();
								}

								//erase both pointers
								int i = 0;
								while ((*players[i]).name != readIn) {
									i++;
								}
								players.erase(players.begin() + i);
								proceed3 = true;
							}
						}
					}
				}
			}
		}
	}
	if (players.size() > 0) {
		currentDealerPosition = (currentDealerPosition + 1) % players.size();
		//read in an extra newline character here so that problems don't arise in prompting users to discard cards
		string cardsToDiscard;
		getline(cin, cardsToDiscard);
	}
	return success;
}

//the beforeGame method checks to see which players don't have any chips left and removes the players who have no
//	chips and did not choose to be given the automatic 20 chip refill
int Game::beforeGame() {
	vector<string> toRemove = haveCoin(players);
	for (size_t i = 0; i < toRemove.size(); i) {
		//erase both pointers
		int j = 0;
		while ((*players[j]).name != toRemove[i]) {
			j++;
		}
		players.erase(players.begin() + j);
	}
	return success;
}


int Game::bettingRound(bool what)
{
	int i = currentDealerPosition;
	int currentPot = 0;
	int betsMatched = 0;
	int checks = 0;
	int calls = 0;



	//int playersStillIn = players.size();
	int playersStillIn = 0;
	for (size_t i = 0; i < players.size(); i++)
	{
		if ((*players[i]).chips >= 0) {
			playersStillIn++;

		}
	}

	bool betPlaced = false;
	vector<int> amountBet;
	for (size_t i = 0; i < players.size(); i++)
	{
		amountBet.push_back(0);
	}
	int locOfLargest = 0;
	for (size_t i = 0; i < amountBet.size() - 1; i++)
	{
		if (amountBet[i] < amountBet[i + 1]) {
			locOfLargest = i + 1;
		}
	}
	int numWithCards = 0;
	for (size_t i = 0; i < players.size(); i++)
	{
		if ((*players[i]).hand.getHandSize() != 0) {
			numWithCards++;

		}
	}

	if (numWithCards <= oneIndex && what == true) {
		//do nothing
		cout << "not enough players remaining in round to have betting    PLEASE PRESS ENTER" << endl;
	}

	else  if (what == true) {
		while (betsMatched < (playersStillIn - 1) && checks < playersStillIn && calls < (playersStillIn - 1)) {

			if ((*players[i]).chips < 0) {
				//do nothing bc that player has already folded
				cout << (*players[i]).name << " HAS NO MORE CARDS and does not partcipate in betting" << endl;
			}

			else if (betPlaced == false) {

				cout << (*players[i]).name << " do you want to Bet or Check? please enter either \"check\", \"bet 1\", or \"bet 2\"." << endl;
				cout << "your current number of chips is: " << (*players[i]).chips << endl;
				string readIn;
				bool proceed = false;
				while (proceed == false) {
					cin >> readIn;
					if (readIn == "check") {
						players.size();
						checks++;
						proceed = true;
					}
					if (readIn == "bet" && (*players[i]).chips > 0) {
						proceed = true;
						betPlaced = true;
						int betIn;
						bool proceed2 = false;
						while (proceed2 == false) {
							checks = 0;
							cin >> betIn;
							if ((betIn == 1 || betIn == 2) && (*players[i]).chips >= betIn) {
								proceed2 = true;
								locOfLargest = i;
								amountBet[locOfLargest] = amountBet[locOfLargest] + betIn;
								currentPot = currentPot + betIn;
								(*players[i]).chips = (*players[i]).chips - betIn;
							}
						}
					}
				}
			}

			else {
				cout << (*players[i]).name << " do you want to fold, call, or raise?  please enter 'fold, 'call', or 'raise #'" << endl;
				cout << "your current number of chips is: " << (*players[i]).chips << endl;
				string readIn;
				bool proceed = false;
				while (proceed == false) {
					cin >> readIn;
					if (readIn == "call") {
						calls++;
						int chipsNeeded = (amountBet[locOfLargest] - amountBet[i]);
						if ((*players[i]).chips >= chipsNeeded) {
							(*players[i]).chips = (*players[i]).chips - chipsNeeded;

							currentPot = currentPot + chipsNeeded;

							amountBet[i] = amountBet[locOfLargest];
							locOfLargest = i;

						}
						else {
							currentPot = currentPot + (*players[i]).chips;
							(*players[i]).chips = 0;
						}
						proceed = true;
					}
					else if (readIn == "fold") {

						playersStillIn--;
						proceed = true;
						(*players[i]).chips = (*players[i]).chips *-1;

						if ((*players[i]).hand.getHandSize() != 0) {
							int numINHAND = (*players[i]).hand.getHandSize();
							for (int j = 0; j < numINHAND; ++j) {

								Card temp = (*players[i]).hand[(*players[i]).hand.getHandSize() - 1];
								discardedDeck.add_card(temp);
								(*players[i]).hand.remove_card((*players[i]).hand.getHandSize() - 1);

							}
						}


					}
					else if (readIn == "raise") {
						int chipsNeeded = (amountBet[locOfLargest] - amountBet[i]);
						if ((*players[i]).chips > chipsNeeded) {
							proceed = true;			 				// need to check if have enough chips to meet raise adn orginal obligation
							int betIn;
							bool proceed2 = false;
							while (proceed2 == false) {
								cin >> betIn;
								if ((betIn == 1 || betIn == 2) && (*players[i]).chips >= chipsNeeded + betIn) {
									calls = 0;
									proceed2 = true;

									currentPot = currentPot + betIn + chipsNeeded;
									(*players[i]).chips = (*players[i]).chips - chipsNeeded - betIn;
									amountBet[i] = amountBet[locOfLargest] + betIn;
									locOfLargest = i;
								}
							}
						}
						else {
							cout << "you do not have enough chips to raise, please pick anouther choice" << endl;
						}

					}

				}
			}
			i = (i + 1) % players.size();
		}
	}



	else {
		while (betsMatched < (playersStillIn - 1) && checks < playersStillIn && calls < (playersStillIn - 1)) {
			if (betPlaced == false) {

				cout << (*players[i]).name << " do you want to Bet or Check? please enter either \"check\", \"bet 1\", or \"bet 2\"." << endl;
				cout << "your current number of chips is: " << (*players[i]).chips << endl;
				string readIn;
				bool proceed = false;
				while (proceed == false) {
					cin >> readIn;
					if (readIn == "check") {
						players.size();
						checks++;
						proceed = true;
					}
					if (readIn == "bet" && (*players[i]).chips > 0) {
						proceed = true;
						betPlaced = true;
						int betIn;
						bool proceed2 = false;
						while (proceed2 == false) {
							checks = 0;
							cin >> betIn;
							if ((betIn == 1 || betIn == 2) && (*players[i]).chips >= betIn) {
								proceed2 = true;
								locOfLargest = i;
								amountBet[locOfLargest] = amountBet[locOfLargest] + betIn;
								currentPot = currentPot + betIn;
								(*players[i]).chips = (*players[i]).chips - betIn;
							}
						}
					}
				}
			}

			else {
				cout << (*players[i]).name << " do you want to fold, call, or raise?  please enter 'fold, 'call', or 'raise #'" << endl;
				cout << "your current number of chips is: " << (*players[i]).chips << endl;
				string readIn;
				bool proceed = false;
				while (proceed == false) {
					cin >> readIn;
					if (readIn == "call") {
						calls++;
						int chipsNeeded = (amountBet[locOfLargest] - amountBet[i]);
						if ((*players[i]).chips >= chipsNeeded) {
							(*players[i]).chips = (*players[i]).chips - chipsNeeded;

							currentPot = currentPot + chipsNeeded;

							amountBet[i] = amountBet[locOfLargest];
							locOfLargest = i;

						}
						else {
							currentPot = currentPot + (*players[i]).chips;
							(*players[i]).chips = 0;
						}
						proceed = true;
					}
					else if (readIn == "fold") {

						playersStillIn--;

						proceed = true;
						(*players[i]).chips = (*players[i]).chips *-1;

						if ((*players[i]).hand.getHandSize() != 0) {
							int numINHAND = (*players[i]).hand.getHandSize();
							for (int j = 0; j < numINHAND; ++j) {

								Card temp = (*players[i]).hand[(*players[i]).hand.getHandSize() - 1];
								discardedDeck.add_card(temp);

								(*players[i]).hand.remove_card((*players[i]).hand.getHandSize() - 1);

							}
						}


					}
					else if (readIn == "raise") {
						int chipsNeeded = (amountBet[locOfLargest] - amountBet[i]);
						if ((*players[i]).chips > chipsNeeded) {
							proceed = true;			 				// need to check if have enough chips to meet raise adn orginal obligation
							int betIn;
							bool proceed2 = false;
							while (proceed2 == false) {
								cin >> betIn;
								if ((betIn == 1 || betIn == 2) && (*players[i]).chips >= chipsNeeded + betIn) {
									calls = 0;
									proceed2 = true;

									currentPot = currentPot + betIn + chipsNeeded;
									(*players[i]).chips = (*players[i]).chips - chipsNeeded - betIn;
									amountBet[i] = amountBet[locOfLargest] + betIn;
									locOfLargest = i;
								}
							}
						}
						else {
							cout << "you do not have enough chips to raise, please pick anouther choice" << endl;
						}

					}

				}
			}
			i = (i + 1) % players.size();
		}
	}



	//these two lines are just so that the rest of before turn works correcty
	string cardsToDiscard;
	getline(cin, cardsToDiscard);

	return currentPot;
}



//uses the original pokerRankCompare function to compare two hands of two player's that were passed in
bool pokerRankCompare2(shared_ptr<Player> p1, shared_ptr<Player> p2) {
	if (p1 == nullptr) {
		return false;
	}
	else {
		if (p2 == nullptr) {
			return true;
		}
		else {
			if ((*p1).hand.getHandSize() < numCardsInHand || (*p2).hand.getHandSize() < numCardsInHand) {
				if ((*p1).hand.getHandSize() == 0 && (*p2).hand.getHandSize() == 0) {
					return false;
				}
				else if ((*p1).hand.getHandSize() == 0) {
					return false;
				}
				else {
					return true;
				}
			}
			else {
				return pokerRankCompare((*p1).hand, (*p2).hand);
			}
		}
	}
}

vector<string> haveCoin(vector<shared_ptr<Player>> players) {
	vector<string> toRemove;
	for (size_t i = 0; i < players.size(); ++i) {
		if ((*players[i]).chips <= 0) {
			cout << (*players[i]).name << ", you are out of chips. type 'yes' to reset your chip level to 20 or 'no' to leave the game" << endl;
			bool proceed = false;
			while (proceed == false) {
				string readIn;
				cin >> readIn;
				if (readIn == "yes") {
					(*players[i]).chips = 20;
					proceed = true;
				}
				if (readIn == "no") {
					string fileName = (*players[i]).name + ".txt";
					fstream ofs;
					//if the file existed i wanted to clear out the old data
					ofs.open(fileName, ios::out | ios::trunc);
					ofs.close();
					ofs.open(fileName);
					//write the data back into the file
					if (ofs.is_open()) {
						ofs << (*players[i]).name << " " << (*players[i]).handsWon << " " << (*players[i]).handsLost << " " << (*players[i]).chips << endl;
						ofs.close();
					}
					else {
						ofstream outputFile(fileName);
						outputFile << (*players[i]).name << " " << (*players[i]).handsWon << " " << (*players[i]).handsLost << " " << (*players[i]).chips << endl;
						outputFile.close();
					}

					//what players to erase
					toRemove.push_back((*players[i]).name);
					proceed = true;
				}
			}
		}
	}
	return toRemove;
}


//the combinations code was adapted from a solution I found online at:
//https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
void Game::combinations(int index, Player &p, Hand &tmp, vector<Hand> &allCombinations, int start, int end) {
	// Current combination is ready to be printed, print it
	if (index == numCardsInHand) {
		allCombinations.push_back(tmp);
		return;
	}

	// replace index with all possible elements. The condition
	// "end-i+1 >= r-index" makes sure that including one element
	// at index will make a combination with remaining elements
	// at remaining positions
	for (int i = start; i <= end && end - i + 1 >= numCardsInHand - index; ++i) {
		tmp.myHand[index] = p.hand.myHand[i];
		combinations(index + 1, p, tmp, allCombinations, i + 1, end);
	}
}