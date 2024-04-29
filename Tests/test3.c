#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "STD_TYPES.h"
#include "HMM.h"
#define NUM_ALLOCS 1000
#define MAX_ALLOC_SIZE 1000
#define CORRUPTION_SIZE 8 // Number of bytes to corrupt in allocated memory
int main() {
    void* ptrs[NUM_ALLOCS] = {NULL};

    // Allocate memory
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = rand() % MAX_ALLOC_SIZE + 1;
        ptrs[i] = malloc(size);
        if (ptrs[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            // Cleanup allocated memory before exiting
            for (int j = 0; j < i; j++) {
                free(ptrs[j]);
            }
            exit(EXIT_FAILURE);
        }

        // Introduce memory corruption
        memset(ptrs[i], 0xFF, CORRUPTION_SIZE);
    }

    // Check for memory corruption
    for (int i = 0; i < NUM_ALLOCS; i++) {
        uintptr_t start_addr = (uintptr_t)ptrs[i];
        for (uintptr_t addr = start_addr; addr < start_addr + MAX_ALLOC_SIZE; addr++) {
            char* byte_ptr = (char*)addr;
            if (*byte_ptr != 0 && *byte_ptr != (char)0xFF) {
                fprintf(stderr, "Memory corruption detected at address %p\n", (void*)addr);
                // Cleanup allocated memory before exiting
                for (int j = 0; j < NUM_ALLOCS; j++) {
                    free(ptrs[j]);
                }
                exit(EXIT_FAILURE);
            }
        }
    }

    // Free memory
    for (int i = 0; i < NUM_ALLOCS; i++) {
        free(ptrs[i]);
    }

    printf("Memory allocation and corruption check completed successfully!\n");

    return 0;
}

#endif