#include "ppm.h"

PIX** alloc_pixel(int x, int y)
{
	PIX** tmp = NULL;
	
	tmp = (PIX**)(malloc(sizeof(PIX*) * y));
	if(tmp == NULL) return tmp;
	for(int j=0; j<y; j++)
		tmp[j] = (PIX*)(malloc(sizeof(PIX) * x));
	for(int j=0; j<y; j++)
	{
		for(int i=0; i<x; i++)
		{
			tmp[j][i].r = 0;
			tmp[j][i].g = 0;
			tmp[j][i].b = 0;
		}
	}
	return tmp;
}

PPM ppmCreate(char *filename, int x, int y)
{
	PPM img = (PPM)malloc(sizeof(struct ppm));
	if(img != NULL)
	{
		img->filename = (char*)malloc(strlen(filename)+1);
		strcpy(img->filename, filename);
		img->w = x; img->h = y;
		img->p = alloc_pixel(x, y);
	}
	return img;
}

void ppmDrawPixel(PPM img, int x, int y, int32_t col)
{
	if(x < img->w && x >= 0 && y < img->h && y >= 0)
	{
		img->p[y][x].r = (col & 0xFF);
		img->p[y][x].g = (col>>8 & 0xFF);
		img->p[y][x].b = (col>>16 & 0xFF);
	}
}

void ppmDrawLine(PPM img, point p1, point p2, int32_t col)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;
	float a,b,ii,jj;
	
	if (p1.x < p2.x) {xmin=p1.x; xmax=p2.x;} else{xmin=p2.x; xmax=p1.x;}
	if (p1.y < p2.y) {ymin=p1.y; ymax=p2.y;} else{ymin=p2.y; ymax=p1.y;}
	
	if (xmin==xmax) for (j=ymin;j<=ymax;j++) ppmDrawPixel(img,xmin,j,col);
	if (ymin==ymax) for (i=xmin;i<=xmax;i++) ppmDrawPixel(img,i,ymin,col);
	
	if ((xmax-xmin >= ymax-ymin) && (ymax-ymin>0)) {
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (i=xmin;i<=xmax;i++) {
			jj = a*i+b;
			j = jj;
			if (((jj-j) > 0.5) && (j < img->h-1)) j++;
			ppmDrawPixel(img, i, j, col);
		}
	}
	
	if ((ymax-ymin > xmax-xmin) && (xmax-xmin>0)) {
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (j=ymin;j<=ymax;j++) {
			ii = (j-b)/a;
			i = ii;
			if (((ii-i) > 0.5) && (i < img->w-1)) i++;
			ppmDrawPixel(img, i, j, col);
		}
	}
}

PPM ppmOpen(char *filename)
{
	FILE *fimg;
	PPM img;
	
	fimg = fopen(filename, "r");
	if(fimg == NULL){exit(1);}
	img = (PPM)malloc(sizeof(struct ppm));
	if(img == NULL){exit(1);}
	/*Copie du nom*/
	img->filename = malloc(strlen(filename));
	strcpy(img->filename, filename);
	
	/*Traitement d'image*/
	fscanf(fimg, "P3"); //saute P3
	fscanf(fimg, "%d %d", &(img->w), &(img->h)); //chargement dims
	img->p = alloc_pixel(img->w,img->h);
	if(img == NULL){exit(1);}
	
	fscanf(fimg, "%d", &img->p[0][0].r); //saute 255
	
	for(int j=0; j < img->h; j++)
	{
		for(int i = 0; i < img->w; i++)
		{
			fscanf(fimg, "%d %d %d", &img->p[j][i].r, &img->p[j][i].g, &img->p[j][i].b);
			//printf("%d %d %d - J %d\n", img.p[j][i].r, img.p[j][i].g, img.p[j][i].b, j);
		}
	}
	fclose(fimg);
	return  img;
}

PPM ppmReduce(PPM src, int c) // c > 1
{	
	PPM dst = (PPM)malloc(sizeof(struct ppm));
	if(dst == NULL || c <= 1) return NULL;
	
	/*Copie du titre*/
	char size[10];
	sprintf(size, "%d",c);
	
	dst->filename = (char*)malloc(strlen(src->filename) + 22); //...+ la taille max et le "_reduce"
	int i=0;
	while(src->filename[i] != '.')
	{
		dst->filename[i] = src->filename[i];
		i++;
	}
	dst->filename[i] = '\0';
	strcat(dst->filename, "_reduce*");
	strcat(dst->filename, size);
	strcat(dst->filename, ".ppm");
	
	/*Fin*/
	dst->w = src->w/c; dst->h = src->h/c;
	
	dst->p = alloc_pixel(dst->w, dst->h);
	
	for(int i=0; i<dst->w; i++)
	{
		for(int j=0; j<dst->h; j++)
		{
			dst->p[j][i].r = src->p[j*c+1][c*i+1].r;
			dst->p[j][i].g = src->p[j*c+1][c*i+1].g;
			dst->p[j][i].b = src->p[j*c+1][c*i+1].b;
		}
	}
	return dst;
}

void ppmClose(PPM img)
{
	for(int j=0; j<img->h; j++)
		free(img->p[j]);
	free(img->p);
	free(img->filename);
	free(img);
}

int ppmExport(PPM img)
{
	FILE *f;
	f = fopen(img->filename,"w");
	if(f==NULL) return -1;
	//Basic Info
	fprintf(f, "P3\n%d %d\n255\n", img->w, img->h);
	
	//Pixels
	for(int j=img->h-1; j >= 0; j--)
	{
		for(int i=0; i<img->w; i++)
		{
			fprintf(f, "%d %d %d ", img->p[j][i].r, img->p[j][i].g, img->p[j][i].b);
		}
		fprintf(f,"\n");
	}
	
	fclose(f);
	return 0;
}
