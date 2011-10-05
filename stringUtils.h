#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;

namespace stringUtils
{
	//gets the nick that a message was sent from
	string msgNick(string message);
	//gets the channel that a message was sent to
	string msgChannel(string message);
	//gets the individual words from a message
	vector<string> msgWords(string message);
	//tokenize the message
	vector<string> tokenize(string message);
	//split a message with a string delimiter
	vector<string> split(vector<string> words, string seperator);
	//checks whether the character is numeric
	bool charIsNum(char character);
	//parses the string into a int given a time formated as 1d2h3m4s etc
	int parseTime(string time);
	//joins a vector of strings, turning it in to a single space seperated string
	string joinWords(vector<string> words);
	//remove the protocol from the start of a url
	string stripProtocol(string url);
	//some functions to get various parts of a url:
	string urlHostname(string url);
	string urlPath(string url);
	string urlPort(string url);
	//make the string lower case:
	string toLower(string originalString);
	//find a string in a vector of strings
	int findWord(vector<string> haystack, string needle);

	//Due to C++ being rubish you have to declare and implement templates in the same file,
	//thus these are implemented here.

	//converts from a string to an arbitray numerical format
	template<class T> T fromString(const string& s)
	{
		istringstream stream(s);
		T t;
		stream >> t;
		return t;
	}
	//converts to a string from an arbitrary numerical format
	template<class T> string toString(const T& t)
	{
		ostringstream stream;
		stream << t;
		return stream.str();
	}
}

#endif
