#include <stdio.h>
#include "conv_def.h"
#include "netpass.c"
#include "convpass.c"

#define NEURO_PATH "/home/user/ConvNet/neuro.data"

#define SAMPLE_PATH "/home/user/convnet/samples"
#define SAMPLE_CNT 156	
#define SAMPLE_SIZE 400
#define SAMPLE_WIDTH 50
#define SAMPLE_HEIGHT 20

#define ETA 0.01

#define N_CONV_LAYERS 1
#define N_KERNELS 1   
#define KERNEL_WIDTH 7

struct sample {
	double *data;
	double target[2];
};

static void shufflearr(int *pathidx, int len)
{
	int n1, n2, tmp;

	n1 = rand() % (len);
	n2 = rand() % (len);

	while (n1 == n2) 
		n2 = rand() % (len);
 
	tmp = *(pathidx + n1);
	*(pathidx + n1) = *(pathidx + n2);
	*(pathidx + n2) = tmp;
}

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

int main()
{
	/*int nn[] = {100, 1};	
	if((cnet = cnetcreat(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error in init convnet\n");
		goto exit_failure;
	}
	net = netcreat(2, nn, ((frame->width - ((KERNEL_WIDTH / 2 + 1) * 2)) / 2) * ((frame->height - ((KERNEL_WIDTH / 2 + 1) * 2)) / 2) * N_KERNELS);
	nettofile(net, cnet, NEURO_PATH);*/
	struct IplImage *img;
	double *out;
	struct neuronet *net;
	struct convnet *cnet;
	struct data *inp;

	net = (struct neuronet *)malloc(sizeof(struct neuronet));
	cnet = (struct convnet *)malloc(sizeof(struct convnet));
	if ((cnet->knls = init_kernels(N_CONV_LAYERS, N_KERNELS, KERNEL_WIDTH)) == NULL) {
		fprintf(stderr, "error initing kernels\n");
		goto exit_failure;
	}
	if (netfromfile(net, cnet, NEURO_PATH) == -1) {
		fprintf(stderr, "error reading nets\n");
		goto exit_failure;
	}

	examples = (struct sample *)malloc(sizeof(struct sample) * TOTAL);
	idxes = (int *)malloc(sizeof(int) * TOTAL);	
	
	for (i = 0; i < SAMPLE_CNT; i++) {
		bzero(name, 256);
		sprintf(name, "%s/%d.png", SAMPLE_PATH, i);
		if ((img = ipl_readimg(name, IPL_RGB_MODE)) == NULL) {
			fprintf(stderr, "error reading image: %s\n", name);
			return 1;
		}
	
		(examples + i)->data = getdata(img);
		(examples + i)->target[0] = 1.0;	
		(examples + i)->target[1] = 0.0;	
		
		*(idxes + i) = i;
		ipl_freeimg(&img);
	}
	for (i = SAMPLE_CNT; i < TOTAL; i++) {
		bzero(name, 256);
		sprintf(name, "%s/%d.png", NONSAMPLE_PATH, i - SAMPLE_CNT);
		if ((img = ipl_readimg(name, IPL_RGB_MODE)) == NULL) {
			fprintf(stderr, "error reading image : %s\n", name);
			return 1;
		}
	
		(examples + i)->data = getdata(img);
		(examples + i)->target[0] = 0.0;	
		(examples + i)->target[1] = 1.0;	

		*(idxes + i) = i;
		ipl_freeimg(&img);
	}
	inp->w = SAMPLE_WIDTH;
	inp->h = SAMPLE_HEIGHT;
	double error;
	do { //while(error > 0.018/*isguncor != CNTGUNS || notguncor != CNTNOTGUNS*/) {
		//isguncor = isgunincor = notguncor = notgunincor = 0;
		error = 0;
		for (i = 0; i < TOTAL; i++) {
			int idx;
			double isgun_val, isnotgun_val;
			double *data;
			idx = *(idxes + i);
			out = convfpass((examples + idx)->data, cnet, net);

			isgun_val = *(out + net->total_nn - 2);		
			isnotgun_val = *(out + net->total_nn - 1);		
			
			//error += abs(((*((examples + idx)->target) == 1)? isgun_val : isnotgun_val) - *((examples + idx)->target));
			
			if (*((examples + idx)->target) == 1.0)
				error += 1.0 - isgun_val;
			if (*((examples + idx)->target) == 0.0)
				error += 1.0 - isnotgun_val;

			error += fabs((examples + idx)->target[0] - isgun_val) + fabs((examples + idx)->target[1] - isnotgun_val);


			//printf("idx = %d    tar1 = %lf   tar2 = %lf\n", idx,*((examples + idx)->target), *((examples + idx)->target + 1));
			/*if (*((examples + idx)->target) == 1.0 && isgun_val >= isnotgun_val)
				isguncor++;
			else if (*((examples + idx)->target) == 1.0 && isgun_val < isnotgun_val)
				notgunincor++;
			else if (*((examples + idx)->target) == 0.0 && isnotgun_val >= isgun_val)
				notguncor++;
			else if (*((examples + idx)->target) == 0.0 && isnotgun_val < isgun_val)
				isgunincor++;
			*/
			data = connect_fm(cnet->pmaps, cnet->n_kernels);
			inp->data = (examples + idx)->data;
			c_er = netbpass(net, data, out, (examples + idx)->target, ETA);
			convbpass(cnet, &c_er, inp, ETA);
			//getchar();	
			free(out);
			free(data);
			/*w = net->w;
			for (i = 0; i < net->nl; i++) {
				for(j = 0; j < net->nn[i]; j++) {
					for(k = 0; k < net->nw[i]; k++)
						printf("%lf|", *w++);
					printf(" ");
				}
				printf("\n");
			}*/
			
		}
		shufflearr(idxes, TOTAL);
		printf("error = %lf\n", (error / TOTAL / 2));
		//printf("isguncor = %d isgunincor = %d    notguncor = %d  notgunincor = %d\n", isguncor, isgunincor, notguncor, notgunincor);
		//nettofile(net, NEURO_PATH);

	} while((error / TOTAL / 2) > 0.02);

	return 0;	
}
