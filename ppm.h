#ifndef _PPM
#define _PPM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*rgb 0..255*/
typedef struct point
{
	int x,y;
}point;

typedef struct pixel
{
	int r,g,b;
}PIX;

struct ppm
{
	char *filename;
	int w,h;
	PIX **p;
};

typedef struct ppm* PPM;
typedef PPM ppm;

PPM ppmCreate(char *filename, int x, int y);
void ppmDrawPixel(PPM img, int x, int y, int32_t col);
void ppmDrawLine(PPM img, point p1, point p2, int32_t col);
PPM ppmOpen(char *filename);
PPM ppmReduce(PPM src, int c); // c > 1
void ppmClose(PPM img);
int ppmExport(PPM img);

#endif
