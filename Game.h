// file name: Game.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions associated with the Game.cpp file

#pragma once
#include "Deck.h"
#include "Player.h"
using namespace std;
#include <memory>
#include <vector>
#include <algorithm>

class Game {
protected:
	//by default, the shared_ptr is initializaed to null (pointing to nothing)
	static shared_ptr<Game> myGame;
	//using the default constructor, mainDeck is made to be empty
	Deck mainDeck;
	//vector default constructor makes "players" empty (contains no elements)
	vector<shared_ptr<Player>> players;
	//indicates which Player is the current "dealer" in the game (starts at 0 and rotates among the Player positions
	//	throughout the Game)
	size_t currentDealerPosition;
	//discardedDeck is default constructed to be empty
	Deck discardedDeck;
	unsigned int potSize;
public:
	Game();
	static shared_ptr<Game> instance();
	static void startGame(const string &s);
	static void stopGame();
	void playGame();
	void addPlayer(const string &s);
	shared_ptr<Player> findPlayer(const string &findMe);
	virtual ~Game();
	virtual int beforeTurn(Player &p) = 0;
	virtual int turn(Player &p) = 0;
	virtual int afterTurn(Player &p);
	virtual int beforeRound() = 0;
	virtual int round() = 0;
	virtual int afterRound();
	virtual int beforeGame();

	int bettingRound(bool what);
	void combinations(int index, Player &p, Hand &tmp, vector<Hand> &allCombinations, int start, int end);

	friend int main(int argc, char *argv[]);
	friend vector<string> haveCoin(vector<shared_ptr<Player>> players);
	friend bool pokerRankCompare2(shared_ptr<Player> p1, shared_ptr<Player> p2);
};

bool pokerRankCompare2(shared_ptr<Player> p1, shared_ptr<Player> p2);
vector<string> haveCoin(vector<shared_ptr<Player>> players);
