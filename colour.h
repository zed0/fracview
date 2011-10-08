#ifndef COLOUR_H
#define COLOUR_H

#include <cmath>
#include <string>
#include <iostream>

using namespace std;

class colour
{
	public:
		int R;
		int G;
		int B;
		int A;
		colour(int R=0, int G=0, int B=0, int A=255);
		friend ostream& operator<< (ostream &out, colour &cPoint);
		bool operator==(const colour &other) const;
		bool operator!=(const colour &other) const;
		int toAnsi();
		colour add(colour target);
		colour product(double target);
		colour productNoA(double target);
};

#endif
