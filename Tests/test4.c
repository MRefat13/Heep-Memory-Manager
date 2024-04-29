#if 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "STD_TYPES.h"
#include "FreeList.h"
#include "HMM.h"


#define NUM_OPERATIONS 100000
#define MAX_SIZE (100000)

enum Operation
{
    MALLOC,
    CALLOC,
    REALLOC,
    FREE
};

void *allocated_blocks[NUM_OPERATIONS];

void perform_random_operations()
{

    srand(time(NULL));

    for (int i = 0; i < NUM_OPERATIONS; i++)
    {
        enum Operation op = rand() % 4;
        switch (op)
        {
        case MALLOC:
        {
            size_t size = rand() % MAX_SIZE + 1;
            allocated_blocks[i] = HmmAlloc(size);
            //printf("Allocated block %p with size %zu\n", allocated_blocks[i], size);
            break;
        }
        case CALLOC:
        {
            size_t num = rand() % MAX_SIZE + 1;
            //size_t size = rand() % MAX_SIZE + 1;
            allocated_blocks[i] = HmmCalloc(num, MAX_SIZE/num);
            //printf("Allocated block %p with %zu elements of size %zu\n", allocated_blocks[i], num, size);
            break;
        }
        case REALLOC: {
            if (i > 0) {
                int idx = rand() % i;
                size_t size = rand() % MAX_SIZE + 1;
                allocated_blocks[idx] = HmmRealloc(allocated_blocks[idx], size);
                //printf("Reallocated block %p with size %zu\n", allocated_blocks[idx], size);
            }
            break;
        }
        case FREE:
        {
            if (i > 0)
            {
                int idx = rand() % i;
                HmmFree(allocated_blocks[idx]);
                allocated_blocks[idx] = NULL;
                //printf("Freed block %p\n", allocated_blocks[idx]);
            }
            break;
        }
        }
    }
}

int main()
{
    perform_random_operations();
    printf("Test Done Successfully\n");
    return 0;
}

#endif