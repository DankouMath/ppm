#include "ppm.h"

int main(int argc, char **argv)
{
	ppm p = ppmCreate(argv[1], 10, 10);
	ppmDrawPixel(p, 5,5, 0xFFFFFF);
	ppmExport(p);
	ppmClose(p);
	return 0;
}

