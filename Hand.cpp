// file name: Hand.cpp
// author: Emme Wiederhold: ewiederhold@wustl.edu
// content: twelve methods to accompany Hand objects and compliment the program's functionality
//			includes default constructor, copy constructor, and destructor

#include "stdafx.h"
#include "Hand.h"
#include "Deck.h"

//because I don't use a member initialization list here, I'm using the fact that the private member
//	variable "myHand" is being default initialized to be empty
Hand::Hand() {

}

//copy constructor
Hand::Hand(const Hand  &h) {
	myHand = h.myHand;
}

//destructor
Hand::~Hand() {

}

//for the assignment operator, first check to see if the two Hands are equal, and if they are
//	we don't need to do anything - if they aren't equivalent, clear out this.myHand and resize it
//	so that it can contain the same number of elements as the "other" Hand, and then copy the values
//	held in other into this.myHand
Hand &Hand::operator =(const Hand &other) {
	if (&other != this) {
		myHand.clear();
		myHand.resize(other.getHandSize());
		copy(other.myHand.begin(), other.myHand.end(), myHand.begin());
	}
	return *this;
}

int Hand::getHandSize() const {
	return myHand.size();
}

//the equivalence operator assumes that the Hands are both already sorted
//	returns true if and only if exactly the same sequence of Cards appears in both objects
bool Hand::operator ==(const Hand other) const {
	//because I've defined an equivalence operator for my Card objects, I can simply compare the vectors directly
	return (myHand == other.myHand);
}

//the less than operator returns true if and only if the sequence of Cards in the object on which
//	the operator was called appear before the sequence of Cards in the passed object according to
//	a lexical ordering - must take into account Hands of sizes that aren't necessarily 5 Cards
bool Hand::operator <(const Hand &other) const {
	//first check if the two vectors of Cards are equal
	if (myHand == other.myHand) {
		return false;
	}
	//if they aren't then just use the vector less than operator to compare the two Hands
	//vector less than operator:
	//	If the vectors have differing sizes, but the elements that are in common are equal, then the
	//	vector with fewer elements is less than the one with more elements. If the elements have
	//	differing values, then the relationship between the vectors is determined by the relationship
	//	between the first elements that differ
	else {
		return(myHand < other.myHand);
	}
}

//returns a string that represents the Hand in a more user-friendly, easy to read way
string Hand::handToString() const {
	string result = "";
	for (int i = 0; i < getHandSize(); ++i) {
		result += myHand[i].cardToString();
		result += " ";
	}
	return result;
}

//this output stream insertion operator allows us to more easily print Hand objects to the output stream
ostream &operator<< (ostream &o, const Hand &h) {
	o << h.handToString();
	return o;
}

//this insertion operator is used to take a Card from the end of the Deck and add it to a players Hand
void operator<< (Hand &h, Deck &d) {
	//push the last Card in the Deck object to the end of the Hand object
	h.myHand.push_back(d.myDeck.back());
	//make sure to actually remove the last Card object from the end of the Deck object
	d.myDeck.pop_back();
	//then ensure that the Hand is still properly sorted
	sort(h.myHand.begin(), h.myHand.end());
}

