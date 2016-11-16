#ifndef NETFILE_H
#define NETFILE_H

void nettofile(struct neuronet *net, struct convnet *cnet, char *path);
int netfromfile(struct neuronet *net, struct convnet *cnet, char *path);

#endif // NET_STRUCTS_H
