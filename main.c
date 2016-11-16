#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "iplimage.h"
#include "iplvideo.h"
#include "ipldefs.h"
#include "conv_def.h"
#include "convpass.h"
#include "net_structs.h"
#include "netcreat.h"
#include "netfile.h"

#define N_CONV_LAYERS 1
#define N_KERNELS 1    
#define KERNEL_WIDTH 7

#define NEURO_PATH "/home/user/ConvNet/neuro.data"

int main()
{
	struct IplImage *frame;
	double *out;
	struct neuronet *net;
	struct convnet *cnet;

	net = (struct neuronet *)malloc(sizeof(struct neuronet));
	cnet = (struct convnet *)malloc(sizeof(struct convnet));
	if ((cnet->knls = init_kernels(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error initing kernels\n");
		goto exit_failure;
	}

	frame = ipl_readimg("/home/user/3.png", IPL_RGB_MODE);
	if (netfromfile(net, cnet, NEURO_PATH) == -1) {
		fprintf(stderr, "error reading nets\n");
		goto exit_failure;
	}
	
	/*int nn[] = {100, 1};	
	if((cnet = cnetcreat(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error in init convnet\n");
		goto exit_failure;
	}
	net = netcreat(2, nn, ((frame->width - ((KERNEL_WIDTH / 2 + 1) * 2)) / 2) * ((frame->height - ((KERNEL_WIDTH / 2 + 1) * 2)) / 2) * N_KERNELS);
	nettofile(net, cnet, NEURO_PATH);*/

	out = convfpass(frame, cnet, net);		
	printf("out = %lf\n", *(out));	

	return 0;

exit_failure:
	return -1;

}
