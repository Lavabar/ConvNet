#include <stdio.h>
#include <stdlib.h>
#include "iplimage.h"
#include "conv_def.h"

int conv(struct data *img, struct feature_map *fm, struct kernel *kernel)
{
	int y, x, i, j, g;
	int w, h;
	w = img->w;
	h = img->h;
	g = 0;
	
	//convolution with kernel_width=n
	for (j = 0; j < h - ((kernel->w / 2) * 2); j++) {
		for (i = 0; i < w - ((kernel->w / 2) * 2); i++) {
			for (y = 0; y < kernel->w; y++) {
				for (x = 0; x < kernel->w; x++) {
					if ((w * j + i) + y * w + x >= w * h) {
						fprintf(stderr, "segfault\n");
						goto exit_failure;
					}
					fm->data[g] += img->data[(j * w + i) + y * w + x] * kernel->data[y * kernel->w + x];
				}
			}
			g++;
		}
	}
	return 0;

exit_failure:
	return -1;
}
