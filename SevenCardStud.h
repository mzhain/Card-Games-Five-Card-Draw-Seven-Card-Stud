// file name: SevenCardStud.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions associated with the SevenCardStud.cpp file

#pragma once
using namespace std;
#include "Game.h"

class SevenCardStud : public Game {
	int turnNumber;
protected:

public:
	SevenCardStud();
	virtual int beforeTurn(Player &p);
	virtual int turn(Player &p);
	virtual int beforeRound();
	virtual int round();

	friend class Game;
};
