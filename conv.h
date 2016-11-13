#ifndef _H_CONV
#define _H_CONV

#include "conv_def.h"
#include "iplimage.h"

int conv(struct IplImage *img, struct feature_map *fm, struct kernel *kernel);

#endif
