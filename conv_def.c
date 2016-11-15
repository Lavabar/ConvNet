#include "conv_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct feature_map *init_fmaps(int n_fmaps, int w, int h)
{
	struct feature_map *fm;
	int i;
	
	if((fm = (struct feature_map *)malloc(sizeof(struct feature_map) * n_fmaps)) == NULL) {
		fprintf(stderr, "error in malloc fm\n");
		goto exit_failure;
	}
	
	for (i = 0; i < n_fmaps; i++) {
		if((fm[i].data = (double *)malloc(sizeof(double) * w * h)) == NULL) {
			fprintf(stderr, "error in malloc data\n");
			goto exit_failure;
		}
		fm[i].w = w;
		fm[i].h = h;
	}

	return fm;

exit_failure:
	return NULL;
}

void free_fm(struct feature_map *fm)
{
	free(fm->data);
	free(fm);	
}

struct kernel *init_kernels(int n_conv_layers, int n_kernels, int kernel_width)
{
	struct kernel *res;
	int i, j;
	if((res = (struct kernel *)malloc(sizeof(struct kernel) * n_conv_layers * n_kernels)) == NULL) {
		fprintf(stderr, "error in malloc res\n");
		goto exit_failure;	
	}
		
	for (i = 0; i < n_kernels; i++)
		if((res[i].data = (double *)malloc(sizeof(double) * kernel_width * kernel_width)) == NULL) {
			fprintf(stderr, "error in malloc data\n");
			goto exit_failure;
		}


	srand(time(NULL));
	for (i = 0; i < n_conv_layers; i++) {
		res[i].w = kernel_width;
		for (j = 0; j < res[i].w * res[i].w; j++) {
			res[i].data[j] = (double)rand() / (double)RAND_MAX * 2.0 - 1.0;
			//printf("%3.1lf ", res[i].data[j]);
		}
	}
	//printf("\n");
	return res;

exit_failure:
	return NULL;
}
