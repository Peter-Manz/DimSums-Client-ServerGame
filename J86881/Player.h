#pragma once
#include <iostream>
#include <string>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;

namespace j86881 {
	/**
	* A Player class, will contain a name, and a high score. The name will always
	* be an String, The score will be a Integer.
	*/
	class Player {
		string name;	/**<Name given by the Player*/
		int score;		/**<Personal High score*/

	public:
		/**
		*   Default constructor
		*   sets name to empty, and score to zero
		*/
		Player() :name(""), score(0) {}
		/**
		*   Overloaded constructor
		*   @param n string type representing name for Player
		*   @param s integer type representing high score
		*/
		Player(string n, int s) :name(n), score(s) {}
		/**
		*   Less than operator, compatible with std::sort
		*   @param playerObj the Player object to compare against
		*   @return true the lowest value of score
		*/
		//start of adapted code Thispointer (2022)
		bool operator < (const Player& playerObj)const {
			return score < playerObj.score;
		}
		//end of adapted code
		/**
		*   Boost Serialization Access (Friend)
		*   Allows Boost Library Access when serializing to a File
		*/
		friend class boost::serialization::access;
		/**
		*   serialize
		*   @param Archive ar accesses name, and score
		*   @param version stored as const unsigned int
		*	@tparam Archive
		*/
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& name;
			ar& score;
		}
		/**
		*   getName, essential to record name of player
		*	@return name from Player object as a string
		*/
		int getScore() { return score; }
		/**
		*   getName, essential to record name of player
		*	@return name from Player object as a string
		*/
		string getName() { return name; }
		/**
		*   Output operator (friend)
		*   @param os the stream object to output to
		*   @param p the Player object to output
		*   @return a reference to the stream object os
		*/
		friend ostream& operator <<(ostream& os, Player& p);
	};
	//Implementation Code-----------------------------------------------------
	ostream& operator <<(ostream& os, Player& p) {
		os << "Name: " << p.name << "Score: " << p.score;
		return os;
	}
}