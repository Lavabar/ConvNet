#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "iplimage.h"
#include "iplvideo.h"
#include "ipldefs.h"
#include "conv_def.h"
#include "conv.h"
#include "pool.h" 
#include "relu.h"
#include "net_structs.h"
#include "netcreat.h"
#include "netpass.h"

#define N_CONV_LAYERS 1
#define N_KERNELS 3
#define KERNEL_WIDTH 3

double *connect_fm(struct feature_map *fm)
{
	double *res;
	int i, j, g;
	g = 0;
	if ((res = (double *)malloc(sizeof(double) * N_KERNELS * fm[0].w * fm[0].h)) == NULL) {
		fprintf(stderr, "error in malloc res\n");
		goto exit_failure;	
	}

	for (i = 0; i < N_KERNELS; i++)
		for (j = 0; j < fm[i].w * fm[i].h; j++)
			res[g++] = fm[i].data[j];

	return res;

exit_failure:
	return NULL;
}

int main()
{
	int i;
	struct IplImage *frame;
	struct feature_map *fmaps, *pmaps;
	struct kernel *knls;
	double *data, *out;
	struct neuronet *net;
	int nn[] = {100, 1};
	
	if((knls = init_kernels(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error in init kernels\n");
		goto exit_failure;
	}
	frame = ipl_readimg("/home/user/5.png", IPL_RGB_MODE);	
	if((fmaps = init_fmaps(N_KERNELS, frame->width - KERNEL_WIDTH - 1, frame->height - KERNEL_WIDTH - 1)) == NULL) {
		fprintf(stderr, "error in init fmaps\n");
		goto exit_failure;
	}
	if((pmaps = init_fmaps(N_KERNELS, (frame->width - KERNEL_WIDTH - 1) / 2, (frame->height - KERNEL_WIDTH - 1) / 2)) == NULL) {
		fprintf(stderr, "error in init pooled_maps");
		goto exit_failure;
	}

	//ipl_scaleimg(&frame, 25, 10);
	
	for (i = 0; i < N_KERNELS; i++) {
		if(conv(frame, &(fmaps[i]), &(knls[i]))) {
			fprintf(stderr, "error in convolution\n");
			goto exit_failure;
		}
		if(pool(&(fmaps[i]), &(pmaps[i]))) {
			fprintf(stderr, "error in pooling\n");
			goto exit_failure;
		}
		relu(&(pmaps[i]));
	}
	if ((data = connect_fm(pmaps)) == NULL) {
		fprintf(stderr, "error in connect feature maps\n");
		goto exit_failure;
	}
	net = netcreat(2, nn, N_KERNELS * pmaps[0].w * pmaps[0].h);
	out = netfpass(net, data);
	
	printf("out = %lf", *(out));	

	return 0;

exit_failure:
	return -1;

}
