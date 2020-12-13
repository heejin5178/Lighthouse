#include <stdio.h>

#define MAX 500

#define MAGIC 0.5

double CDF(int* dataset, int num, int x)
{
	int count;
	double res;

	count = 0;
	for(int i = 0; i < num; ++i) {
		if(dataset[i] <= x) {
			count++;
		}
	}
	res = (double)count / num;
	return res;
}

int is_large(int* dataset, int num, int x)
{
	return MAGIC <= CDF(dataset, num, x);
}

int main(void)
{
	int sample[MAX] = {1, 2, 3, 4, 5, 6, 7, 8};
	int num = 8;
	int new_data = 3;
	double cdf;

	sample[num++] = new_data;

	if(is_large(sample, num, new_data))
		printf("Data size is big.\n");
	else
		printf("Data size is small.\n");	
	return 0;
}
