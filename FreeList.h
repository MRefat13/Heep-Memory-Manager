/**
 * @file FreeList.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef FREE_LIST_H_
#define FREE_LIST_H_

/**
 * @struct block_t
 * @brief Structure representing a memory block in the heap.
 */
typedef struct block_t
{
    struct block_t *pPrevious; /**< Pointer to the previous block in the heap. */
    struct block_t *pNext;     /**< Pointer to the next block in the heap. */
    uint32_t length;           /**< Length of the memory block. */
} block_t;

/**
 * @struct freeList_t
 * @brief Structure representing the free list of memory blocks in the heap.
 */
typedef struct
{
    block_t *pHead;    /**< Pointer to the head block of the free list. */
    block_t *pTail;    /**< Pointer to the tail block of the free list. */
    uint16_t size;     /**< Number of blocks in the free list. */
} freeList_t;

/**
 * @brief Initializes a free list.
 *
 * @param pList Pointer to the free list to be initialized.
 */
void FreeList_Init(freeList_t *pList);

/**
 * @brief Inserts a block into the free list.
 *
 * @param pList   Pointer to the free list.
 * @param pBlock  Pointer to the block to be inserted.
 * @return        An error code indicating the success or failure of the insertion operation.
 */
error_t FreeList_InsertBlock(freeList_t *pList, block_t *pBlock);

/**
 * @brief Checks if two blocks are contiguous in memory.
 *
 * @param pList      Pointer to the head of the free list.
 * @param p1stBlock  Pointer to the first block.
 * @param p2ndBlock  Pointer to the second block.
 * @return           True if the blocks are contiguous, false otherwise.
 */
bool_t FreeList_IsContigious(freeList_t *pList, block_t *p1stBlock, block_t *p2ndBlock);

/**
 * @brief Merges two contiguous blocks of memory.
 *
 * @param pList      Pointer to the free list.
 * @param p1stBlock  Pointer to the first block.
 * @param p2ndBlock  Pointer to the second block.
 * @return           An error code indicating the success or failure of the merge operation.
 */
error_t FreeList_MergeBlocks(freeList_t *pList, block_t *p1stBlock, block_t *p2ndBlock);

/**
 * @brief Splits a block of memory.
 *
 * @param pList         Pointer to the free list.
 * @param pBlock        Pointer to the old block of memory.
 * @param splittedSize  Size of the splitted part from the block.
 * @return              Pointer to the old block of memory (pBlock). Null if the split operation failed.
 * @note                This function will fail if Ihe size of the splitted part is larger than the block size
 */
void *FreeList_SplitBlock(freeList_t *pList, block_t *pBlock, uint32_t splittedSize);

/**
 * @brief Deletes a block from the free list.
 *
 * @param pList    Pointer to the free list.
 * @param pBlock   Pointer to the block to be deleted.
 * @return         An error code indicating the success or failure of the deletion operation.
 */
error_t FreeList_DeleteBlock(freeList_t *pList, block_t *pBlock);


/**
 * @brief Finds a block in the free list with a suitable size.
 *
 * @param pList        Pointer to the free list.
 * @param blockLength  Desired block length.
 * @return             Pointer to the nearest block to the desired one. 
 * 						Null if the desired block is larger than all the blocks in the list.
 * @note  This function search in the free  by two techniques [ Best-Fet or First-Fit]
 * 			you can change the technique for the configuration fle
 */
block_t *FreeList_FindSuitableBlock(freeList_t *pList, uint32_t blockLength);

#endif /* FREE_LIST_H_ */