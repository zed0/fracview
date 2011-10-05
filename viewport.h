#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "colour.h"

using namespace std;

class viewport
{
	public:
		viewport(double minX=-2, double maxX=2, double minY=-2, double maxY=2, int newPixelsHigh=20, int newPixelsWide=20, int newAntialias=2);
		void render();
		void drawToTerminal();
		void drawToUnicode();
		void drawToPPM();

		vector<vector<colour> > pixelMap;
		double minX, maxX, minY, maxY;
		int pixelsHigh;
		int pixelsWide;
		int antialias;
};
#endif
