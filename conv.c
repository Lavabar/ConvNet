#include <stdio.h>
#include <stdlib.h>
#include "iplimage.h"
#include "conv_def.h"

struct feature_map *convfpass(struct IplImage *img, double *kernel)
{
	int y, x, i, j;
	int w, h;
	struct feature_map *fm;
	w = img->width;
	h = img->height;
	fm = (struct feature_map *)malloc(sizeof(struct feature_map));
	fm->data = (double *)malloc(sizeof(double) * (w - 2) * (h - 2));
	fm->w = w - 2;
	fm->h = h - 2;	
	
	for (y = 1, i = 0; y < h - 1; y++, i++)
		for (x = 1, j = 0; x < w - 1; x++, j++)
			fm->data[i * fm->w + j] = img->data[(y - 1) * w + (x - 1)] * kernel[0 * 3 + 0] + img->data[(y - 1) * w + (x)] * kernel[0 * 3 + 1] + img->data[(y - 1) * w + (x + 1)] * kernel[0 * 3 + 2] + 
				img->data[(y) * w + (x - 1)] * kernel[1 * 3 + 0] + img->data[(y) * w + (x)] * kernel[1 * 3 + 1] + img->data[(y) * w + (x + 1)] * kernel[1 * 3 + 2] +
				img->data[(y + 1) * w + (x - 1)] * kernel[2 * 3 + 0] + img->data[(y + 1) * w + (x)] * kernel[2 * 3 + 1] + img->data[(y + 1) * w + (x + 1)] * kernel[2 * 3 + 2];		
	return fm;	
}
