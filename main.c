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

int main()
{
	int i;
	struct IplImage *frame;
	double *data, *out;
	struct neuronet *net;
	struct kernel *knls;

	int nn[] = {100, 1};	
	if((knls = init_kernels(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error in init kernels\n");
		goto exit_failure;
	}

	frame = ipl_readimg("/home/user/5.png", IPL_RGB_MODE);
	
	net = netcreat(2, nn, ((frame->width - (KERNEL_WIDTH / 2 + 1)) / 2) * ((frame->height - (KERNEL_WIDTH / 2 + 1)) / 2) * N_KERNELS);
	nettofile();
		
	out = convfpass(frame, knls, net);
			
	printf("out = %lf\n", *(out));	

	return 0;

exit_failure:
	return -1;

}
