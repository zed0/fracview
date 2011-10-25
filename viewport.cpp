#include "viewport.h"

using namespace std;

viewport::viewport(double minX, double maxX, double minY, double maxY, double stretch_k, double theta, int pixelsHigh, int pixelsWide, int antialias, double colourScale, int colourOffset)
{
	this->pixelsWide = pixelsWide;
	this->pixelsHigh = pixelsHigh;
	this->antialias = antialias;
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->stretch_k = stretch_k;
	this->theta = theta;
	this->colourScale = colourScale;
	this->colourOffset = colourOffset;
	this->pixelMap = vector<vector<colour> >(pixelsWide, vector<colour>(pixelsHigh));
}

void viewport::render()
{
	double s_theta = sin(theta);
	double c_theta = cos(theta);
	for(int i=0; i<pixelsWide; ++i)
	{
		for(int j=0; j<pixelsHigh; ++j)
		{
			colour pixelColour(0,0,0,0);
			for(int k=0; k<antialias; ++k)
			{
				for(int m=0; m<antialias; ++m)
				{
					double width = maxX - minX;
					double x = minX + (i - 0.5+double(antialias)/2 + k/double(antialias))/pixelsWide * width;
					double height = maxY - minY;
					double y = minY + (j - 0.5+double(antialias)/2 + m/double(antialias))/pixelsHigh * height;
					//cout << x << "," << y << ";";

					double ca = x; //real part
					double cb = y; //imaginary part
					double za = x;
					double zb = y;
					double ztemp;
					double zx;
					double zy;

					int itterations = 0;
					int max = 1000;

					while(za*za + zb*zb <= 4 && itterations <= max)
					{
						++itterations;
						zx = za*(s_theta*s_theta + stretch_k*c_theta*c_theta) + zb*(stretch_k - 1)*s_theta*c_theta;
						zy = za*(stretch_k - 1)*s_theta*c_theta + zb*(stretch_k*s_theta*s_theta + c_theta*c_theta);
						za = zx*zx - zy*zy + ca;
						zb = 2*zx*zy + cb;

						/*
						ztemp = za*za - zb*zb + ca;
						zb = 2*za*zb + cb;
						za = ztemp;
						*/

					}
					colour currentColour;
					if(za*za + zb*zb <= 4)
					{
						currentColour = colour();
					}
					else
					{
						//currentColour = colour(255,255,255);
						itterations *= colourScale;
						itterations += colourOffset;
						itterations = itterations%1536;
						int red = 0;
						int green = 0;
						int blue = 0;
						if(itterations < 256)
						{
							red = 255;
							blue = itterations;
						}
						else if(itterations < 512)
						{
							blue = 255;
							red = 512 - itterations;
						}
						else if(itterations < 768)
						{
							blue = 255;
							green = itterations - 512;
						}
						else if(itterations < 1024)
						{
							green = 255;
							blue = 1024 - itterations;
						}
						else if(itterations < 1280)
						{
							green = 255;
							red = itterations - 1024;
						}
						else if(itterations < 1536)
						{
							red = 255;
							green = 1536 - itterations;
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
	colour foreColour;
	for(int i=0; i<pixelMap.size(); ++i)
	{
		for(int j=0; j<pixelMap.at(i).size(); ++j)
		{
			//don't output colour codes unless the colour has changed, significantly faster on old terminals.
			if(pixelMap.at(i).at(j) != foreColour)
			{
				foreColour = pixelMap.at(i).at(j);
				cout << "\033[48;05;" << foreColour.toAnsi() << "m";
			}
			cout << "  ";
		}
		foreColour=NULL;
		cout << "\033[0m\n";
	}
	cout << endl;
}

void viewport::drawToUnicode()
{
	colour foreColour;
	colour backColour;
	for(int i=0; i<pixelMap.size()-1; i+=2)
	{
		for(int j=0; j<pixelMap.at(0).size(); ++j)
		{
			//don't output colour codes unless the colour has changed, significantly faster on old terminals.
			if(pixelMap.at(i).at(j) != foreColour)
			{
				foreColour = pixelMap.at(i).at(j);
				cout << "\033[48;05;" << foreColour.toAnsi() << "m";
			}
			if(pixelMap.at(i+1).at(j) != backColour)
			{
				backColour = pixelMap.at(i+1).at(j);
				cout << "\033[38;05;" << backColour.toAnsi() << "m";
			}
			cout << "▄";
		}
		foreColour=NULL;
		backColour=NULL;
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

void viewport::drawToFile(string filename)
{
	using stringUtils::toString;
	FILE* output = popen(("convert ppm:- " + filename).c_str(), "w");
	fprintf(output, "P3\n");
	fprintf(output, "%i %i\n", pixelMap.size(), pixelMap.at(0).size());
	fprintf(output, "255");
	for(int i=0; i<pixelMap.size(); ++i)
	{
		for(int j=0; j<pixelMap.at(i).size(); ++j)
		{
			if(j%5 == 0)
			{
				fprintf(output, "\n");
			}
			fprintf(output, " %i", pixelMap.at(i).at(j).R);
			fprintf(output, " %i", pixelMap.at(i).at(j).G);
			fprintf(output, " %i", pixelMap.at(i).at(j).B);
		}
		fprintf(output, "\n");
	}
	pclose(output);
}
