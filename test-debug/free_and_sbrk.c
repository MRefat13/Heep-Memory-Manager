
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "STD_TYPES.h"
#include "FreeList.h"
#include "HMM.h"
#if 1
#define MAX_ALLOCS 1000000

#define getInt(x, ...) atoi(x)

extern char end, edata, etext;
extern freeList_t list;
int
main(int argc, char *argv[])
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;
    printf("etext = %p, edata=%p, end=%p, initial program break=%p\n", &etext, &edata, &end, (char*)sbrk(0));
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("%s num-allocs block-size [step [min [max]]]\n", argv[0]);
        // exit(1);
    }

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");
    if (numAllocs > MAX_ALLOCS) {
        printf("num-allocs > %d\n", MAX_ALLOCS);
        // exit(1);
    }

    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
    freeMin =  (argc > 4) ? getInt(argv[4], GN_GT_0, "min") : 1;
    freeMax =  (argc > 5) ? getInt(argv[5], GN_GT_0, "max") : numAllocs;

    if (freeMax > numAllocs) {
        printf("free-max > num-allocs\n");
        // exit(1);
    }

    printf("Initial program break:          %10p\n", sbrk(0));

    printf("Allocating [numAllocs]: %d * [blockSize]%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; j++) {
        ptr[j] = malloc(blockSize);
        if (ptr[j] == NULL) {
            printf("malloc returned null\n");
            // exit(1);
        }
    }

    printf("Before free(), program break is: %10p\n", sbrk(0));
    printf("Freeing blocks from %d to %d in steps of %d\n",
                freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j += freeStep)
         free(ptr[j]);

    printf("After free(),  program break is: %10p\n", sbrk(0));
    printf("Free List size is:  %d\n",list.size);
    printf("Last Block size is: %d\n",list.pTail->length);
    // exit(EXIT_SUCCESS);
}
#endif  
