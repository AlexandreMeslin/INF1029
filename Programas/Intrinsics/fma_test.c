#include <immintrin.h>
#include <stdio.h>

/**
 * This program demonstrates the use of the _mm256_fmaddsub_pd intrinsic to perform a fused multiply-add and subtract operation on double-precision floating-point vectors.
 * It initializes three vectors of double-precision floating-point numbers, and then computes the result by alternately subtracting and adding the third vector from the product of the first and second vectors.
 * The _mm256_fmaddsub_pd intrinsic computes the operation (a * b) + c for even-indexed elements and (a * b) - c for odd-indexed elements, where a, b, and c are the corresponding elements from the input vectors.
 * 
 * Note: To compile this program, you need to use a compiler that supports AVX intrinsics and enable the appropriate flags (e.g., -mfma for GCC).
 * 
 * To compile this program, you can use the following command:
 * gcc -Wall -mfma -o fma_test fma_test.c
 * 
 * To run the compiled program, simply execute it from the command line:
 * ./fma_test
 */
int main() {
  __m256d veca = _mm256_setr_pd(6.0, 6.0, 6.0, 6.0);

  __m256d vecb = _mm256_setr_pd(2.0, 2.0, 2.0, 2.0);
  
  __m256d vecc = _mm256_setr_pd(7.0, 7.0, 7.0, 7.0);
  
  /* Alternately subtract and add the third vector
     from the product of the first and second vectors */
  __m256d result = _mm256_fmaddsub_pd(veca, vecb, vecc);
  
  /* Display the elements of the result vector */
  double* res = (double*)&result;
  printf("%lf %lf %lf %lf\n", res[0], res[1], res[2], res[3]);
  
  return 0;
}
