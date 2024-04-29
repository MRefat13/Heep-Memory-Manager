#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "STD_TYPES.h"
#include "HMM_Config.h"
#include "FreeList.h"
#include "HMM.h"


void *pMyHeapTop = NULL_ptr;
freeList_t list;
int flag = 0;
#define GetProgBreak() (sbrk(0))


#define Size_alignment(size)\
do\
{\
    if ( size % 8 != 0)\
    {\
            size = size + (8 - (size%8));\
    }\
}while (0)

static uint32_t customCeil(f32_t num) {
    int integerPart = (int)num;
    double fractionalPart = num - integerPart;
    if (fractionalPart > 0) {
        return integerPart + 1;
    } else {
        return integerPart;
    }
}

void *malloc(uint32_t size)
{   
    void *pReturnAddress = NULL_ptr;
    block_t *pSuitableBlock = NULL_ptr;
    error_t kErrorState = kNoError;
    uint32_t mulFactor = 1;
    // Align the size to 8 byte
    Size_alignment(size);
    // Calculate the total size
    uint32_t totalSize = size + sizeof(metaData_t);
    // 1 - Search in the Free list to fund a suitable free block
    pSuitableBlock = FreeList_FindSuitableBlock(&list, size);
    // Case[1]: There is a free block in the free list is suitable
    if ( pSuitableBlock != NULL_ptr )
    {
        // Case[1-1]: If the free block is larger than the desired length[size]
        if ( pSuitableBlock->length > size)
        {
            pReturnAddress = FreeList_SplitBlock(&list, pSuitableBlock, size);
        }
        // Case[1-2]: If the free block length is equal to the desired length[size]
        else if ( pSuitableBlock->length == size)
        {
            // Remove the free block from the free list
            kErrorState &= FreeList_DeleteBlock(&list, pSuitableBlock);
            pReturnAddress = (void*)pSuitableBlock;
        }
        flag = 0;
    }
    // Case[2]: There is no suitable free block in the free list
    else if ( pSuitableBlock == NULL_ptr)
    {
        /* 
          Allocate a new block of memory by allocating from the top of the heap
          or by extending the heap if needed
        */

        // First Check if the desired size is larger than SIZE_OF_PAGE * NUM_OF_PAGES 
        if ( totalSize  > (SIZE_OF_PAGE * NUM_OF_PAGES))
        {
            // Calculating how may blocks should be allocated
            // the size of the block here is [SIZE_OF_PAGE * NUM_OF_PAGES]
            mulFactor = customCeil( (f32_t)totalSize / (SIZE_OF_PAGE * NUM_OF_PAGES));
        }

        // Case[2-1]: Fist malloc call
        if ( pMyHeapTop == NULL_ptr )
        {
            // Extend the heap by increasing the program break
            pMyHeapTop = sbrk(SIZE_OF_PAGE * NUM_OF_PAGES * mulFactor);
            if ( pMyHeapTop != SBRK_ERROR)
            {
                /* 
                    TODO: Make Free list init in free function not here
                */
                // Initialize the free list
                FreeList_Init(&list);
            }else
            {
                kErrorState = kError;
            }
            flag =0;
        }
        // Case[2-2]: There is no enough space in the heap
        else if ((void*)((char*)pMyHeapTop + totalSize) > GetProgBreak() ||
                    GetProgBreak() == pMyHeapTop)
         {
            // Extend the heap by increasing the program break 
            if ( sbrk(SIZE_OF_PAGE * NUM_OF_PAGES * mulFactor) == SBRK_ERROR )
            {
                kErrorState = kError;
            }
            flag = 1;
        }
    }

    // Case No error occurred either by finding a free block or by extending the heap
    if ( kErrorState != kError)
    {
        // Ccheck if there was a free block in the free list
        if ( pSuitableBlock == NULL_ptr )
        {
            // Assign the return address to the top of my heap
            pReturnAddress = pMyHeapTop;
            // update the top of the heap
            pMyHeapTop = (void*)((char*)pMyHeapTop + totalSize);
        }

        //metaData_t locMetaData = {size, ALLOCATED_SIGNATURE};
        // Add Meta Data to the reserved block of memory
        ((metaData_t*)pReturnAddress)->length = size;
        ((metaData_t*)pReturnAddress)->signature = ALLOCATED_SIGNATURE;
        // Increment the returned address by the size of the meta data block
        // cast it first to (char *) to make the step size of the ptr 1 byte
        pReturnAddress = (void*)((char *)pReturnAddress+sizeof(metaData_t));

        if ( pMyHeapTop > GetProgBreak())
        {
            if ( pSuitableBlock == NULL_ptr)
            {
                printf("Suitable block is NULL\n");
            }
            printf("Current Heap Top:      %p\n",pMyHeapTop);
            printf("Current Program break: %p\n",sbrk(0));
            printf("Current block size     %d\n",totalSize - 24);
            assert(0&& "Exceeding the program break");
        }
    }
    return pReturnAddress;
}


