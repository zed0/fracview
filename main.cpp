#include "colour.h"
#include "viewport.h"
#include "stringUtils.h"

using namespace std;

int main(int argc, char* argv[])
{
	int pixelsHigh = 90;
	int pixelsWide = 90;
	double minX = -2.5;
	double maxX = 1.5;
	double minY = -2;
	double maxY = 2;
	int antialiasing = 2;
	vector<string> args(argv, argv + argc);
	for(int i=0; i<args.size(); ++i)
	{
		if(args.at(i) == "--help")
		{
			cout << "Usage:" << args.at(0) << " [--min-x <min x>] [--max-x <max x>] [--min-y <min y>] [--max-y <max y>] [--antialiasing <antialiasing>]" << endl;
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
	viewport camera(minX, maxX, minY, maxY, pixelsHigh, pixelsWide, antialiasing);
	camera.render();
	return 0;
}
