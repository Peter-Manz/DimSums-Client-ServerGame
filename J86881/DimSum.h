#pragma once
#include <iostream>
#include <string>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;

namespace J86881 {
	/**
	* A DimSum class, will contain a ID, a question and a value which is the answer. The ID will always
	* be an Integer, The Question will be a string, and the answer will be a Integer.
	*/
	class DimSum {
		int ID;					/**<Identifier for DimSum */
		string question;		/**<Phrase or Question */
		int answer;				/**<Answer of the Question */
	public:
		/**
	   *   Default constructor
	   *   sets ID, and answer set to zero, and sets question to empty
	   */
		DimSum() :ID(0), question(""), answer(0) {}
		/**
		*   Overloaded constructor
		*   @param n integer type representing Identifier for DimSum
		*   @param q string type representing Phrase or Question
		*	@param a integer representing Answer of the Question
		*/
		DimSum(int n, string q, int a) :ID(n), question(q), answer(a) {}
		/**
		*   Less than operator, compatible with std::sort
		*   @param dimSumObj the DimSum object to compare against
		*   @return true the lowest value of answer
		*/
		//start of adapted code Thispointer (2022)
		bool operator < (const DimSum& dimSumObj)const {
			return answer < dimSumObj.answer;
		}
		//end of adapted code
		/**
		*   Boost Serialization Access (Friend)
		*   Allows Boost Library Access when serializing to a File
		*/
		friend class boost::serialization::access;
		/**
		*   serialize
		*   @param Archive ar accesses ID, question, and answer 
		*   @param version stored as const unsigned int
		*	@tparam Archive
		*/
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& ID;
			ar& question;
			ar& answer;
		}
		/**
		*   getID, essential to retrieve specific objects from collection and monitor used objects
		*   @return ID from DimSum object as integer
		*/
		int getID() { return ID; }
		/**
		*   getQuestion, essential to allocate questions to available answer options
		*   @return question from DimSum object as string
		*/
		string getQuestion() { return question; }
		/**
		*   getAnswer, essential to ensure answer values are monitored and not used multiple times in same group
		*	@return answer from DimSum object as a interger
		*/
		int getAnswer() { return answer; }
		/**
		*   Output operator (friend)
		*   @param os the stream object to output to
		*   @param d the DimSum object to output
		*   @return a reference to the stream object os
		*/
		friend ostream& operator <<(ostream& os, DimSum& d);
	};

	//Implementation Code-----------------------------------------------------
	ostream& operator <<(ostream& os, DimSum& d) {
		os << "ID: " << d.ID << " Question: " << d.question << " Answer: " << d.answer;
		return os;
	}
}
