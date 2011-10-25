#include "colour.h"
#include "viewport.h"
#include "stringUtils.h"
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

int main(int argc, char* argv[])
{
	bool utf = true;
	int pixelsHigh = 0;
	int pixelsWide = 0;

	double minX = -2.5;
	double maxX = 1.5;
	double minY = -2;
	double maxY = 2;
	double stretch_k = 1.0;
	double theta = 0.0;
	int iterates = 1000;
	int antialiasing = 2;
	double magnification = 1;
	double colourScale = 128;
	int colourOffset = 0;
	vector<string> args(argv, argv + argc);
	string saveLocation = "";
	for(int i=0; i<args.size(); ++i)
	{
		if(args.at(i) == "--help")
		{
			cout << "Usage:" << args.at(0) << " [--min-x <min x>] [--max-x <max x>] [--min-y <min y>] [--max-y <max y>] [--antialiasing <antialiasing>] [--width <pixels wide>] [--height <pixels high>] [--no-utf] [--k <k value>] [--theta <theta value>] [--iterates <max iterations>] [--dir <save location>]" << endl;
			cout << "Keys:" << endl;
			cout << "h,j,k,l: scroll left, down, up, right." << endl;
			cout << "u,d: zoom in, out." << endl;
			cout << "n,m: colour scale up, down." << endl;
			cout << "p: print to file, supports any output format supported by imagemagick." << endl;
			cout << "q: quit." << endl;
			cout << endl << "UTF characters are on by default as they increase the resolution in both directions by a factor of 2; if you are having problems displaying images then please use --no-utf." << endl;
			cout << "Detected terminal size: " << pixelsHigh << "x" << pixelsWide << endl;
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
		if(args.at(i) == "--no-utf")
		{
			utf = false;
		}
		if(args.at(i) == "--antialiasing" && args.size() > i+1)
		{
			antialiasing = stringUtils::fromString<int>(args.at(i+1));
		}
		if(args.at(i) == "--k" && args.size() > i+1)
		{
			stretch_k = stringUtils::fromString<double>(args.at(i+1));
		}
		if(args.at(i) == "--theta" && args.size() > i+1)
		{
			theta = stringUtils::fromString<double>(args.at(i+1));
		}
		if(args.at(i) == "--dir" && args.size() > i+1)
		{
			saveLocation = args.at(i+1);
		}
		if(args.at(i) == "--iterates" && args.size() > i+1)
		{
			iterates = stringUtils::fromString<int>(args.at(i+1));
		}
	}
	if(pixelsWide == 0 && pixelsHigh == 0)
	{
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		cout << "w:" << w.ws_col << "; h:" << w.ws_row << " ";
		if((w.ws_col/2)<w.ws_row)
		{
			pixelsHigh = w.ws_col;
			pixelsWide = w.ws_col;
			cout << "width" << endl;
			pixelsHigh-=1;
			pixelsWide-=1;
			if(!utf)
			{
				pixelsHigh/=2;
				pixelsWide/=2;
			}
		}
		else
		{
			pixelsHigh = w.ws_row;
			pixelsWide = w.ws_row;
			cout << "height" << endl;
			if(utf)
			{
				pixelsHigh*=2;
				pixelsWide*=2;
			}
			pixelsHigh-=2;
			pixelsWide-=2;
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
		viewport camera(minX, maxX, minY, maxY, stretch_k, theta, pixelsHigh, pixelsWide, antialiasing, colourScale, colourOffset, iterates);
		camera.render();
		cout << "\033[u";
		cout << "\033[0;0H";
		if(utf)
		{
			camera.drawToUnicode();
		}
		else
		{
			camera.drawToTerminal();
		}
		cout << "\033[2KMagnification: " << magnification << "; x: " << (minX + maxX)/2 << "; y: " << (minY + maxY)/2 << ";";
		if(ch=cin.get())
		{
			if(ch == 'q')
			{
				cout << endl;
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
			else if(ch == '.') {
				colourOffset += 100;
			}
			else if(ch == ',')
			{
				colourOffset -= 100;
			}
			else if(ch == 'n') {
				colourScale *= 2;
			}
			else if(ch == 'm')
			{
				colourScale /= 2;
			}
			else if(ch == 'p')
			{
				string defaultFilename = saveLocation + "fracviewX" + stringUtils::toString<double>((maxX+minX)/2) + "Y" + stringUtils::toString<double>((maxY+minY)/2) + "." + stringUtils::toString<int>(magnification) + ".png";
				string filename;
				string defaultResX = "800";
				string resX;
				string defaultResY = "800";
				string resY;
				tcsetattr (STDIN_FILENO, TCSANOW, &before);
				cout << "Enter filename (" << defaultFilename << "):";
				getline(cin, filename);
				if(filename.empty())
				{
					filename = defaultFilename;
				}
				cout << "Enter x resolution (800):";
				getline(cin, resX);
				if(resX.empty())
				{
					resX = defaultResX;
				}
				cout << "Enter y resolution (800):";
				getline(cin, resY);
				if(resY.empty())
				{
					resY = defaultResY;
				}
				tcsetattr (STDIN_FILENO, TCSANOW, &after);
				viewport camera(minX, maxX, minY, maxY, stretch_k, theta, stringUtils::fromString<int>(resX), stringUtils::fromString<int>(resY), antialiasing, colourScale, colourOffset);
				camera.render();
				camera.drawToFile(filename);
				cin.ignore(); //clear the input buffer
			}
		}
	}
	return 0;
}
