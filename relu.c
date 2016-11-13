#include <stdio.h>
#include <stdlib.h>
#include "conv_def.h"

void relu(struct feature_map *fm)
{
	int i;
	int w, h;
	w = fm->w;
	h = fm->h;
	
	for (i = 0; i < h * w; i++)
		fm->data[i] = (fm->data[i] > 0)? fm->data[i] : 0;	
}