//returns true if and only if the first Hand object ranks higher than the second Hand object given the specified
//	poker hand ranking definition --- assumes the Hands come in sorted, meaning the highest ranked card will
//	be the fifth (last) card in the hand
bool pokerRankCompare(const Hand &hand1, const Hand &hand2) {
	int hand1rank = hand1.getHandRank(hand1.myHand);
	int hand2rank = hand2.getHandRank(hand2.myHand);
	//if the ranks are equal, perform the correct operation according to the Hand rank type
	if (hand1rank == hand2rank) {
		//if Straight Flush or Straight, the two Hands are sorted by the highest Card in each Hand
		//	the lowest rank, 2, is value 0 in the Rank enum, where ace, the highest rank, is value 12
		if (hand1rank == straightFlush || hand1rank == straight) {
			return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
		}
		//if Four of a Kind, the Hands are sorted by the rank of the four cards that have the same rank
		if (hand1rank == fourOfAKind) {
			//if the four cards that have the same rank are at the beginning of the first Hand
			if (hand1.myHand[firstCardInHand] == hand1.myHand[secondCardInHand]) {
				//if the four cards that have the same rank are at the beginning of the second Hand
				if (hand2.myHand[firstCardInHand] == hand2.myHand[secondCardInHand]) {
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				else {	//if the four cards that have the same rank are at the end of the second Hand
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[secondCardInHand].currentRank);
				}
			}
			else {	//if the four cards that have the same rank are at the end of the first Hand
					//if the four cards that have the same rank are at the beginning of the second Hand
				if (hand2.myHand[firstCardInHand] == hand2.myHand[secondCardInHand]) {
					return (hand1.myHand[secondCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				else {	//if the four cards that have the same rank are at the end of the second Hand
					return (hand1.myHand[secondCardInHand].currentRank > hand2.myHand[secondCardInHand].currentRank);
				}
			}
		}
		//if Full House, the Hands are sorted by the rank of the three cards that have the same rank, then by the
		//	rank of the two cards that have the same rank
		if (hand1rank == fullHouse) {
			//if the three cards that have the same rank are at the BEGINNING of the FIRST hand
			if (hand1.myHand[secondCardInHand].currentRank == hand1.myHand[thirdCardInHand].currentRank) {
				//if the three cards that have the same rank are at the BEGINNING of the SECOND hand
				if (hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) {
					//if the rank of the three cards that have the same rank for both hands are the same,
					//	check the rank of the two cards that have the same rank for both hands
					if (hand1.myHand[firstCardInHand].currentRank == hand2.myHand[firstCardInHand].currentRank) {
						return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
					}
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				//if the three cards that have the same rank are at the END of the SECOND hand
				else {
					//if the rank of the three cards that have the same rank for both hands are the same,
					//	check the rank of the two cards that have the same rank for both hands
					if (hand1.myHand[firstCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank) {
						return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
					}
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
				}
			}
			//if the three cards that have the same rank are the END of the FIRST hand
			else {
				//if the three cards that have the same rank are at the BEGINNING of the SECOND hand
				if (hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) {
					//if the rank of the three cards that have the same rank for both hands are the same,
					//	check the rank of the two cards that have the same rank for both hands
					if (hand1.myHand[fifthCardInHand].currentRank == hand2.myHand[firstCardInHand].currentRank) {
						return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
					}
					return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				else {	//if the three cards that have the same rank are at the END of the SECOND hand
						//if the rank of the three cards that have the same rank for both hands are the same,
						//	check the rank of the two cards that have the same rank for both hands
					if (hand1.myHand[fifthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank) {
						return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
					}
					return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
				}
			}
		}
		//if Flush or No Rank, the Hands are sorted by highest ranking card, then by the next highest ranking card, etc.
		if (hand1rank == flush_ || hand1rank == noRank) {
			//if the ranks of the highest ranked card in each hand are equal, then compare the next highest ranked card
			//	until their ranks don't match, or just return false if the hands are totally equal
			if (hand1.myHand[fifthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank) {
				if (hand1.myHand[fourthCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank) {
					if (hand1.myHand[thirdCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) {
						if (hand1.myHand[secondCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) {
							if (hand1.myHand[firstCardInHand].currentRank == hand2.myHand[firstCardInHand].currentRank) {
								return false;
							}
							else {
								return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
							}
						}
						else {
							return (hand1.myHand[secondCardInHand].currentRank > hand2.myHand[secondCardInHand].currentRank);
						}
					}
					else {
						return (hand1.myHand[thirdCardInHand].currentRank > hand2.myHand[thirdCardInHand].currentRank);
					}
				}
				else {
					return (hand1.myHand[fourthCardInHand].currentRank > hand2.myHand[fourthCardInHand].currentRank);
				}
			}
			else {
				return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
			}
		}
		//if Three of a Kind, the Hands are sorted by the rank of the three cards that have the same rank
		if (hand1rank == threeOfAKind) {
			//the three cards that have the same rank are at the BEGINNING of the FIRST hand
			if ((hand1.myHand[firstCardInHand].currentRank == hand1.myHand[secondCardInHand].currentRank) && (hand1.myHand[secondCardInHand].currentRank == hand1.myHand[thirdCardInHand].currentRank)) {
				//the three cards that have the same rank are at the BEGINNING of the SECOND hand
				if ((hand2.myHand[firstCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) && (hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank)) {
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				//the three cards that have the same rank are in the MIDDLE of the SECOND hand
				if ((hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) && (hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank)) {
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[thirdCardInHand].currentRank);
				}
				//the three cards that have the same rank are at the END of the SECOND hand
				if ((hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank) && (hand2.myHand[fourthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank)) {
					return (hand1.myHand[firstCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
				}
				return false;
			}
			//the three cards that have the same rank are in the MIDDLE of the FIRST hand
			if ((hand1.myHand[secondCardInHand].currentRank == hand1.myHand[thirdCardInHand].currentRank) && (hand1.myHand[thirdCardInHand].currentRank == hand1.myHand[fourthCardInHand].currentRank)) {
				//the three cards that have the same rank are at the BEGINNING of the SECOND hand
				if ((hand2.myHand[firstCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) && (hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank)) {
					return (hand1.myHand[thirdCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				//the three cards that have the same rank are in the MIDDLE of the SECOND hand
				if ((hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) && (hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank)) {
					return (hand1.myHand[thirdCardInHand].currentRank > hand2.myHand[thirdCardInHand].currentRank);
				}
				//the three cards that have the same rank are at the END of the SECOND hand
				if ((hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank) && (hand2.myHand[fourthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank)) {
					return (hand1.myHand[thirdCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
				}
				return false;
			}
			//the three cards that have the same rank are at the END of the FIRST hand
			if ((hand1.myHand[thirdCardInHand].currentRank == hand1.myHand[fourthCardInHand].currentRank) && (hand1.myHand[fourthCardInHand].currentRank == hand1.myHand[fifthCardInHand].currentRank)) {
				//the three cards that have the same rank are at the BEGINNING of the SECOND hand
				if ((hand2.myHand[firstCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) && (hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank)) {
					return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[firstCardInHand].currentRank);
				}
				//the three cards that have the same rank are in the MIDDLE of the SECOND hand
				if ((hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) && (hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank)) {
					return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[thirdCardInHand].currentRank);
				}
				//the three cards that have the same rank are at the END of the SECOND hand
				if ((hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank) && (hand2.myHand[fourthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank)) {
					return (hand1.myHand[fifthCardInHand].currentRank > hand2.myHand[fifthCardInHand].currentRank);
				}
				return false;
			}
			return false;
		}
		//if Two Pairs, the Hands are sorted by the rank of the higher pair, then by the rank of the lower pair,
		//	then by the rank of the fifth card
		if (hand1rank == twoPairs) {
			int higherPairHand1;
			int lowerPairHand1;
			int unpairedHand1;
			//if the FIRST Hand contains PAIR PAIR UNPAIRED
			if ((hand1.myHand[firstCardInHand].currentRank == hand1.myHand[secondCardInHand].currentRank) && (hand1.myHand[thirdCardInHand].currentRank == hand1.myHand[fourthCardInHand].currentRank)) {
				higherPairHand1 = thirdCardInHand;
				lowerPairHand1 = firstCardInHand;
				unpairedHand1 = fifthCardInHand;
			}
			//if the FIRST Hand contains PAIR UNPAIRED PAIR
			if ((hand1.myHand[firstCardInHand].currentRank == hand1.myHand[secondCardInHand].currentRank) && (hand1.myHand[fourthCardInHand].currentRank == hand1.myHand[fifthCardInHand].currentRank)) {
				higherPairHand1 = fourthCardInHand;
				lowerPairHand1 = firstCardInHand;
				unpairedHand1 = thirdCardInHand;
			}
			//if the FIRST Hand contains UNPAIRED PAIR PAIR
			if ((hand1.myHand[secondCardInHand].currentRank == hand1.myHand[thirdCardInHand].currentRank) && (hand1.myHand[fourthCardInHand].currentRank == hand1.myHand[fifthCardInHand].currentRank)) {
				higherPairHand1 = fourthCardInHand;
				lowerPairHand1 = secondCardInHand;
				unpairedHand1 = firstCardInHand;
			}
			int higherPairHand2;
			int lowerPairHand2;
			int unpairedHand2;
			//if the SECOND Hand contains PAIR PAIR UNPAIRED
			if ((hand2.myHand[firstCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) && (hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank)) {
				higherPairHand2 = thirdCardInHand;
				lowerPairHand2 = firstCardInHand;
				unpairedHand2 = fifthCardInHand;
			}
			//if the SECOND Hand contains PAIR UNPAIRED PAIR
			if ((hand2.myHand[firstCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) && (hand2.myHand[fourthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank)) {
				higherPairHand2 = fourthCardInHand;
				lowerPairHand2 = firstCardInHand;
				unpairedHand2 = thirdCardInHand;
			}
			//if the SECOND Hand contains UNPAIRED PAIR PAIR
			if ((hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) && (hand2.myHand[fourthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank)) {
				higherPairHand2 = fourthCardInHand;
				lowerPairHand2 = secondCardInHand;
				unpairedHand2 = firstCardInHand;
			}
			//now actually compare the Hands - if the rank of the higher pairs is equal between the two Hands, compare the
			//	rank of the lower pairs, and if those are equal compare the unpaired card of each Hand or return false if
			//	 they are truly equal Hands
			if (hand1.myHand[higherPairHand1].currentRank == hand2.myHand[higherPairHand2].currentRank) {
				if (hand1.myHand[lowerPairHand1].currentRank == hand2.myHand[lowerPairHand2].currentRank) {
					if (hand1.myHand[unpairedHand1].currentRank == hand2.myHand[unpairedHand2].currentRank) {
						return false;
					}
					else {
						return (hand1.myHand[unpairedHand1].currentRank > hand2.myHand[unpairedHand2].currentRank);
					}
				}
				else {
					return (hand1.myHand[lowerPairHand1].currentRank > hand2.myHand[lowerPairHand2].currentRank);
				}
			}
			else {
				return (hand1.myHand[higherPairHand1].currentRank > hand2.myHand[higherPairHand2].currentRank);
			}
		}
		//if One Pair, the Hands are sorted by the rank of the pair, then by the highest unpaired card, then the
		//	next highest unpaired card, and then by the next highest unpaired card
		if (hand1rank == onePair) {
			int pairHand1;
			int highestUnpairedHand1;
			int secondHighestUnpairedHand1;
			int thirdHighestUnpairedHand1;
			//if the pair is at the BEGINNING of the FIRST hand
			if (hand1.myHand[firstCardInHand].currentRank == hand1.myHand[secondCardInHand].currentRank) {
				pairHand1 = firstCardInHand;
				highestUnpairedHand1 = fifthCardInHand;
				secondHighestUnpairedHand1 = fourthCardInHand;
				thirdHighestUnpairedHand1 = thirdCardInHand;
			}
			//if the pair is at the BEGINNING MIDDLE spot of the FIRST hand
			if (hand1.myHand[secondCardInHand].currentRank == hand1.myHand[thirdCardInHand].currentRank) {
				pairHand1 = secondCardInHand;
				highestUnpairedHand1 = fifthCardInHand;
				secondHighestUnpairedHand1 = fourthCardInHand;
				thirdHighestUnpairedHand1 = firstCardInHand;
			}
			//if the pair is at the END MIDDLE spot of the FIRST hand
			if (hand1.myHand[thirdCardInHand].currentRank == hand1.myHand[fourthCardInHand].currentRank) {
				pairHand1 = thirdCardInHand;
				highestUnpairedHand1 = fifthCardInHand;
				secondHighestUnpairedHand1 = secondCardInHand;
				thirdHighestUnpairedHand1 = firstCardInHand;
			}
			//if the pair is at the END of the FIRST hand
			if (hand1.myHand[fourthCardInHand].currentRank == hand1.myHand[fifthCardInHand].currentRank) {
				pairHand1 = fourthCardInHand;
				highestUnpairedHand1 = thirdCardInHand;
				secondHighestUnpairedHand1 = secondCardInHand;
				thirdHighestUnpairedHand1 = firstCardInHand;
			}
			int pairHand2;
			int highestUnpairedHand2;
			int secondHighestUnpairedHand2;
			int thirdHighestUnpairedHand2;
			//if the pair is at the BEGINNING of the SECOND hand
			if (hand2.myHand[firstCardInHand].currentRank == hand2.myHand[secondCardInHand].currentRank) {
				pairHand2 = firstCardInHand;
				highestUnpairedHand2 = fifthCardInHand;
				secondHighestUnpairedHand2 = fourthCardInHand;
				thirdHighestUnpairedHand2 = thirdCardInHand;
			}
			//if the pair is at the BEGINNING MIDDLE spot of the SECOND hand
			if (hand2.myHand[secondCardInHand].currentRank == hand2.myHand[thirdCardInHand].currentRank) {
				pairHand2 = secondCardInHand;
				highestUnpairedHand2 = fifthCardInHand;
				secondHighestUnpairedHand2 = fourthCardInHand;
				thirdHighestUnpairedHand2 = firstCardInHand;
			}
			//if the pair is at the END MIDDLE spot of the SECOND hand
			if (hand2.myHand[thirdCardInHand].currentRank == hand2.myHand[fourthCardInHand].currentRank) {
				pairHand2 = thirdCardInHand;
				highestUnpairedHand2 = fifthCardInHand;
				secondHighestUnpairedHand2 = secondCardInHand;
				thirdHighestUnpairedHand2 = firstCardInHand;
			}
			//if the pair is at the END of the SECOND hand
			if (hand2.myHand[fourthCardInHand].currentRank == hand2.myHand[fifthCardInHand].currentRank) {
				pairHand2 = fourthCardInHand;
				highestUnpairedHand2 = thirdCardInHand;
				secondHighestUnpairedHand2 = secondCardInHand;
				thirdHighestUnpairedHand2 = firstCardInHand;
			}
			//now actually compare the Hands - if the rank of the pairs is equal, move on to compare the highest ranked
			//	unpaired card, etc until the ranks don't match or returns false if the hands are truly equal
			if (hand1.myHand[pairHand1].currentRank == hand2.myHand[pairHand2].currentRank) {
				if (hand1.myHand[highestUnpairedHand1].currentRank == hand2.myHand[highestUnpairedHand2].currentRank) {
					if (hand1.myHand[secondHighestUnpairedHand1].currentRank == hand2.myHand[secondHighestUnpairedHand2].currentRank) {
						if (hand1.myHand[thirdHighestUnpairedHand1].currentRank == hand2.myHand[thirdHighestUnpairedHand2].currentRank) {
							return false;
						}
						else {
							return (hand1.myHand[thirdHighestUnpairedHand1].currentRank > hand2.myHand[thirdHighestUnpairedHand2].currentRank);
						}
					}
					else {
						return(hand1.myHand[secondHighestUnpairedHand1].currentRank > hand2.myHand[secondHighestUnpairedHand2].currentRank);
					}
				}
				else {
					return (hand1.myHand[highestUnpairedHand1].currentRank > hand2.myHand[highestUnpairedHand2].currentRank);
				}
			}
			else {
				return (hand1.myHand[pairHand1].currentRank > hand2.myHand[pairHand2].currentRank);
			}
		}
		return false;
	}
	//in my "handRanks" enum, the highest valued poker hand (straight flush) is value 0 and the lowest (no rank) is value 8
	//	warranting the use of the less than symbol here
	else {
		return (hand1.getHandRank(hand1.myHand) < hand2.getHandRank(hand2.myHand));
	}
}

/*
* printHandRank takes in a reference to a constant vector of Cards that only contains valid poker hands
*	-since we know that each valid poker hand has five Cards, we process five Cards at a time,
*		first copying them into a temporary vector so that each hand can be sorted independently
*		of the other hands
*		-this sorted hand is then passed to the getHandRank function, where the rank of the poker hand is
*			evaluated and passed back to this function, where an equivalent text string is printed
*			to the output stream for the user to see
*/
int Hand::printHandRank(const vector<Card> &hands) {
	//since the "hands" vector is made up of hands (each hand being 5 consecutive Cards),
	//	the total number of hands in the vector can be found by dividing the length by 5
	int numHands = hands.size() / numCardsInHand;
	//the for loop is executed once for every hand in the "hands" vector
	for (int i = 0; i < numHands; ++i) {
		vector<Card> tmp;
		//fill the tmp vector with the next five Cards from the hands vector (five Cards makes up one hand)
		for (int j = 0; j < 5; ++j) {
			tmp.push_back(hands.at(numCardsInHand * i + j));
		}
		sort(tmp.begin(), tmp.end());
		switch (getHandRank(tmp)) {
		case straightFlush: cout << "Straight Flush" << endl; break;
		case fourOfAKind: cout << "Four of a Kind" << endl; break;
		case fullHouse: cout << "Full House" << endl; break;
		case flush_: cout << "Flush" << endl; break;
		case straight: cout << "Straight" << endl; break;
		case threeOfAKind: cout << "Three of a Kind" << endl; break;
		case twoPairs: cout << "Two Pairs" << endl; break;
		case onePair: cout << "One Pair" << endl; break;
		case noRank: cout << "No Rank" << endl; break;
		}
	}
	return success;
}


/*
* getHandRank takes in a reference to a constant vector of Cards that contains a single valid poker hand
*	-for ease of reference, the rank and suit of each hand is fetched at the beginning of the method
*	-the hand is then evaluated until it matches the requirements for a certain poker hand rankings,
*		as described above each if statement evaluation
*		-the corresponding poker hand ranking is returned, and if the hand doesn't match any of the
*			given rankings, noRank is returned
*/
int Hand::getHandRank(const vector<Card> &handRef) const {
	int firstCardRANK = handRef[firstCardInHand].currentRank;
	int firstCardSUIT = handRef[firstCardInHand].currentSuit;
	int secondCardRANK = handRef[secondCardInHand].currentRank;
	int secondCardSUIT = handRef[secondCardInHand].currentSuit;
	int thirdCardRANK = handRef[thirdCardInHand].currentRank;
	int thirdCardSUIT = handRef[thirdCardInHand].currentSuit;
	int fourthCardRANK = handRef[fourthCardInHand].currentRank;
	int fourthCardSUIT = handRef[fourthCardInHand].currentSuit;
	int fifthCardRANK = handRef[fifthCardInHand].currentRank;
	int fifthCardSUIT = handRef[fifthCardInHand].currentSuit;
	//checks specifically for a circular straight flush
	if ((fifthCardRANK == Card::ace) && (firstCardRANK == Card::two) && (secondCardRANK == Card::three) && (thirdCardRANK == Card::four) && (fourthCardRANK == Card::five) &&
		(firstCardSUIT == secondCardSUIT) && (secondCardSUIT == thirdCardSUIT) && (thirdCardSUIT == fourthCardSUIT) && (fourthCardSUIT == fifthCardSUIT)) {
		return straightFlush;
	}
	//if all the ranks are consecutive and all cards have the same suit, then the hand is a straight flush
	if ((firstCardRANK + 1 == secondCardRANK) && (secondCardRANK + 1 == thirdCardRANK) && (thirdCardRANK + 1 == fourthCardRANK) && (fourthCardRANK + 1 == fifthCardRANK)) {
		if ((firstCardSUIT == secondCardSUIT) && (secondCardSUIT == thirdCardSUIT) && (thirdCardSUIT == fourthCardSUIT) && (fourthCardSUIT == fifthCardSUIT)) {
			return straightFlush;
		}
	}
	//if four of the five cards have the same rank, then the hand is four of a kind
	if (((firstCardRANK == secondCardRANK) && (secondCardRANK == thirdCardRANK) && (thirdCardRANK == fourthCardRANK)) ||
		((secondCardRANK == thirdCardRANK) && (thirdCardRANK == fourthCardRANK) && (fourthCardRANK == fifthCardRANK))) {
		return fourOfAKind;
	}
	//if three of the five cards have the same rank and the other two have the same rank, then the hand is a full house
	if (((firstCardRANK == secondCardRANK) && (secondCardRANK == thirdCardRANK) && (fourthCardRANK == fifthCardRANK)) ||
		((firstCardRANK == secondCardRANK) && (thirdCardRANK == fourthCardRANK) && (fourthCardRANK == fifthCardRANK))) {
		return fullHouse;
	}
	//if the five cards are not all consecutive in rank but have the same suit, then the hand is a flush
	if ((firstCardRANK + 1 != secondCardRANK) || (secondCardRANK + 1 != thirdCardRANK) || (thirdCardRANK + 1 != fourthCardRANK) || (fourthCardRANK + 1 != fifthCardRANK)) {
		if ((firstCardSUIT == secondCardSUIT) && (secondCardSUIT == thirdCardSUIT) && (thirdCardSUIT == fourthCardSUIT) && (fourthCardSUIT == fifthCardSUIT)) {
			return flush_;
		}
	}
	//checks specifically for a circular straight hand ranking
	//we can assume the suits are not equal because if they were, it would've been declared a straight flush above
	if ((fifthCardRANK == Card::ace) && (firstCardRANK == Card::two) && (secondCardRANK == Card::three) && (thirdCardRANK == Card::four) && (fourthCardRANK == Card::five)) {
		return straight;
	}
	//if the five cards are all consecutive in rank but do not all have the same suit, then the hand is a straight
	if ((firstCardRANK + 1 == secondCardRANK) && (secondCardRANK + 1 == thirdCardRANK) && (thirdCardRANK + 1 == fourthCardRANK) && (fourthCardRANK + 1 == fifthCardRANK)) {
		if ((firstCardSUIT != secondCardSUIT) || (secondCardSUIT == thirdCardSUIT) || (thirdCardSUIT == fourthCardSUIT) || (fourthCardSUIT == fifthCardSUIT)) {
			return straight;
		}
	}
	//if three of the five cards have the same rank and the other two cards have different ranks, then the hand is three of a kind
	if (((firstCardRANK == secondCardRANK) && (secondCardRANK == thirdCardRANK) && (fourthCardRANK != fifthCardRANK)) ||
		((secondCardRANK == thirdCardRANK) && (thirdCardRANK == fourthCardRANK) && (firstCardRANK != fifthCardRANK)) ||
		((thirdCardRANK == fourthCardRANK) && (fourthCardRANK == fifthCardRANK) && (firstCardRANK != secondCardRANK))) {
		return threeOfAKind;
	}
	//if two of the five cards have the same rank, another of the two cards are of a different rank,
	//	and the other card is of a different rank, then the hand is two pairs
	if ((firstCardRANK == secondCardRANK) && (thirdCardRANK == fourthCardRANK) && (fifthCardRANK != firstCardRANK) && (fifthCardRANK != thirdCardRANK) ||
		(secondCardRANK == thirdCardRANK) && (fourthCardRANK == fifthCardRANK) && (firstCardRANK != secondCardRANK) && (firstCardRANK != fourthCardRANK) ||
		(firstCardRANK == secondCardRANK) && (fourthCardRANK == fifthCardRANK) && (thirdCardRANK != firstCardRANK) && (thirdCardRANK != fourthCardRANK)) {
		return twoPairs;
	}
	//if two of the five cards have the same rank and the other three cards have different ranks,
	//	then the hand is a pair
	if (((firstCardRANK == secondCardRANK) && (thirdCardRANK != firstCardRANK) && (fourthCardRANK != firstCardRANK) &&
		(fourthCardRANK != thirdCardRANK) && (fifthCardRANK != firstCardRANK) && (fifthCardRANK != thirdCardRANK) && (fifthCardRANK != fourthCardRANK)) ||
		((secondCardRANK == thirdCardRANK) && (fourthCardRANK != secondCardRANK) && (fifthCardRANK != secondCardRANK) &&
		(fifthCardRANK != fourthCardRANK) && (firstCardRANK != secondCardRANK) && (firstCardRANK != fourthCardRANK) && (firstCardRANK != fifthCardRANK)) ||
			((thirdCardRANK == fourthCardRANK) && (fifthCardRANK != thirdCardRANK) && (firstCardRANK != thirdCardRANK) &&
		(firstCardRANK != fifthCardRANK) && (secondCardRANK != thirdCardRANK) && (secondCardRANK != fifthCardRANK) && (secondCardRANK != firstCardRANK)) ||
				((fourthCardRANK == fifthCardRANK) && (firstCardRANK != fourthCardRANK) && (secondCardRANK != fourthCardRANK) &&
		(secondCardRANK != firstCardRANK) && (thirdCardRANK != fourthCardRANK) && (thirdCardRANK != firstCardRANK) && (thirdCardRANK != secondCardRANK))) {
		return onePair;
	}
	//if the hand wasn't any of the above, it doesn't have a rank!
	return noRank;
}

Card Hand::operator[](size_t pos)
{
	if (pos >= myHand.size()) {
		throw HandReferencingOutOfBounds;
	}
	return myHand[pos];
}

Hand Hand::remove_card(size_t pos)
{
	if (pos >= myHand.size()) {
		throw HandReferencingOutOfBounds;
	}
	else {
		myHand.erase(myHand.begin() + pos);
	}
	return *this;
}
