#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define timedifference_msec(start, stop) ((double)(stop - start) * 1000.0 / CLOCKS_PER_SEC)

#define DATASET_SIZE 1024000

__global__ void add(int n, float *d_x, float *d_y);

int main(int argc, char **argv) {
      float *h_x, *h_y;
      float *d_x, *d_y;
      cudaError_t cudaError;
      int i;
      clock_t start, stop;
      clock_t overall_start, overall_stop;

      // Mark overall start time
      overall_start = clock();

      // Disable buffering entirely
      setbuf(stdout, NULL);
 
      // Allocating arrays on host
      printf("Allocating arrays h_x and h_y on host...");
      start = clock();

      h_x = (float*)malloc(DATASET_SIZE*sizeof(float));
      h_y = (float*)malloc(DATASET_SIZE*sizeof(float));

      // check malloc memory allocation
      if (h_x == NULL || h_y == NULL) {
            printf("Error: malloc unable to allocate memory on host.");
            return 1;
      }
      
      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));
  
      // Allocating array on device
      printf("Allocating array d_x and d_y on device...");
      start = clock();

      cudaError = cudaMalloc(&d_x, DATASET_SIZE*sizeof(float));

      // check cudaMalloc memory allocation
      if (cudaError != cudaSuccess) {
            printf("cudaMalloc d_x returned error %s (code %d)\n", 
		      cudaGetErrorString(cudaError), cudaError);
            return 1;
      }

      cudaError = cudaMalloc(&d_y, DATASET_SIZE*sizeof(float));

      // check cudaMalloc memory allocation
      if (cudaError != cudaSuccess) {
            printf("cudaMalloc d_y returned error %s (code %d)\n", 
		      cudaGetErrorString(cudaError), cudaError);
            return 1;
      }

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));
  
      // Initialize host memory
      printf("Initializing array h_x and h_y on host...");
      start = clock();

      for (i =0; i < DATASET_SIZE; ++i) {
            h_x[i] = 1.0f;
            h_y[i] = 2.0f;
      }

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));

      // Copy array from host to device
      printf("Copying arrays from host to device...");
      start = clock();

      cudaError = cudaMemcpy(d_x, h_x, DATASET_SIZE*sizeof(float), cudaMemcpyHostToDevice);

      if (cudaError != cudaSuccess) {
            printf("cudaMemcpy (h_x -> d_x) returned error %s (code %d), line(%d)\n", cudaGetErrorString(cudaError), cudaError, __LINE__);
            return 1;
      }

      cudaError = cudaMemcpy(d_y, h_y, DATASET_SIZE*sizeof(float), 
	      cudaMemcpyHostToDevice);

      if(cudaError != cudaSuccess) {
            printf("cudaMemcpy (h_x -> d_x) returned error %s (code %d), line(%d)\n", cudaGetErrorString(cudaError), cudaError, __LINE__);
            return 1;
      }

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));
 
      // Run kernel on elements on the GPU
      printf("Running kernel on elemnts of d_x and d_y...");
      start = clock();

      add<<<1, 1>>>(DATASET_SIZE, d_x, d_y);

      // Wait for GPU to finish before accessing on host
      cudaDeviceSynchronize();

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));
      
      // Copy array from device to host
      printf("Copying array from device (d_y) to host (h_y)...");
      start = clock();

      cudaError = cudaMemcpy(h_y, d_y, DATASET_SIZE*sizeof(float), cudaMemcpyDeviceToHost);

      if (cudaError != cudaSuccess) {
            printf("cudaMemcpy (d_y -> h_y) returned error %s (code %d), line(%d)\n", cudaGetErrorString(cudaError), cudaError, __LINE__);
            return 1;
      }

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));
  
      // Check for errors (all values should be 3.0f)
      printf("Checking for processing errors...");
      start = clock();

      float maxError = 0.0f;
      float diffError = 0.0f;
      for (i = 0; i < DATASET_SIZE; i++) {
            maxError = (maxError > (diffError=fabs(h_y[i]-3.0f)))? maxError : diffError;
      }

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));
      printf("Max error: %f\n", maxError);
 
      // Free memory
      printf("Freeing memory...");
      start = clock();
      cudaFree(d_x);
      cudaFree(d_y);
      free(h_x);
      free(h_y);
      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));

      // Mark overall stop time
      overall_stop = clock();
      // Show elapsed time
      printf("Overall time: %f ms\n", timedifference_msec(overall_start, overall_stop));

      // Return exit code
      return 0;
}

// Kernel function to add the elements of two arrays
__global__ void add(int n, float *d_x, float *d_y) {
      for (int i = 0; i < n; ++i) {
            d_y[i] = d_x[i] + d_y[i];
      }
}
