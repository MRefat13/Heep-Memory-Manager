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

typedef struct block_t
{
	struct block_t *pPrevious;
	struct block_t *pNext;
	uint32_t length;
}block_t;

typedef struct
{
	block_t *pHead;
	block_t *pTail;
	uint16_t size;
}freeList_t;

void FreeList_Init(freeList_t *pList);
error_t FreeList_InsertBlock(freeList_t *pList, block_t *pBlock);

/**
 * @brief This function is used to check if the two blocks are contigious in the memory 
 * 			or not  
 * 
 * @param pList pointer the the head of the list 
 * @param p1stBlock Pointer to the first block
 * @param p2ndBlock Pointer to the second block
 * @return bool_t 
 */
bool_t FreeList_IsContigious(freeList_t *pList, block_t *p1stBlock, block_t *p2ndBlock);

/**
 * @brief This function is used to merge two contigious blocks of memory
 * 			it adds the size of the first block to the size of the second block and
 * 			store the result in the first block then remove the second block from the list
 * 
 * @param pList 
 * @param p1stBlock Pointer to the first block
 * @param p2ndBlock Pointer to the second block
 * @note This function works only if the two blocks are contigious in the memory
 * @return error_t 
 */
error_t FreeList_MergeBlocks(freeList_t *pList, block_t *p1stBlock, block_t *p2ndBlock);

/**
 * @brief This function is used to split a block of memory
 * 
 * @param pList        : Pointer to the free List
 * @param pBlock       : Pointer to the old block of memory
 * @param splittedSize : Size of the splitted part from the block
 * 
 * @return void* : Pointer to the old block of memory[pBlock]
 * 			- The return will be null if it  failed to split due to some cases included:
 *             -If Ihe size of the splitted part is larger than the block size
 * 			   -If one of the parameter is Null
 */
void *FreeList_SplitBlock(freeList_t *pList, block_t *pBlock, uint32_t splittedSize);


error_t FreeList_DeleteBlock(freeList_t *pList, block_t *pBlock);

/**
 * @brief This function is used to find a block in the free list with a suitable size
 * 			it does this by searching in the free list and it uses a two techniques
 * 			[Fist-First and Best-Fet]  based in a macro in config file 
 * 
 * @param pList       : Pointer to the free List
 * @param blockLength : desired block length
 * @return block_t*   : pointer to the nearest block to the desired one
 * 				The return is null if desired block is larger than all the blocks in the list
 */
block_t *FreeList_FindSuitableBlock(freeList_t *pList, uint32_t blockLength);


error_t FreeList_LowerProgramBreak(freeList_t *pList);

void FreeList_traverse(freeList_t *pList, void (*pFun)(uint32_t blockLength));
error_t  FreeList_IsEmpty(freeList_t *pList);
error_t  FreeList_IsFull(freeList_t *pList);
uint32_t FreeList_GetSize(freeList_t *pList);
#endif /* FREE_LIST_H_ */