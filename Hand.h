// file name: Hand.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions correlating with the Hand.cpp file

#pragma once
#include "Cards.h"
#include "Deck.h"
using namespace std;
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>

//defines the private member variable associated with each new Hand object, along with all of the public
//	method definitions that are then declared in Hand.cpp
//	-also includes friend functions so that the specified methods can access the private member variable
class Hand {
private:
	vector<Card> myHand;
public:
	Hand();
	Hand(const Hand &h);
	~Hand();
	Hand &operator =(const Hand &other);
	int getHandSize() const;
	bool operator ==(const Hand other) const;
	bool operator <(const Hand &other) const;
	string handToString() const;
	int printHandRank(const vector<Card> &hands);
	int getHandRank(const vector<Card> &handRef) const;
	Card operator [](size_t pos);
	Hand remove_card(size_t pos);

	friend ostream &operator<< (ostream &o, const Hand &h);
	friend void operator<< (Hand &h, Deck &d);
	friend bool pokerRankCompare(const Hand &hand1, const Hand &hand2);
	friend class Game;
	friend class SevenCardStud;
	friend class TexasHoldEm;
};

//non-member function definitions
ostream &operator<< (ostream &o, const Hand &h);
void operator<< (Hand &h, Deck &d);
bool pokerRankCompare(const Hand &hand1, const Hand &hand2);
