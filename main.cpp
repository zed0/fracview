#include "colour.h"
#include "viewport.h"

using namespace std;

int main(int argc, char* argv[])
{
	double camHeight = 4;
	double camWidth = 4;
	int pixelsHigh = 80;
	int pixelsWide = 80;
	viewport camera(camHeight, camWidth, pixelsHigh, pixelsWide);
	camera.render();
	return 0;
}
