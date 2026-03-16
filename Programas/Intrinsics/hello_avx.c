#include <immintrin.h>
#include <stdio.h>

/**
 * This program demonstrates the use of AVX intrinsics to perform vectorized operations on 256-bit wide registers. 
 * It initializes two vectors of single-precision floating-point numbers, 
 * computes their difference using the _mm256_sub_ps intrinsic, 
 * and then prints the resulting vector elements to the console.
 * 
 * Note: To compile this program, you need to use a compiler that supports AVX intrinsics and enable the appropriate flags (e.g., -mavx for GCC).
 * 
 * To compile this program, you can use the following command:
 * gcc -Wall -mavx -o hello_avx hello_avx.c
 * 
 * To run the compiled program, simply execute it from the command line:
 * ./hello_avx
 */
int main() {
  /* Initialize the two argument vectors */
  __m256 evens = _mm256_set_ps(2.0, 8.0, 32.0, 128.0, 512.0, 2048.0, 8192.0, 32768.0);
  __m256 odds = _mm256_set_ps(1.0, -4.0, 16.0, -64.0, 256.0, -1024.0, 4096.0, -16384.0);

  /* Compute the difference between the two vectors */
  __m256 result = _mm256_sub_ps(evens, odds);

  /* Display the elements of the result vector */
  float* f = (float*)&result;
  printf("%f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

  return 0;
}
