#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "stringUtils.h"
#include "colour.h"

using namespace std;

class viewport
{
	public:
		viewport(double minX=-2, double maxX=2, double minY=-2, double maxY=2, double stretch_k=1, double theta=0, int newPixelsHigh=20, int newPixelsWide=20, int newAntialias=2, double colourScale=128, int colourOffset=0);
		void render();
		void drawToTerminal();
		void drawToUnicode();
		void drawToPPM();
		void drawToFile(string filename);

		vector<vector<colour> > pixelMap;
		double minX, maxX, minY, maxY, stretch_k, theta;
		int pixelsHigh;
		int pixelsWide;
		int antialias;
		double colourScale;
		int colourOffset;
};
#endif
