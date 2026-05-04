#include <sys/time.h>
#include "timer.h"
#include <stdlib.h>

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

#ifdef GCC
int cudaMalloc(float **p, long size) {
	if((*p = malloc(size)))
		return cudaSuccess;
	else
		return 8752;
}

char *cudaGetErrorString(int erro) {
	return "Deu erro";
}

int cudaMemcpy(void *pDest, void *pOrig, int size, int nope) {
	char *dest, *orig;
	dest = pDest;
	orig = pOrig;
	for(int i=0; i<size; i++) {
		dest[i] = orig[i];
	}
	return cudaSuccess;
}

void cudaFree(void *p) {
	free(p);
}

void cudaDeviceSynchronize(void) {
}
#endif