void *calloc(uint32_t nmemb, uint32_t size)
{
    void *pReturnAddress = NULL_ptr;
    Size_alignment(size);
    pReturnAddress =  malloc(nmemb * size);
    // Fill the allocated data with zeros
    if ( pReturnAddress != NULL_ptr)
    {
        pReturnAddress = memset(pReturnAddress, 0, size);
    }
    return pReturnAddress;
}

void *realloc(void *ptr, uint32_t size)
{
    void *pReturnAddress = NULL_ptr;
    // Case[1] 
    if ( ptr == NULL_ptr)
    {
        pReturnAddress = malloc(size);
    }
    // Case[2]
    else if ( size == 0)
    {
        free(ptr);
    }
    //Case[3]
    else
    {
        // Get Meta Data of the allocated block
        metaData_t *pMetaData = (metaData_t *)( (char*)ptr - sizeof(metaData_t));
        // Case[3-1] Check if ptr is a valid pointer
        if ( pMetaData->signature == ALLOCATED_SIGNATURE )
        {
            // First align the size to 8 byte
            Size_alignment(size);
            // Case[3-1-1] : if size < old block size
            if ( size < pMetaData->length)
            {
                uint32_t remainingSize = pMetaData->length - size;
                pReturnAddress = ptr;
                // Check if the remaining size > size of metadata
                if ( remainingSize > sizeof(metaData_t))
                {
                    // Creat a free block and insert it in the free list
                    block_t *pblock = (block_t *)((char*)ptr + size);
                    pblock->length = remainingSize - sizeof(metaData_t);
                    FreeList_InsertBlock(&list, pblock);
                }else
                {
                    // The remaining size will be fragmented 
                }
                // Update the meta data
                pMetaData->length = size;
            }
            // Case[3-1-2] : if size > old block size
            else if ( size > pMetaData->length)
            {
                // Allocate a block with the new size
                pReturnAddress = malloc(size);
                if ( pReturnAddress != NULL_ptr)
                {
                    // Copy the old data to the new blok of memory
                    /* 
                        TODO: change the type from char to long int after doing the alignment                    
                     
                    */
                   uint64t iterator = 0;
                   for ( iterator = 0; iterator < (pMetaData->length)/8; iterator++ )
                   {
                        ((sint64_t*)pReturnAddress)[iterator] = ((sint64_t*)ptr)[iterator];
                   }
                   // Free the old block
                   free(ptr);
                }
            }
        }
    }
    return pReturnAddress;
}

void free(void *ptr)
{
    if (ptr != NULL_ptr)
    {
        error_t kErrorState = kNoError;
        metaData_t *pLocMetaData = (metaData_t *)((char*)ptr-sizeof(metaData_t));
        // Check if the ptr is a valid pointer
        // This condition is not valid for next time you try to free the same freed block
        if ( pLocMetaData->signature == ALLOCATED_SIGNATURE )
        {   
            pLocMetaData->signature = 0;
            block_t *pblock = (block_t *)((char*)ptr-sizeof(metaData_t));
            pblock->length = pLocMetaData->length;
            kErrorState = FreeList_InsertBlock(&list, pblock);

            // Check if the insert is done successfully
            if ( kErrorState == kNoError)
            {
                // Check if The next block is a free block
                if ( FreeList_IsContigious(&list,pblock, pblock->pNext) == kTrue)
                {
                    // Merge these two blocks of memory
                    kErrorState &= FreeList_MergeBlocks(&list, pblock, pblock->pNext);
                }
                // Check if The previous block is a free block
                if ( FreeList_IsContigious(&list,pblock->pPrevious, pblock) == kTrue)
                {
                    // Merge these two blocks of memory
                    kErrorState &= FreeList_MergeBlocks(&list, pblock->pPrevious, pblock);
                }
            }
            kErrorState = FreeList_LowerProgramBreak(&list);
            if ( kErrorState == kError)
            {
                /* 
                    TODO: 
                */
               assert(0&&"Cannot lower program break\n");
            }
        }else
        {
            assert(0&&"[free]Invalid Pointer");
        }
    }
}