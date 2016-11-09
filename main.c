#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "iplimage.h"
#include "iplvideo.h"
#include "ipldefs.h"
#include "conv_def.h"
#include "conv.h" 

int main()
{
	int x, y;
	struct IplImage *frame;
	struct feature_map *fm;
	unsigned char kernel[9];
	kernel[0] = 0;
	kernel[1] = 1;
	kernel[2] = 0;
	kernel[3] = 0;
	kernel[4] = 1;
	kernel[5] = 0;
	kernel[6] = 0;
	kernel[7] = 1;
	kernel[8] = 0;

	frame = ipl_readimg("/home/user/5.png", IPL_RGB_MODE);	
	//ipl_scaleimg(&frame, 25, 10);
	for (y = 0; y < frame->height; y++) {
		for (x = 0; x < frame->width; x++)
			printf("%5d", frame->data[y * frame->width + x]);
		printf("\n");	
	}

	if((fm = convfpass(frame, kernel)) == NULL)
		fprintf(stderr, "error in convolution\n");

	printf("\n");	printf("\n");	printf("\n");	printf("\n");
	for (y = 0; y < fm->h; y++) {
		for (x = 0; x < fm->w; x++)
			printf("%5.1lf ", fm->data[y * fm->w + x]);
		printf("\n");	
	}
	

	return 0;
}
