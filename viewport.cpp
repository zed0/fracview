#include "viewport.h"

using namespace std;

viewport::viewport(double minX, double maxX, double minY, double maxY, int pixelsHigh, int pixelsWide, int antialias)
{
	this->pixelsWide = pixelsWide;
	this->pixelsHigh = pixelsHigh;
	this->antialias = antialias;
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->pixelMap = vector<vector<colour> >(pixelsHigh, vector<colour>(pixelsWide));
}

void viewport::render()
{
	for(int i=0; i<pixelsHigh; ++i)
	{
		for(int j=0; j<pixelsWide; ++j)
		{
			colour pixelColour(0,0,0,0);
			for(int k=0; k<antialias; ++k)
			{
				for(int m=0; m<antialias; ++m)
				{
					double height = maxX - minX;
					double x = minX + (i - 0.5+double(antialias)/2 + k/double(antialias))/pixelsHigh * height;
					double width = maxY - minY;
					double y = minY + (j - 0.5+double(antialias)/2 + m/double(antialias))/pixelsWide * width;
					//cout << x << "," << y << ";";

					double ca = x; //real part
					double cb = y; //imaginary part
					double za = x;
					double zb = y;
					double ztemp;

					int itterations = 0;
					int max = 1000;

					while(sqrt(za*za + zb*zb) <= 4 && itterations <= max)
					{
						++itterations;
						//z(a+1) = z(a)^2+c
						ztemp = za*za - zb*zb + ca;
						zb = 2*za*zb + cb;
						za = ztemp;
					}
					//cout << x << "," << y << ":" << za << "," << zb << ";";
					colour currentColour;
					if(sqrt(za*za + zb*zb) <= 1)
					{
						currentColour = colour();
					}
					else
					{
						//currentColour = colour(255,255,255);
						itterations*=100;
						itterations = itterations%1792;
						int red = 0;
						int green = 0;
						int blue = 0;
						if(itterations < 256)
						{
							red = itterations;
						}
						else if(itterations < 512)
						{
							red = 255;
							green = itterations - 256;
						}
						else if(itterations < 768)
						{
							green = 255;
							red = 768 - itterations;
						}
						else if(itterations < 1024)
						{
							green = 255;
							blue = itterations - 768;
						}
						else if(itterations < 1280)
						{
							green = 1280 - itterations;
							blue = 255;
						}
						else if(itterations < 1536)
						{
							red = itterations - 1280;
							blue = 255;
						}
						else
						{
							red = 255;
							green = itterations - 1536;
							blue = 255;
						}
						currentColour = colour(red, green, blue);
						//currentColour = colour((double(itterations)/double(max))*255,(double(itterations)/double(max))*255,(double(itterations)/double(max))*255);
					}

					pixelColour = pixelColour.add(currentColour.product(1.0/(antialias*antialias)));
				}
			}
			pixelMap.at(i).at(j) = pixelColour;
		}
	}
}

void viewport::drawToTerminal()
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

void viewport::drawToUnicode()
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

void viewport::drawToPPM()
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
