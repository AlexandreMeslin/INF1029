#include <cpuid.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    unsigned int cpuid_required_level = 11, cpuid_leaf = 1, max_level =0, vendor_id =0;
    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;

    /* Check if extended feature information is available */
    max_level = __get_cpuid_max(0, &vendor_id);
    if (max_level < cpuid_required_level) {
        printf("Error: cpuid does not provide cpu feature informattion.\n");
        return 1;
    }

    __cpuid_count(cpuid_required_level, cpuid_leaf, eax, ebx, ecx, edx);

    printf("Logical Processors: %d\n", ebx);
    printf("CPUID requested by: CPU#%d\n", edx);
    
    return 0;
}
