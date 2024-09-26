#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memup.h"

void get_memory_info() {
#ifdef __APPLE__
    int mib[2];
    int64_t physical_memory;
    size_t len = sizeof(physical_memory);
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    sysctl(mib, 2, &physical_memory, &len, NULL, 0);
    
    // More memory calculations for macOS can be added here.
    printf("Physical memory: %.2f GiB\n", (double)physical_memory / (1024 * 1024 * 1024));
#elif defined(__linux__) || defined(__GNU__)
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        printf("Memory calculation error (ERROR 0xHMM)\n");
        return;
    }
    
    char line[256];
    int total_memory = 0;
    int free_memory = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%d", &total_memory);
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line + 13, "%d", &free_memory);
        }
    }
    fclose(file);
    
    printf("Memory: %.2f GiB / %.2f GiB\n", 
           (double)(total_memory - free_memory) / (1024 * 1024), 
           (double)total_memory / (1024 * 1024));
#else
    printf("what os are you using again? (error: 0xeww)\n");
#endif
}
