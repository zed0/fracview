#include "colour.h"
#include "viewport.h"
#include "stringUtils.h"
#include <termios.h>

using namespace std;

int main(int argc, char* argv[])
{
	int pixelsHigh = 80;
	int pixelsWide = 80;
	double minX = -2.5;
	double maxX = 1.5;
	double minY = -2;
	double maxY = 2;
	int antialiasing = 2;
	double magnification = 1;
	double colourScale = 128;
	vector<string> args(argv, argv + argc);
	for(int i=0; i<args.size(); ++i)
	{
		if(args.at(i) == "--help")
		{
			cout << "Usage:" << args.at(0) << " [--min-x <min x>] [--max-x <max x>] [--min-y <min y>] [--max-y <max y>] [--antialiasing <antialiasing>]" << endl;
			cout << "Keys:" << endl;
			cout << "h,j,k,l: scroll left, down, up, right." << endl;
			cout << "u,d: zoom in, out." << endl;
			cout << "n,m: colour scale up, down." << endl;
			cout << "p: print to file, supports any output format supported by imagemagick." << endl;
			cout << "q: quit." << endl;
			return 0;
		}
		if(args.at(i) == "--min-x" && args.size() > i+1)
		{
			minX = stringUtils::fromString<double>(args.at(i+1));
		}
		if(args.at(i) == "--max-x" && args.size() > i+1)
		{
			maxX = stringUtils::fromString<double>(args.at(i+1));
		}
		if(args.at(i) == "--min-y" && args.size() > i+1)
		{
			minY = stringUtils::fromString<double>(args.at(i+1));
		}
		if(args.at(i) == "--max-y" && args.size() > i+1)
		{
			maxY = stringUtils::fromString<double>(args.at(i+1));
		}
		if(args.at(i) == "--height" && args.size() > i+1)
		{
			pixelsHigh = stringUtils::fromString<int>(args.at(i+1));
		}
		if(args.at(i) == "--width" && args.size() > i+1)
		{
			pixelsWide = stringUtils::fromString<int>(args.at(i+1));
		}
		if(args.at(i) == "--antialiasing" && args.size() > i+1)
		{
			antialiasing = stringUtils::fromString<int>(args.at(i+1));
		}
	}
	termios before, after;
	tcgetattr (STDIN_FILENO, &before); // fill 'before' with current termios values
	after = before; // make a copy to be modified
	after.c_lflag &= (~ICANON); // Disable canonical mode, including line buffering
	after.c_lflag &= (~ECHO); // Don't echo characters on the screen (optional)
	tcsetattr (STDIN_FILENO, TCSANOW, &after); // Set the modified flags

	char ch;
	//cout << "\033[s";
	while(true)
	{
		viewport camera(minX, maxX, minY, maxY, pixelsHigh, pixelsWide, antialiasing, colourScale);
		camera.render();
		//drawToTerminal(pixelMap);
		cout << "\033[u";
		cout << "\033[0;0H";
		camera.drawToUnicode();
		cout << "\033[2KMagnification: " << magnification << "; x: " << (minX + maxX)/2 << "; y: " << (minY + maxY)/2 << ";" << endl;
		if(ch=cin.get())
		{
			if(ch == 'q')
			{
				tcsetattr (STDIN_FILENO, TCSANOW, &before);
				return 0;
			}
			else if(ch == 'h')
			{
				double tempMinY = minY-(maxY-minY)/2.0;
				maxY = maxY-(maxY-minY)/2.0;
				minY = tempMinY;
			}
			else if(ch == 'j')
			{
				double tempMinX = minX+(maxX-minX)/2.0;
				maxX = maxX+(maxX-minX)/2.0;
				minX = tempMinX;
			}
			else if(ch == 'k')
			{
				double tempMinX = minX-(maxX-minX)/2.0;
				maxX = maxX-(maxX-minX)/2.0;
				minX = tempMinX;
			}
			else if(ch == 'l')
			{
				double tempMinY = minY+(maxY-minY)/2.0;
				maxY = maxY+(maxY-minY)/2.0;
				minY = tempMinY;
			}
			else if(ch == 'u')
			{
				double tempMinX = minX + (maxX-minX)/4;
				maxX = maxX - (maxX-minX)/4;
				minX = tempMinX;
				double tempMinY = minY + (maxY-minY)/4;
				maxY = maxY - (maxY-minY)/4;
				minY = tempMinY;
				magnification *= 2;
			}
			else if(ch == 'd')
			{
				double tempMinX = 3.0*minX/2.0 - maxX/2.0;
				maxX = 3.0*maxX/2.0 - minX/2.0;
				minX = tempMinX;
				double tempMinY = 3.0*minY/2.0 - maxY/2.0;
				maxY = 3.0*maxY/2.0 - minY/2.0;
				minY = tempMinY;
				magnification /= 2;
			}
			else if(ch == 'n')
			{
				colourScale *= 2;
			}
			else if(ch == 'm')
			{
				colourScale /= 2;
			}
			else if(ch == 'p')
			{
				string filename = "fracview.png";
				int resX = 200;
				int resY = 200;
				tcsetattr (STDIN_FILENO, TCSANOW, &before);
				cout << "Enter filename (fracview.png):";
				cin >> filename;
				cout << "Enter x resolution (800):";
				cin >> resX;
				cout << "Enter y resolution (800):";
				cin >> resY;
				tcsetattr (STDIN_FILENO, TCSANOW, &after);
				viewport camera(minX, maxX, minY, maxY, resX, resY, antialiasing, colourScale);
				camera.render();
				camera.drawToFile(filename);
				cin.ignore(); //clear the input buffer
			}
		}
	}
	return 0;
}
