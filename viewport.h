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
		viewport(double newWidth=20, double newHeight=20, int newPixelsHigh=20, int newPixelsWide=20, int newAntialias=2);
		void render();
		void drawToTerminal(vector<vector<colour> > pixelMap);
		void drawToUnicode(vector<vector<colour> > pixelMap);
		void drawToPPM(vector<vector<colour> > pixelMap);

		double width;
		double height;
		int pixelsHigh;
		int pixelsWide;
		int antialias;
};
#endif
