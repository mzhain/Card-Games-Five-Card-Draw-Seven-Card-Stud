// file name: TexasHoldEm.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions associated with the TexasHoldEm.cpp file

#pragma once
using namespace std;
#include "Game.h"

class TexasHoldEm : public Game {
protected:
	vector<Card> sharedCards;
public:
	TexasHoldEm();
	virtual int beforeTurn(Player &p);
	virtual int turn(Player &p);
	virtual int beforeRound();
	virtual int round();
};
