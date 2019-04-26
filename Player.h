// file name: Player.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions correlating with the Player.cpp file

#pragma once
using namespace std;
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Hand.h"

struct Player {
	string name;
	Hand hand;
	unsigned int handsWon;
	unsigned int handsLost;
	int chips;

	Player(const char fileName[]);
};

ostream &operator<< (ostream &o, const Player &p);
