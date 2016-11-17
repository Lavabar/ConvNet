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

static double *connect_fm(struct feature_map *fm, int n)
{
	double *res;
	int i, j, g;
	g = 0;
	if ((res = (double *)malloc(sizeof(double) * n * fm[0].w * fm[0].h)) == NULL) {
		fprintf(stderr, "error in malloc res\n");
		goto exit_failure;	
	}

	for (i = 0; i < n; i++)
		for (j = 0; j < fm[i].w * fm[i].h; j++)
			res[g++] = fm[i].data[j];

	return res;

exit_failure:
	return NULL;
}

double *convfpass(struct IplImage *frame, struct convnet *cnet, struct neuronet *net)
{
	int i, x, y;
	double *data, *out;
	
	for (i = 0; i < cnet->n_kernels; i++) {
		for (y = 0; y < cnet->k_width; y++) {
			for (x = 0; x < cnet->k_width; x++)
				printf("%3.1lf ", cnet->knls[i].data[y * cnet->k_width + x]);
			printf("\n");
		}
		printf("\n");
	}
	if((cnet->fmaps = init_fmaps(cnet->n_kernels, frame->width - cnet->k_width - 1, frame->height - cnet->k_width - 1)) == NULL) {
		fprintf(stderr, "error in init fmaps\n");
		goto exit_failure;
	}
	if((cnet->pmaps = init_fmaps(cnet->n_kernels, (frame->width - cnet->k_width - 1) / 2, (frame->height - cnet->k_width - 1) / 2)) == NULL) {
		fprintf(stderr, "error in init pooled_maps");
		goto exit_failure;
	}

	//ipl_scaleimg(&frame, 25, 10);
	
	for (i = 0; i < cnet->n_kernels; i++) {
		if(conv(frame, &(cnet->fmaps[i]), &(cnet->knls[i]))) {
			fprintf(stderr, "error in convolution\n");
			goto exit_failure;
		}
		for (y = 0; y < cnet->fmaps[i].h; y++) {
			for (x = 0; x < cnet->fmaps[i].w; x++)
				printf("%3.1lf ", cnet->fmaps[i].data[y * cnet->fmaps[i].w + x]);
			printf("\n");
		}
		printf("\n");
		if(pool(&(cnet->fmaps[i]), &(cnet->pmaps[i]))) {
			fprintf(stderr, "error in pooling\n");
			goto exit_failure;
		}
		relu(&(cnet->pmaps[i]));
	}
	if ((data = connect_fm(cnet->pmaps, cnet->n_kernels)) == NULL) {
		fprintf(stderr, "error in connect feature maps\n");
		goto exit_failure;
	}
	out = netfpass(net, data);
	
	for (y = 0; y < cnet->pmaps[0].h * cnet->n_kernels; y++) {
		for (x = 0; x < cnet->pmaps[0].w; x++)
			printf("%3.1lf  ", data[y * cnet->pmaps[0].w + x]);
		printf("\n");
	}
		
	//printf("out = %lf\n", *(out));	

	return out;

exit_failure:
	return NULL;

}

int convbpass(struct conv net *cnet, double *out, double *target, double eta)
{
	
}
