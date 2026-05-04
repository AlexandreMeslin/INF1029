#include <sys/time.h>

float timedifference_msec(struct timeval t0, struct timeval t1);

#ifdef GCC
#ifndef GCC_timer
#define GCC_timer

#define cudaSuccess	1
#define cudaMemcpyDeviceToHost 1
#define cudaMemcpyHostToDevice 2
#define __global__

typedef int cudaError_t;
typedef struct {
	int x, y, z;
} BLOCKIDX;

typedef struct {
	int x, y, z;
} THREADIDX;

typedef struct {
	int x, y, z;
} BLOCKDIM;

typedef struct {
	int x, y, z;
} GRIDDIM;

BLOCKDIM	blockDim;
BLOCKIDX	blockIdx;
THREADIDX	threadIdx;
GRIDDIM		gridDim;

int cudaMalloc(float **p, long size);
char *cudaGetErrorString(int erro);
int cudaMemcpy(void *pDest, void *pOrig, int size, int nope);
void cudaFree(void *p);
void cudaDeviceSynchronize(void);
#endif

extern BLOCKIDX		blockIdx;
extern THREADIDX	threadIdx;
extern BLOCKDIM		blockDim;
extern GRIDDIM		gridDim;
#endif
