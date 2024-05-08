/**
 * @file HMM.c
 * @author Mohamed Refat
 * @brief 
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "STD_TYPES.h"
#include "HMM_Config.h"
#include "FreeList.h"
#include "HMM.h"

/*--------------- Global variable section ----------------*/
void *pMyHeapTop = NULL_ptr;
void *pCurProgBreak = NULL_ptr;
freeList_t list;

/*--------------- Function Like macro variable section ----------------*/
#define GetProgBreak() (sbrk(0))

/**
 * @brief This function is used to align the size to 8 byte 
 * 
 */
#define Size_alignment(size)\
do\
{\
    if ( size % 8 != 0)\
    {\
            size = size + (8 - (size%8));\
    }\
}while (0)


/*--------------- Static function section ----------------*/
/**
 * @brief This function is used to round up the float number to the next integer
 * 
 * @param num float number
 * @return uint32_t rounded number
 */
static uint32_t helper_CustomCeil(f32_t num) {
    int integerPart = (int)num;
    double fractionalPart = num - integerPart;
    if (fractionalPart > 0) {
        return integerPart + 1;
    } else {
        return integerPart;
    }
}
/**
 * @brief This function used to lower the program break
 *          it first check if there is no allocated memory after the last free block
 *          if true get the block and check if the length is grater than the program break step 
 * 
 * @param pList 
 * @return error_t 
 */
static error_t helper_LowerProgramBreak(freeList_t *pList)
{
	error_t kErrorState = kNoError;
	if ( NULL_ptr != pList )
	{
		// Check if there is no allocated blocks after the last block in the free list
		void *pBlockTopAddress = (void*) (( (char *)(pList->pTail)) + pList->pTail->length + sizeof(block_t));
		if ( pMyHeapTop == pBlockTopAddress)
		{
			// Get the last block length
			uint32_t blockLength = pList->pTail->length;
			// Get the difference between pMyHeapTop and the program break
			uint32_t diff = (char *)pCurProgBreak - (char *)pMyHeapTop;
			// Get number of unallocated bytes
			uint32_t unallocated = blockLength + diff;
			// Check if the memory unallocated  is larger than the program break step
			if ( unallocated > SIZE_OF_PAGE*NUM_OF_PAGES )
			{
				// Get number of unallocated steps
				uint32_t mulFactor = unallocated / (SIZE_OF_PAGE*NUM_OF_PAGES);
				sint64_t decrements= (-1) * (sint64_t)(SIZE_OF_PAGE*NUM_OF_PAGES) * mulFactor;
				if ( sbrk( decrements) != SBRK_ERROR )
				{
					// Get the difference between program break and the pMyHeapTop
					uint32_t diff = (char *)pMyHeapTop - (char *)GetProgBreak();
					// Update the size of the last block of memory
					pList->pTail->length = pList->pTail->length - diff;
					// Update the top of the my heap
					pMyHeapTop = GetProgBreak();
                    // Update the current program break
                    pCurProgBreak = GetProgBreak();
				}else
				{
					kErrorState = kError;
				}
			}
		}else
		{
			// There a data allocated after the last free block
		}
	}else
	{
		kErrorState = kError;
	}
	return kErrorState;
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
    // Search in the Free list to fund a suitable free block
    pSuitableBlock = FreeList_FindSuitableBlock(&list, size);
    // Case[1]: There is a free block in the free list is suitable
    if ( pSuitableBlock != NULL_ptr )
    {
        uint32_t remainingLength = pSuitableBlock->length - size;
        // Case[1-1]: If the free block is larger than the desired length[size]
        //  and the remaining Length is large than meta data
        if ( pSuitableBlock->length > size && remainingLength > sizeof(metaData_t))
        {
            pReturnAddress = FreeList_SplitBlock(&list, pSuitableBlock, size);
        }
        // Case[1-2]: If the free block is larger than the desired length[size]
        //  but the remaining Length is less than meta data
        else if ( pSuitableBlock->length > size )
        {
            // Update the desired size to the founded free block size
            size = pSuitableBlock->length;
            pReturnAddress = (void*)pSuitableBlock;
            // Remove the free block from the free list
            kErrorState &= FreeList_DeleteBlock(&list, pSuitableBlock);
        }
        // Case[1-3]: If the free block length is equal to the desired length[size]
        else if ( pSuitableBlock->length == size)
        {
            // Remove the free block from the free list
            kErrorState &= FreeList_DeleteBlock(&list, pSuitableBlock);
            pReturnAddress = (void*)pSuitableBlock;
        }
    }
    // Case[2]: There is no suitable free block in the free list or it's empty
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
            mulFactor = helper_CustomCeil( (f32_t)totalSize / (SIZE_OF_PAGE * NUM_OF_PAGES));
        }

        // Case[2-1]: First malloc call
        if ( pMyHeapTop == NULL_ptr )
        {
            // Extend the heap by increasing the program break
            pMyHeapTop = sbrk(SIZE_OF_PAGE * NUM_OF_PAGES * mulFactor);
            if ( pMyHeapTop != SBRK_ERROR)
            {
                FreeList_Init(&list);
                // Update the current program break
                pCurProgBreak = GetProgBreak();
            }else
            {
                kErrorState = kError;
            }
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
            // Update the current program break
            pCurProgBreak = GetProgBreak();
        }
    }

    // Case No error occurred either by finding a free block or by extending the heap
    if ( kErrorState != kError)
    {
        // Check if there wasn't a free block in the free list
        if ( pSuitableBlock == NULL_ptr )
        {
            // Assign the return address to the top of my heap
            pReturnAddress = pMyHeapTop;
            // update the top of the heap
            pMyHeapTop = (void*)((char*)pMyHeapTop + totalSize);
        }
        ((metaData_t*)pReturnAddress)->length = size;
        ((metaData_t*)pReturnAddress)->signature = ALLOCATED_SIGNATURE;
        // Increment the returned address by the size of the meta data block
        // cast it first to (char *) to make the step size of the ptr 1 byte
        pReturnAddress = (void*)((char *)pReturnAddress+sizeof(metaData_t));
    }

    return pReturnAddress;
}


void *calloc(uint32_t nmemb, uint32_t size)
{
    void *pReturnAddress = NULL_ptr;
    if ( nmemb != 0 && size !=0)
    {
        pReturnAddress =  malloc(nmemb * size);
        // Fill the allocated block with zeros
        pReturnAddress = memset(pReturnAddress, 0, size);
    }
    return pReturnAddress;
}

void *realloc(void *ptr, uint32_t size)
{
    
    void *pReturnAddress = ptr;
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
                    // Update the meta data
                    FreeList_InsertBlock(&list, pblock);
                    // Check if The next block is a free block
                    if ( FreeList_IsContigious(&list,pblock, pblock->pNext) == kTrue)
                    {
                        // Merge these two blocks of memory
                        FreeList_MergeBlocks(&list, pblock, pblock->pNext);
                    }
                    pMetaData->length = size;
                }
            }
            // Case[3-1-2] : if size > old block size
            else if ( size > pMetaData->length)
            {
                // Allocate a block with the new size
                pReturnAddress = malloc(size);
                if ( pReturnAddress != NULL_ptr)
                {
                   memcpy(pReturnAddress,ptr,pMetaData->length);
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
            kErrorState = helper_LowerProgramBreak(&list);
            if ( kErrorState == kError)
            {
               assert(0&&"Cannot lower program break\n");
            }
        }else
        {
            assert(0&&"[free]Invalid Pointer");
        }
    }
}