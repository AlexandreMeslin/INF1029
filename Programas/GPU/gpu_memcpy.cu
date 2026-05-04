/**
 * @file gpu_memcpy.cu
 * @brief This program demonstrates how to perform memory copy operations between host and device using CUDA.
 * @author NVIDIA Corporation
 */
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <time.h>

#define timedifference_msec(start, stop) ((double)(stop - start) * 1000.0 / CLOCKS_PER_SEC)

#define DATASET_SIZE 1024000

/**
 * This function demonstrates the use of cudaMemcpy to copy data between host and device memory.
 * It performs the following steps:
 * 1. Allocates memory on the host for two arrays (h_x and h_y).
 * 2. Allocates memory on the device for one array (d_x).
 * 3. Initializes the host array h_x with values.
 * 4. Copies the host array h_x to the device array d_x using cudaMemcpy.
 * 5. Copies the device array d_x back to the host array h_y using cudaMemcpy.
 * 6. Checks for errors by comparing the values in h_x and h_y.
 * 7. Frees the allocated memory on both host and device.
 * 8. Measures and prints the time taken for each operation and the overall execution time.
 */
int main(int argc, char **argv)
{
      float *h_x, *h_y;
      float *d_x;
      cudaError_t cudaError;
      int i;
      clock_t start_time, stop_time, overall_start_time, overall_stop_time;

      // Mark overall start time
      overall_start_time = clock();
      // Disable buffering entirely
      setbuf(stdout, NULL);

      // Allocating arrays on host
      printf("Allocating arrays h_x e h_y on host...");
      start_time = clock();

      h_x = (float *)malloc(DATASET_SIZE * sizeof(float));
      h_y = (float *)malloc(DATASET_SIZE * sizeof(float));

      // check malloc memory allocation
      if (h_x == NULL || h_y == NULL) {
            printf("Error: malloc unable to allocate memory on host.");
            return 1;
      }

      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));

      // Allocating array on device
      printf("Allocating array d_x on device...");
      start_time = clock();

      cudaError = cudaMalloc(&d_x, DATASET_SIZE * sizeof(float));

      // check cudaMalloc memory allocation
      if (cudaError != cudaSuccess) {
            printf("cudaMalloc d_x returned error %s (code %d)\n",
                  cudaGetErrorString(cudaError), cudaError);
            return 1;
      }

      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));

      // Initialize host memory
      printf("Initializing array h_x on host...");
      start_time = clock();

      for (i = 0; i < DATASET_SIZE; ++i) h_x[i] = (float)i;

      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));

      // Copy array from host to device
      printf("Copying array from host (h_x) to device (d_x)...");
      start_time = clock();

      cudaError = cudaMemcpy(d_x, h_x, DATASET_SIZE * sizeof(float), cudaMemcpyHostToDevice);

      if (cudaError != cudaSuccess) {
            printf("cudaMemcpy (h_x -> d_x) returned error %s (code %d), 		line(%d)\n", cudaGetErrorString(cudaError), cudaError, __LINE__);
            return 1;
      }

      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));

      // Copy array from device to host
      printf("Copying array from device (d_x) to host (h_y)...");
      start_time = clock();

      cudaError = cudaMemcpy(h_y, d_x, DATASET_SIZE * sizeof(float), cudaMemcpyDeviceToHost);

      if (cudaError != cudaSuccess) {
            printf("cudaMemcpy (d_x -> h_y) returned error %s (code %d),            line(%d)\n", cudaGetErrorString(cudaError),
                   cudaError, __LINE__);
            return 1;
      }

      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));

      // Check for errors (all values should be 0.0f)
      printf("Checking for processing errors...");
      start_time = clock();

      float maxError = 0.0f;
      float diffError = 0.0f;
      for (i = 0; i < DATASET_SIZE; i++)
            maxError = (maxError > (diffError = fabs(h_x[i] - h_y[i]))) ? maxError : diffError;

      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));
      printf("Max error: %f\n", maxError);

      // Free memory
      printf("Freeing memory...");
      start_time = clock();
      cudaFree(d_x);
      free(h_x);
      free(h_y);
      stop_time = clock();
      printf("%f ms\n", timedifference_msec(start_time, stop_time));

      // Mark overall stop time
      overall_stop_time = clock();
      // Show elapsed time
      printf("Overall time: %f ms\n", timedifference_msec(overall_start_time, overall_stop_time));

      // Return exit code
      return 0;
}
