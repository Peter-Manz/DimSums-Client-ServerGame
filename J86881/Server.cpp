#include <boost/thread.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include "DimSum.h"
#include "Player.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using boost::asio::ip::tcp;

using namespace std;
using namespace J86881;
using namespace j86881;

//start of adapted code from Stackoverflow (2012)
/*! \file */
//end of adapted code

/**
*   createVectorStrings will require an starting index and iterate through the given vector every other index
*   @param lines requires a vector <string> to filter through
*   @param start integer will represent the starting index lines
*/
vector <string> createVectorStrings(vector<string>& lines, int start)
{
	vector<string> createdVector;
	for (int i = start; i < lines.size(); i += 2)
		createdVector.push_back(lines.at(i));
	return createdVector;
}
/**
*   loadFromTextFile will load the from a Serialized text file named DimSumsCollection.txt,
*   next it will Archive to a Vector of DimSum's and return the vector named DimSumsCollection
*/
vector <DimSum> loadFromTextFile() {

	vector<DimSum> DimSumsCollection;

	try {
		ifstream in("DimSumsCollection.txt");
		boost::archive::text_iarchive arch(in);
		arch >> DimSumsCollection;
	}
	catch (exception& ex) {
		cerr << ex.what();
	}
	return DimSumsCollection;
}
/**
*   loadLeaderBoards will load the from a Serialized text file named leaderBoards.txt,
*   next it will Archive to a Vector of Player's, and return the Vector named leaderBoards
*/
vector <Player> loadLeaderBoards() {
	vector<Player> leaderBoards;
	try {
		ifstream in("leaderBoard.txt");
		boost::archive::text_iarchive arch(in);
		arch >> leaderBoards;
	}
	catch (exception& ex) {
		cerr << ex.what();
	}
	return leaderBoards;
}
/**
*   updateLeaderBoards will update the leaderBoard.txt file in the case of a new HighScore,
*   next it will Archive to a Vector of Player's named leaderBoards
*	@param  leaderBoards vector of <Player> will be passed and updated
*	@param name string will represent the name of <Player>
*	@param score integer will represent the highscore of <Player>
*/
void updateLeaderBoards(vector<Player> &leaderBoards, string &name, int &score) {

	Player player(name, score);
	leaderBoards.push_back(player);

	try{
		ofstream out("leaderBoard.txt");
		boost::archive::text_oarchive ar(out);
		ar << leaderBoards;
	}
	catch (exception& ex) {
		cerr << ex.what();
	}

}
/**
*   displayLeaderBoards will call the function loadLeaderBoards and populate the leaderBoards vector
*   the vector will be sorted by the Score value, then will be reversed to set the highest value to top.
*	Finally, results will be output through stream to client. 
*	@param stream iostream must be passed to output to client
*/
void displayLeaderBoards(iostream& stream) {
	vector<Player> leaderBoards = loadLeaderBoards();
	sort(leaderBoards.begin(), leaderBoards.end());
	reverse(leaderBoards.begin(), leaderBoards.end());

	for (auto i : leaderBoards) {
		stream << i.getName() << " : " << i.getScore() << endl;
	}
	stream << "#";
}
/**
*   myrandom will return a random int value
*/
// Code Adapted From (J86881)
int myrandom(int i) {
	return (rand() % i);
}
// End of Adapted Code
/**
*   removeFromCollectionWithID will remove the matching ID in the <DimSum> Collection from the usedQuestionIDs.
*   to ensure the correct object will be removed the largest Index must be taken first from the collection.
*	@param DimSumCollection <DimSum> must be passed be updated correctly
*	@param usedQuestionsIDs <int> must be referenced to ensure integrity of Collection
*/
void removeFromCollectionWithID(vector<DimSum>& DimSumsCollection, vector<int>& usedQuestionIDs) {
	sort(usedQuestionIDs.begin(), usedQuestionIDs.end());
	reverse(usedQuestionIDs.begin(), usedQuestionIDs.end());
	for (int i = 0; i < usedQuestionIDs.size(); i++) {
		for (int j = DimSumsCollection.size() - 1; j > 0; --j) {
			if (usedQuestionIDs[i] == DimSumsCollection[j].getID()) {
				DimSumsCollection.erase(DimSumsCollection.begin() + j);
			}
		}
	}
}
/**
*   allocateSortedQuestions simultaneously assign unique values as well recording usedIDs. 
*   The DimSumCollection is first sorted from lowest answer value to highest answer value
*	The first and last answers will be added to a questionRounds and roundValue vector, while being removed from main DimSumCollection
*	The values can now be compared against to only allow values which do not match values in Vector currently
*	Once found, and four questions are allocated the removeFromCollectionWithID function will update DimSumCollection
*	@param DimSumCollection <DimSum> must be passed be updated correctly
*	@param roundValues <int> will be populated with new vector of unique values
*	@param questionRounds <string> will be populated with new vector of questions
*	@param usedQuestionsIDs <int> must be referenced to ensure integrity of Collection
*/
void allocateSortedQuestions(vector<DimSum>& DimSumsCollection, vector<int>& roundValues, vector<string>& questionRounds, vector<int> &usedQuestionIDs) {
	sort(DimSumsCollection.begin(), DimSumsCollection.end());

	roundValues.push_back(DimSumsCollection[0].getAnswer());
	questionRounds.push_back(DimSumsCollection[0].getQuestion());
	usedQuestionIDs.push_back(DimSumsCollection[0].getID());
	DimSumsCollection.erase(DimSumsCollection.begin());

	int range = DimSumsCollection.size() - 1;
	roundValues.push_back(DimSumsCollection[range].getAnswer());
	questionRounds.push_back(DimSumsCollection[range].getQuestion());
	usedQuestionIDs.push_back(DimSumsCollection[range].getID());
	DimSumsCollection.erase(DimSumsCollection.end() - 1);

	for (int i = DimSumsCollection.size() - 1; i > 0; --i) {
		if (DimSumsCollection[i].getAnswer() > roundValues.front() && DimSumsCollection[i].getAnswer() < roundValues.back()) {
			roundValues.push_back(DimSumsCollection[i].getAnswer());
			questionRounds.push_back(DimSumsCollection[i].getQuestion());
			usedQuestionIDs.push_back(DimSumsCollection[i].getID());
		}
		if (roundValues.size() == 4) {
			break;
		}
	}

	removeFromCollectionWithID(DimSumsCollection, usedQuestionIDs);
}
/**
*   valueOfQuestions will return the sum total of two values in a vector
*   the front and back values within a vector will be selected for operation and will essentially be correct answer
*	@param numbersAdded  vector <int> will be the expected parameter
*/
int valueOfQuestions(vector<int>& numbersAdded)
{
	int total = 0;
	total = numbersAdded.back() + numbersAdded.front();
	return total;
}
/**
*   answerGenerator combine two of the questions into one string, by specifying index of vector
*   the combined string will be known as generateAnswer will be returned 
*	@param questions vector<string> will be referenced to populate answers
*	@param startOfAnswer integer will be first half of answer
*	@param endOfAnswer integer will be second half of answer
*/
string answerGenerator(vector<string>& questions, int startOfAnswer, int endOfAnswer)
{
	string generatedAnswer;

	generatedAnswer = questions[startOfAnswer] + " & " + questions[endOfAnswer];
		
	return generatedAnswer;
}
/**
*   answerOptionz will utilize the function answerGenerator to make four unique answers.
*	The unique answers will be stored in a vector then returned from function
*	@param questionRounds vector<string> will be referenced to populate availableAnswers for user
*/
vector<string> answerOptionz(vector <string>& questionRounds)
{
	string correctAnswer = answerGenerator(questionRounds, 0, 3);
	string answerTwo = answerGenerator(questionRounds, 1, 2);
	string answerThree = answerGenerator(questionRounds, 2, 3);
	string answerFour = answerGenerator(questionRounds, 0, 1);

	vector <string> availableAnswers;
	availableAnswers.push_back(correctAnswer);
	availableAnswers.push_back(answerTwo);
	availableAnswers.push_back(answerThree);
	availableAnswers.push_back(answerFour);

	return availableAnswers;
}
/**
*   shuffleAndReturnKey will keep track of correctAnswer key and shuffle the available options.
*	The key code will be returned from function and compared later against user input
*	@param availableAnswers vector<string> will be passed and shuffled
*	@param correctAnswerCode integer will be compared against and returned as index j 
*	@param stream iostream must be passed to output to client
*/
int shuffleAndReturnKey(vector<string>& availableAnswers, int correctAnswerCode, iostream& stream) {

	const string correctAnswer = availableAnswers[correctAnswerCode];
	cout << "Correct Answer: " << correctAnswer << endl;
	srand(time(NULL));
	random_shuffle(availableAnswers.begin(), availableAnswers.end(), myrandom);

	for (int i = 0; i < availableAnswers.size(); i++) {
		stream << "(" << i + 1 << "): " << availableAnswers[i] << endl;
	}

	for (int j = 0; j < availableAnswers.size(); j++) {
		if (availableAnswers[j] == correctAnswer) {
			return j;
		}		
	}
	return correctAnswerCode;	
}
/**
*   pointManager will keep track of userInput and compare against the answerOptions aswell as updating score
*	The key code will be returned from function and compared later against user input
*	@param availableAnswers vector<string> will be passed and compared against
*	@param userInput integer will be put into switch statement depending on user selection
*	@param currentPoints integer will be updated in the case of a correct answer 
*	@param correctAnswerCode integer will be the index of the availableAnswer and passed to correctAnswer string
*	@param stream iostream must be passed to output to client
*/
int pointManager(vector<string>& availableAnswers,int& userInput, int &currentPoints,int &correctAnswerCode,iostream& stream) {
	
	const string correctAnswer = availableAnswers[correctAnswerCode];
	string selectedOption;

	switch (userInput) {
	case 1:
		selectedOption += availableAnswers[0];
		break;
	case 2:
		selectedOption += availableAnswers[1];
		break;
	case 3:
		selectedOption += availableAnswers[2];
		break;
	case 4:
		selectedOption += availableAnswers[3];
		break;
	default:
		stream << "Invalid Option" << endl;
	}

	if (selectedOption == correctAnswer)
	{
		stream << "*** This is Correct! Well Done! (+5) Points!! ***"<<endl;
		return currentPoints += 5;		
	}
	else {
		stream << "The Correct answer was: " << correctAnswer <<endl;
	}

	return currentPoints;
}
/**
*   dimSumGame will be the main game function and hold the main variables and their declarations.
*	the game will continue until all ten rounds have answers submitted.
*	if the currentPoints are greater then the highest score recorded on the leaderboards, the user will be asked to input name and sieralized
*	@param stream iostream must be passed to output to client aswell be the input for userAnswer and required for highscore input
*/
void dimSumsGame(iostream& stream) {

	vector <DimSum> DimSumsCollection;
	DimSumsCollection = loadFromTextFile();

	vector <Player> leaderBoards;
	leaderBoards = loadLeaderBoards();

	vector<int> usedQuestionIDs,correctAnswerPair;
	vector<string> players;
	
	int roundNumber = 1;
	int currentPoints = 0;

	do {	
		vector<int> roundValues;
		vector<string> questionRounds;
		allocateSortedQuestions(DimSumsCollection, roundValues, questionRounds, usedQuestionIDs);

		stream << "~ Round: " << roundNumber << "/10\tPoints: " << currentPoints << " ~" << endl;
		stream << "Which of the following combine together to equal the total?" << endl;
		stream << "? + ? = " << valueOfQuestions(roundValues)<< endl;

		vector<string> availableAnswers = answerOptionz(questionRounds);

		int answerKeyCode = shuffleAndReturnKey(availableAnswers, 0, stream);

		stream << "Player Answer: " <<  "#" << endl;

		int  userAnswer;
		stream >> userAnswer;

		currentPoints = pointManager(availableAnswers, userAnswer, currentPoints, answerKeyCode, stream);

		roundNumber++;

	} while (roundNumber <= 10);

	stream << "** Your High score was: " << currentPoints << " **" << endl;

	string userName;
	
	if (currentPoints >= leaderBoards.back().getScore()) {
		stream << "New High Score!!!" << endl;
		stream << "Enter in name: #" << endl;			
		stream >> userName;
		updateLeaderBoards(leaderBoards, userName, currentPoints);
	}
	displayLeaderBoards(stream);
}
/**
*	main will host the network endpoint and stream
*	if the correct condition is met determines which function the client travels next
*/
int main() {
	boost::asio::io_service ios;
	tcp::endpoint endpoint(tcp::v4(), 7000);
	tcp::acceptor acceptor(ios, endpoint);
	tcp::iostream stream;
	acceptor.accept(*stream.rdbuf());

	string message;

	do {
		stream >> message;
		stream << "Main Menu: (1)Play DimSums Game. (2)View LeaderBoards. (end)Exit.#" << endl;
		stream >> message;
		cout << message << endl;

		if (message == "1") {
			dimSumsGame(stream);
		}
		if (message == "2") {
			displayLeaderBoards(stream);
		}
	} while (!message.empty() || message != "end");
	return 0;
}
