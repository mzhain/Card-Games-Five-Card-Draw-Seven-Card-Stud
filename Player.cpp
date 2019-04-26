// file name: Player.cpp
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: two methods to accompany Player objects and compliment the program's functionality
//		includes constructor

#include "stdafx.h"
#include "Player.h"
#include "Cards.h"

//non-member insertion operator to correctly print out a player's information
ostream & operator<<(ostream & o, const Player & p)
{
	cout << p.name << " has " << p.handsWon << " WIN(S) and " << p.handsLost << " LOSS(ES) and " << p.chips << "CHIPS" << endl;
	return o;
}

//constructing a player object checks to see if that player has a file associated with them already
//	and initializes their member variables accordingly
Player::Player(const char fileName[]) : name(fileName), hand(Hand()), handsWon(0), handsLost(0), chips(20)
{
	string input = fileName + string(".txt");
	ifstream ifs;
	ifs.open(input);
	if (ifs.is_open()) {
		string currentLine;
		string nameInput;
		unsigned int winInput;
		unsigned int lossInput;
		unsigned int chipInput;
		int proceed = 0;
		while (getline(ifs, currentLine)) {
			istringstream iss(currentLine);
			if (iss >> nameInput) {
				proceed++;
			}
			if (iss >> winInput) {
				proceed++;
			}
			if (iss >> lossInput) {
				proceed++;
			}
			if (iss >> chipInput) {      //LAB4
				proceed++;
			}
		}
		//only assigns values that were read in if all values were found,
		//	otherwise the values stay the same values as in the member initialization list
		if (proceed == 4) {
			name = nameInput;
			handsWon = winInput;
			handsLost = lossInput;
			chips = chipInput;      //LAB4
		}
	}
}
