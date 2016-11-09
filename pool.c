#include <stdio.h>
#include <stdlib.h>

int *pool(int *img)
{
	int x, y, w, h;
	int i, j;

	w = img->width;
	h = img->height;
	
	int *feature_map;
	feature_map = (int *)malloc(sizeof(int) * (w/2) * (h/2));
	
	for(y = 0, i = 0; y < h - 1; y += 2, i++)
		for (x = 0, j = 0; x < w - 1; x += 2, j++)
			feature_map[i * w/2 + j] = max(max(max(img->data[y * w + x], img[y * w + x + 1]), 
				img->data[(y + 1) * w + x]),
					img->data[(y + 1) * w + x + 1]);
	free(img);
	return feature_map;		
}
