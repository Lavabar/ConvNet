#include <stdio.h>
#include <stdlib.h>
#include "conv_def.h"

double max(double a, double b)
{
	return (a > b)? a : b;
}

int pool(struct feature_map *fm, struct feature_map *fm1)
{
	int x, y, w, h;
	int i, j;

	w = fm->w;
	h = fm->h;
	
	for(y = 0, i = 0; y < h - 1; y += 2, i++)
		for (x = 0, j = 0; x < w - 1; x += 2, j++) {
			if ((i * fm1->w + j >= fm1->w * fm1->h) || (y * w + x >= fm->w * fm->h) || (y * w + x + 1 >= fm->w * fm->h) || (y + 1 * w + x + 1 >= fm->w * fm->h) || ((y + 1) * w + x >= fm->w * fm->h)) {
				fprintf(stderr, "segfault\n");
				goto exit_failure;
			}
			fm1->data[i * fm1->w + j] = max(max(max(fm->data[y * w + x], fm->data[y * w + x + 1]), fm->data[(y + 1) * w + x]), fm->data[(y + 1) * w + x + 1]);
		}

	return 0;

exit_failure:
	return -1;
}
