#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include "conv_def.h"
#include "netcreat.h"
#include "netpass.h"
#include "netfile.h"
#include "iplimage.h"
#include "ipldefs.h"

#define NEURO_PATH "/home/user/ConvNet/neuro.data"

#define ETA 1.0

#define N_CONV_LAYERS 1
#define N_KERNELS 1   
#define KERNEL_WIDTH 3

#define SAMPLE_WIDTH 4
#define SAMPLE_HEIGHT 4

struct sample {
	double *data;
	double target;
};

static double *getdata(struct IplImage *img)
{
	int x, y;
	double *data;
	
	data = (double *)malloc(sizeof(double) * img->width * img->height);
	for (y = 0; y < img->height; y++)
		for (x = 0; x < img->width; x++) {
			unsigned char r, g, b, max;
			r = img->data[img->nchans * (y * img->width + x) + 0];
			g = img->data[img->nchans * (y * img->width + x) + 1];
			b = img->data[img->nchans * (y * img->width + x) + 2];
			max = (r > g)? r : g;
			max = (b > max)? b : max;
			data[y * img->width + x] = (double)max / 255.0 * 2.0 - 1.0;
		}
	return data;
}

static double *connect_fm(struct feature_map *fm, int n)
{
	double *res;
	int i, j, g;
	g = 0;
	if ((res = (double *)malloc(sizeof(double) * n * fm->w * fm->h)) == NULL) {
		fprintf(stderr, "error in malloc res\n");
		goto exit_failure;	
	}

	for (i = 0; i < n; i++)
		for (j = 0; j < fm->w * fm->h; j++)
			res[g++] = fm[i].data[j];

	return res;

exit_failure:
	return NULL;
}

int main()
{
	struct IplImage *img;
	char name[256];
	double *out;
	struct neuronet *net;
	struct convnet *cnet;
	struct data *inp, *imgs;
	struct sample *examples;
	int i, j;

	net = (struct neuronet *)malloc(sizeof(struct neuronet));
	cnet = (struct convnet *)malloc(sizeof(struct convnet));

	/*int nl = 1;
	int nn[] = {2};	
	if((cnet = cnetcreat(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error in init convnet\n");
		goto exit_failure;
	}
	net = netcreat(nl, nn, ((SAMPLE_WIDTH - ((KERNEL_WIDTH / 2) * 2)) / 2) * ((SAMPLE_HEIGHT - ((KERNEL_WIDTH / 2) * 2)) / 2) * N_KERNELS);
	nettofile(net, cnet, NEURO_PATH);*/


	if ((cnet->knls = init_kernels(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error initing kernels\n");
		goto exit_failure;
	}
	if (netfromfile(net, cnet, NEURO_PATH) == -1) {
		fprintf(stderr, "error reading nets\n");
		goto exit_failure;
	}
	
	examples = (struct sample *)malloc(sizeof(struct sample));
	imgs = (struct data *)malloc(sizeof(struct data));
	examples->data = (double *)malloc(sizeof(double) * SAMPLE_WIDTH * SAMPLE_HEIGHT);

	examples->data[0] = 1.0;
	examples->data[1] = 1.0;
	examples->data[2] = 1.0;
	examples->data[3] = 1.0;
	examples->data[4] = 1.0;
	examples->data[5] = 0.0;
	examples->data[6] = 0.0;
	examples->data[7] = 1.0;
	examples->data[8] = 1.0;
	examples->data[9] = 0.0;
	examples->data[10] = 0.0;
	examples->data[11] = 1.0;
	examples->data[12] = 1.0;
	examples->data[13] = 1.0;
	examples->data[14] = 1.0;
	examples->data[15] = 1.0;

	examples->target = 1.0;	
	imgs->data = examples->data;
	imgs->w = 4;
	imgs->h = 4;
	
	out = convfpass(imgs, cnet, net);
	double val;
	val = *(out);
	printf("out = %lf\n", val);
	double *data;
	data = connect_fm(cnet->pmaps, cnet->n_kernels);
	if (netbpass(net, data, out, &(examples->target), cnet, imgs, ETA) == -1) {
		fprintf(stderr, "error in backpass\n");
		goto exit_failure;
	}
	printf("\n");
	for (i = 0; i < cnet->k_width; i++) {
		for (j = 0; j < cnet->k_width; j++)
			printf("%lf ", cnet->knls->data[i * cnet->k_width + j]);
		printf("\n");
	}
	//getchar();	
	free(out);
	free(data);
	
	return 0;

exit_failure:
	return -1;
}
