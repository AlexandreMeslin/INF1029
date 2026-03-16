#include <immintrin.h>
#include <stdio.h>

/**
 * This program demonstrates the use of the _mm256_maskload_epi32 intrinsic 
 * to selectively load elements from an integer array into a 256-bit wide vector register based on a specified mask.
 * The mask determines which elements from the input array are loaded into the result vector, 
 * with non-zero mask values indicating that the corresponding element should be loaded, 
 * and zero mask values indicating that the corresponding element should not be loaded.
 * 
 * To compile this program, you need to use a compiler that supports AVX intrinsics and enable the appropriate flags (e.g., -mavx2 for GCC).
 * 
 * To compile this program, you can use the following command:
 * gcc -Wall -mavx2 -o mask_load mask_load.c
 * 
 * To run the compiled program, simply execute it from the command line:
 * ./mask_load
 */
int main() {

  int int_array[8] = {100, 200, 300, 400, 500, 600, 700, 800};
  
  /* Initialize the mask vector */
  __m256i mask = _mm256_setr_epi32(-20, -72, -48, -9, -100, 3, 5, 8);

  /* Selectively load data into the vector */
  __m256i result = _mm256_maskload_epi32(int_array, mask);
  
  /* Display the elements of the result vector */
  int* res = (int*)&result;
  printf("%d %d %d %d %d %d %d %d\n", 
    res[0], res[1], res[2], res[3], res[4], res[5], res[6], res[7]);
  
  return 0;
}
