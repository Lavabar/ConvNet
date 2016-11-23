#include <stdio.h>
#include <stdlib.h>
#include "iplimage.h"
#include "conv_def.h"

static double *getdata(struct IplImage *img, int sx, int sy, int dw, int dh)
{
	int x, y, x1, y1;
	double *data;
	
	if((data = (double *)malloc(sizeof(double) * dw * dh)) == NULL) {
		fprintf(stderr, "error in malloc data\n");
		goto exit_failure;
	}
	for (y = sy, y1 = 0; y < sy + dh; y++, y1++)
		for (x = sx, x1 = 0; x < sx + dw; x++, x1++) {
			unsigned char r, g, b, max;
			r = img->data[img->nchans * (y * img->width + x)];
			g = img->data[img->nchans * (y * img->width + x) + 1];
			b = img->data[img->nchans * (y * img->width + x) + 2];
			max = (r > g)? r : g;
			max = (b > max)? b : max;
			data[y1 * dw + x1] = (double)max / 255.0 * 2.0 - 1.0;
		}
	return data;

exit_failure:
	return NULL;
}

int conv(struct IplImage *img, struct feature_map *fm, struct kernel *kernel)
{
	int y, x, i, j, g;
	int w, h;
	w = img->width;
	h = img->height;
	g = 0;
	
	double *data;
	if((data = getdata(img, 0, 0, w, h)) == NULL) {
		fprintf(stderr, "error in getdata\n");
		goto exit_failure;	
	}

	//convolution with kernel_width=n
	for (j = 0; j < h - ((kernel->w / 2 + 1) * 2); j++) {
		for (i = 0; i < w - ((kernel->w / 2 + 1) * 2); i++) {
			for (y = 0; y < kernel->w; y++) {
				for (x = 0; x < kernel->w; x++) {
					if ((w * j + i) + y * w + x >= w * h) {
						fprintf(stderr, "segfault\n");
						goto exit_failure;
					}
					fm->data[g] += data[(j * w + i) + y * w + x] * kernel->data[y * kernel->w + x];
				}
			}
			g++;
		}
	}
	return 0;

exit_failure:
	return -1;
}
