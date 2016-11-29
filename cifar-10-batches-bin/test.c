#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *f;
	int *a;
	int n;
	a = (int *)malloc(sizeof(int) * 3072);
	f = fopen("data_batch_1.bin", "r");
	fscanf(f, "%d", &n);
	fread(a, sizeof(char), 3072, f);
	printf("\n %d \n", n);
	int i;
	for (i = 0; i < 32; i++)
		printf("%d ", a[i]);
	return 0;	
}
