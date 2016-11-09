#include <stdio.h>
#include <stdlib.h>

int *relu(int *img)
{
	int x, y;
	int w, h;
	w = img->width;
	h = img->height;
	
	for (y = 0; y < h, y++)
		for (x = 0; x < w, x++)
			img->data[y * w + x] = max(img->data[y * w + x], 0);	
}
