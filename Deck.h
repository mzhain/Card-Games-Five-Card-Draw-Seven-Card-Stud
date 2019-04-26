// file name: Deck.h
// author 1: Emme Wiederhold: ewiederhold@wustl.edu
// author 2: Mattison Hain: mzhain@wustl.edu
// content: function and variable definitions correlating with the Deck.cpp file

#pragma once
#include "Cards.h"
using namespace std;
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>


//forward declaration of class Hand
class Hand;


//defines the private member variable associated with each new Deck object, along with all of the public
//	method definitions that are then declared in Deck.cpp
//	-also includes friend functions so that the specified methods can access the private member variable
class Deck {
private:
	vector<Card> myDeck;
public:
	Deck();
	Deck(string fileName);
	~Deck();
	int loadFile(string fileName);
	void shuffleDeck();
	int getDeckSize() const;
	Deck add_card(Card card);
	Card remove_card();

	friend ostream &operator<< (ostream &o, const Deck &d);
	friend void operator<< (Hand &h, Deck &d);

};

//non-member function definition
ostream &operator<< (ostream &o, const Deck &d);
