#include <stdio.h>
#include "STD_TYPES.h"
#include "HMM.h"
#include <stdlib.h>

#if 0

#define NUM_ALLOCS 1000000
#define MAX_ALLOC_SIZE 1000

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
    }

    // Free memory
    for (int i = 0; i < NUM_ALLOCS; i++) {
        free(ptrs[i]);
    }

    printf("Allocations and deallocations completed successfully!\n");

    return 0;
}
#endif