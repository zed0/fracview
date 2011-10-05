#include "viewport.h"

using namespace std;

viewport::viewport(double newWidth, double newHeight, int newPixelsHigh, int newPixelsWide, int newAntialias)
{
	width = newWidth;
	height = newHeight;
	pixelsHigh = newPixelsHigh;
	pixelsWide = newPixelsWide;
	antialias = newAntialias;
}

void viewport::render()
{
	vector<vector<colour> > pixelMap(pixelsHigh, vector<colour>(pixelsWide));
	for(int i=0; i<pixelsHigh; ++i)
	{
		for(int j=0; j<pixelsWide; ++j)
		{
			colour pixelColour(0,0,0,0);
			for(int k=0; k<antialias; ++k)
			{
				for(int m=0; m<antialias; ++m)
				{
					double x = i-double(pixelsHigh)/2-0.5+double(antialias)/2+k/double(antialias);
					x*=double(height)/pixelsHigh;
					double y = j-double(pixelsWide)/2-0.5+double(antialias)/2+m/double(antialias);
					y*=double(width)/pixelsWide;
					//cout << x << "," << y << ";";

					double ca = x; //real part
					double cb = y; //imaginary part
					double za = ca;
					double zb = cb;

					int itterations = 0;
					int max = 1000;

					while(sqrt(za*za + zb*zb) <= 4 && itterations <= max)
					{
						++itterations;
						//z(a+1) = z(a)^2+c
						za = za*za - zb*zb + ca;
						zb = 2*za*zb + cb;
					}
					//cout << x << "," << y << ":" << za << "," << zb << ";";
					colour currentColour;
					if(sqrt(za*za + zb*zb) <= 4)
					{
						currentColour = colour();
					}
					else
					{
						//currentColour = colour(255,255,255);
						itterations*=100;
						currentColour = colour(itterations,itterations-255,itterations-511);
						//currentColour = colour((double(itterations)/double(max))*255,(double(itterations)/double(max))*255,(double(itterations)/double(max))*255);
					}

					pixelColour = pixelColour.add(currentColour.product(1.0/(antialias*antialias)));
				}
			}
			pixelMap.at(i).at(j) = pixelColour;
		}
	}
	//drawToTerminal(pixelMap);
	drawToUnicode(pixelMap);
	drawToPPM(pixelMap);
}

void viewport::drawToTerminal(vector<vector<colour> > pixelMap)
{
	for(int i=0; i<pixelMap.size(); ++i)
	{
		for(int j=0; j<pixelMap.at(i).size(); ++j)
		{
			cout << "\033[48;05;" << pixelMap.at(i).at(j).toAnsi() << "m  \033[0m";
		}
		cout << "\n";
	}
	cout << endl;
}

void viewport::drawToUnicode(vector<vector<colour> > pixelMap)
{
	for(int i=0; i<pixelMap.size()-1; i+=2)
	{
		for(int j=0; j<pixelMap.at(i).size(); ++j)
		{
			cout << "\033[48;05;" << pixelMap.at(i).at(j).toAnsi() << "m" << "\033[38;05;" << pixelMap.at(i+1).at(j).toAnsi() << "m▄";
		}
		cout << "\033[0m\n";
	}
	cout << endl;
}

void viewport::drawToPPM(vector<vector<colour> > pixelMap)
{
	ofstream outFile("test.ppm");
	outFile << "P3" << endl;
	outFile << pixelMap.size() << " " << pixelMap.at(0).size() << endl;
	outFile << 255;
	for(int i=0; i<pixelMap.size(); ++i)
	{
		for(int j=0; j<pixelMap.at(i).size(); ++j)
		{
			if(j%5 == 0)
			{
				outFile << endl;
			}
			outFile << " " << pixelMap.at(i).at(j).R;
			outFile << " " << pixelMap.at(i).at(j).G;
			outFile << " " << pixelMap.at(i).at(j).B;
		}
		outFile << endl;
	}
	outFile.close();
}
