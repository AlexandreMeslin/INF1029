#include <cpuid.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char YES[]="YES", NO[]="NO";

  char vendor_str[13];
  unsigned int vendor_id;

  unsigned int max_level;

  unsigned int eax, ebx, ecx, edx;

  /* Basic features for SIMD processing */
  unsigned int has_mmx = 0, has_sse = 0, has_sse2 = 0, has_sse3 = 0, has_ssse3 = 0;

  /* Extended features for SIMD processing */
  unsigned int has_sse4_1 = 0, has_sse4_2 = 0;
  unsigned int has_avx = 0, has_avx2 = 0;
  unsigned int has_fma = 0;
  unsigned int has_avx512f = 0;

  printf("%s: Detecting available cpu features...\n", argv[0]);

 /* Get CPU vendor information */
  if (!__get_cpuid(0, &eax, &ebx, &ecx, &edx)) {
        printf("Error: cpuid failed to provide cpu vendor information.\n");
        exit(1);
  }

  memcpy(vendor_str, &ebx, 4);
  memcpy(vendor_str + 4, &edx, 4);
  memcpy(vendor_str + 8, &ecx, 4);
  vendor_str[12] = '\0';

  printf("CPU Vendor: %s\n", vendor_str);

 /* Check if basic feature information is available */
  max_level = __get_cpuid_max(0, &vendor_id);
  if (max_level < 1) {
    printf("Error: cpuid does not provide cpu feature informattion.\n");
    exit(-1);
  }

  /* Get basic feature information */
  if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
        printf("Error: cpuid failed to provide basic feature information.\n");
        exit(1);
  }

  has_sse3 = ecx & bit_SSE3;
  has_ssse3 = ecx & bit_SSSE3;
  has_sse4_1 = ecx & bit_SSE4_1;
  has_sse4_2 = ecx & bit_SSE4_2;
  has_avx = ecx & bit_AVX;
  has_fma = ecx & bit_FMA;

  has_mmx = edx & bit_MMX;
  has_sse = edx & bit_SSE;
  has_sse2 = edx & bit_SSE2;

  printf("MMX = %s\n", has_mmx ? YES : NO);
  printf("SSE = %s\n", has_sse ? YES : NO);
  printf("SSE2 = %s\n", has_sse2 ? YES : NO);
  printf("SSE3 = %s\n", has_sse3 ? YES : NO);
  printf("SSSE3 = %s\n", has_ssse3 ? YES : NO);
  printf("SSE4_1 = %s\n", has_sse4_1 ? YES : NO);
  printf("SSE4_2 = %s\n", has_sse4_2 ? YES : NO);
  printf("AVX = %s\n", has_avx ? YES : NO);
  printf("FMA = %s\n", has_fma ? YES : NO);

  /* Check if extended feature information is available */
  if (max_level >= 7) {
    /* Get extended feature information: __get_cupid_count nao esta definido no cupid.h */
    if (!__get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx)) {
      printf("Error: cpuid failed to provide extended feature information.\n");
      exit(1);
    }
    has_avx2 = ebx & bit_AVX2;

    __get_cpuid(0, &eax, &ebx, &ecx, &edx);
    has_avx512f = ebx & bit_AVX512F;

    printf("AVX2 = %s\n", has_avx2 ? YES : NO);
    printf("AVX512F = %s\n", has_avx512f ? YES : NO);
  }
}
