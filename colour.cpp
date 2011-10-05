#include "colour.h"

using namespace std;

colour::colour(int newR, int newG, int newB, int newA)
{
	R = newR<0?0:newR>255?255:newR;
	G = newG<0?0:newG>255?255:newG;
	B = newB<0?0:newB>255?255:newB;
	A = newA<0?0:newA>255?255:newA;
}

ostream& operator<< (ostream &out, colour &cPoint)
{
	out << "(" << cPoint.R << "," <<
		cPoint.G << "," <<
		cPoint.B << "," << cPoint.A << ")";
	return out;
}

colour colour::add(colour target)
{
	return colour(R+target.R, G+target.G, B+target.B, A+target.A);
}

int colour::toAnsi()
{
	int closest6[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	int closestR = closest6[R];
	int closestG = closest6[G];
	int closestB = closest6[B];
	int result = 16 + closestR*36 + closestG*6 + closestB;
	return result;
}

colour colour::product(double target)
{
	return colour(R*target, G*target, B*target, A*target);
}

colour colour::productNoA(double target)
{
	return colour(R*target, G*target, B*target, A);
}
