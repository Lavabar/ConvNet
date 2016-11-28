#ifndef NETPASS_H
#define NETPASS_H

#include "net_structs.h"

double *netfpass(struct neuronet *net, double *data);
int netbpass(struct neuronet *net, double *inp, double *out, double *target, struct convnet *cnet, struct data *src, double eta);
double *convfpass(struct data *frame, struct convnet *cnet, struct neuronet *net);

#endif
