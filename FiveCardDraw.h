// file name: FiveCardDraw.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions associated with the FiveCardDraw.cpp file

#pragma once
#include "Deck.h"
#include "Game.h"
using namespace std;

class FiveCardDraw : public Game {
protected:

public:
	FiveCardDraw();
	virtual int beforeTurn(Player &p);
	virtual int turn(Player &p);
	virtual int beforeRound();
	virtual int round();

	friend class Game;
};
