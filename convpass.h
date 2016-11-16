#ifndef _H_CONVPASS
#define _H_CONVPASS

#include "iplimage.h"
#include "conv_def.h"
#include "net_structs.h"

double *convfpass(struct IplImage *frame, struct convnet *cnet, struct neuronet *net);

#endif
