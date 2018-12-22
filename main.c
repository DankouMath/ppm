#include "ppm.h"

int main(int argc, char **argv)
{
	ppm p = ppmCreate(argv[1], 10, 10);
	point p1 = {0,0};
	point p2 = {9,1};
	ppmDrawLine(p, p1,p2, 0xFF78FF);
	ppmExport(p);
	ppmClose(p);
	return 0;
}

