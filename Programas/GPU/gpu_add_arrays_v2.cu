/**
 * @file gpu_add_arrays_v2.cu
 * @brief This program adds two arrays of floating-point numbers using CUDA.
 * 
 * The program allocates memory for two arrays on the host and the device, 
 * initializes the host arrays, copies them to the device, 
 * runs a kernel to add the elements of the arrays, copies 
 * the result back to the host, checks for errors, 
 * and finally frees the allocated memory. 
 * The program also measures and prints the time taken for each step and the overall execution time.
 */
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define timedifference_msec(start, stop) ((double)(stop - start) * 1000.0 / CLOCKS_PER_SEC)

#define DATASET_SIZE 1024000

#define THREADS_PER_BLOCK 256
#define MAX_BLOCKS_PER_GRID 4096

/**
 * @brief Kernel function to add the elements of two arrays
 * 
 * A função soma UM elemento de cada array, 
 * onde o índice do elemento é calculado a partir do índice do bloco e do índice da thread.
 * 
 * @param n The number of elements in the arrays
 * @param d_x The first input array (device pointer)
 * @param d_y The second input array (device pointer) and output array (device pointer)
 */
__global__ void add(int n, float *d_x, float *d_y) {
      int index = blockIdx.x * blockDim.x + threadIdx.x;

      if (index < n) {
            d_y[index] = d_x[index] + d_y[index];
      }
}

/**
 * @brief Main function to execute the program
 * 
 * The main function performs the following steps:
 * 1. Allocates memory for two arrays on the host and the device.
 * 2. Initializes the host arrays.
 * 3. Copies the host arrays to the device.
 * 4. Runs the kernel to add the elements of the arrays on the device.
 * 5. Copies the result back to the host.
 * 6. Checks for errors in the result.
 * 7. Frees the allocated memory.
 * 8. Measures and prints the time taken for each step and the overall execution time.
 */
int main(int argc, char **argv) {
      float *h_x, *h_y;
      float *d_x, *d_y;
      cudaError_t cudaError;
      int i;
      clock_t start, stop, overall_start, overall_stop;

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

        if (cudaError != cudaSuccess) {
            printf("cudaMemcpy (h_x -> d_x) returned error %s (code %d), line(%d)\n", cudaGetErrorString(cudaError), cudaError, __LINE__);
            return 1;
      }

      stop = clock();
      printf("%f ms\n", timedifference_msec(start, stop));

      // Run kernel on elements on the GPU
      printf("Running kernel on elemnts of d_x and d_y...");
      start = clock();

      int blockSize = THREADS_PER_BLOCK;
      int numBlocks = (DATASET_SIZE + blockSize - 1) / blockSize;
      if (numBlocks > MAX_BLOCKS_PER_GRID) numBlocks = MAX_BLOCKS_PER_GRID;

      add<<<numBlocks, blockSize>>>(DATASET_SIZE, d_x, d_y);

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

      //Mark overall stop time
      overall_stop = clock();
      // Show elapsed time
      printf("Overall time: %f ms\n", timedifference_msec(overall_start, overall_stop));

      // Return exit code
      return 0;
}
