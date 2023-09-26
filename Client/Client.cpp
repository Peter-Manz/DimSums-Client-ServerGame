#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
int main() {
	tcp::iostream streaming("127.0.0.1", "7000");
	cout <<"Connect To Server by Pressing any character: ";
	string userInput;

	do {
		cin >> userInput;
		cin.ignore(100, '\n');
		streaming << userInput << endl;	
		string recv;
		getline(streaming, recv,'#');
		cout << recv << endl;

	} while (userInput != "end");
	
	return 0;
}