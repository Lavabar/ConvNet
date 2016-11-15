#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iplimage.h"
#include "conv_def.h"
#include "conv.h"
#include "pool.h" 
#include "relu.h"
#include "net_structs.h"
#include "netpass.h"

static double *connect_fm(struct feature_map *fm)
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

double *convfpass(struct IplImage *frame, struct kernel *knls, struct neuronet *net)
{
	int i, x, y;
	struct feature_map *fmaps, *pmaps;
	double *data, *out;
	
	/*for (i = 0; i < N_KERNELS; i++) {
		for (y = 0; y < KERNEL_WIDTH; y++) {
			for (x = 0; x < KERNEL_WIDTH; x++)
				printf("%3.1lf ", knls[i].data[y * KERNEL_WIDTH + x]);
			printf("\n");
		}
		printf("\n");
	}*/
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
	
	/*for (y = 0; y < pmaps[0].h * N_KERNELS; y++) {
		for (x = 0; x < pmaps[0].w; x++)
			printf("%3.1lf  ", data[y * pmaps[0].w + x]);
		printf("\n");
	}*/
		
	printf("out = %lf\n", *(out));	

	return out;

exit_failure:
	return NULL;

}