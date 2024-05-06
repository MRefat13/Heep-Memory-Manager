/*
 * FreeList.c
 *
 *  Created on: Nov 3, 2022
 *      Author: Mohamed Refat
 */
#include <unistd.h>
#include <assert.h>
#include "STD_TYPES.h"
#include "FreeList.h"
#include "HMM_Config.h"

extern void *pMyHeapTop;
void FreeList_Init(freeList_t *pList)
{
	pList->pHead = NULL_ptr;
	pList->pTail = NULL_ptr;
	pList->size  = 0;
}

error_t FreeList_InsertBlock(freeList_t *pList, block_t *pBlock)
{
	error_t kErrorState = kNoError;
	if( pBlock != NULL_ptr)
	{
		// Case inserting the First block at the linked list
		if(pList->size == 0)
		{
			pList->pTail  = pBlock;
			pList->pHead  = pBlock;
			pBlock->pNext = NULL_ptr;
			pBlock->pPrevious = NULL_ptr;
		}
		else 
		{
			block_t *pTempBlock = pList->pHead;
			/* 				
				The point is to insert the block of the lowest address at lowest position 
					of the linked list
				Check if the address of the pBlock > address of  pTempBlock 
					--> this mean we have to check the next node
				Note: if the pTempBlock->pNext == Null --> this mean we reach the end of the list
			*/
			while( pTempBlock->pNext != NULL_ptr)
			{
				if ( pBlock > pTempBlock )
				{
					pTempBlock = pTempBlock->pNext;
				}else if (pBlock < pTempBlock)
				{
					break;
				}
			}
			if ( pBlock < pTempBlock)
			{
				// Case the pTempBlock was the first block at the list
				if ( pTempBlock->pPrevious == NULL_ptr)
				{
					// Update the head
					pList->pHead = pBlock;
				}
				else
				{
					// update next ptr of the prev block
					pTempBlock->pPrevious->pNext = pBlock;
				}
				// Inserting the pBlock before the pTempBlock
				pBlock->pNext = pTempBlock;
				pBlock->pPrevious = pTempBlock->pPrevious;
				pTempBlock->pPrevious = pBlock;
			}
			// Case the pTempBlock was the last block at the linked list
			else
			{
				if ( pTempBlock->pNext == NULL_ptr)
				{
					// Update the tail
					pList->pTail = pBlock;
				}
				// Insert the block after the existing block
				pBlock->pNext = pTempBlock->pNext;
				pBlock->pPrevious = pTempBlock;
				pTempBlock->pNext = pBlock;
			}
		}
		pList->size++;
	}else
	{
		kErrorState =  kError;
	}
	return kErrorState;
}

error_t FreeList_DeleteBlock(freeList_t *pList, block_t *pBlock)
{
	error_t kErrorState = kNoError;
	if ( pList != NULL_ptr && pBlock != NULL_ptr && pList->size != 0)
	{
		// Case there is only one Block in the linked list
		if ( pList->size == 1)
		{
			pList->pHead = NULL_ptr;
			pList->pTail = NULL_ptr;
		}
		// Case the deleted block is the last block at the list
		else if ( pBlock->pNext == NULL_ptr )
		{
			pList->pTail = pBlock->pPrevious;
			pList->pTail->pNext = NULL_ptr;
		}
		// Case the deleted block is the first block at the linked list
		else if (pBlock->pPrevious == NULL_ptr)
		{
			pList->pHead = pBlock->pNext;
			pList->pHead->pPrevious = NULL_ptr;
		}
		// Case the deleted block is not the first or the last node
		else
		{
			pBlock->pNext->pPrevious = pBlock->pPrevious;
			pBlock->pPrevious->pNext = pBlock->pNext;
		}
		pList->size--;
	}else
	{
		kErrorState = kError;
	}
	return kErrorState;
}

bool_t FreeList_IsContigious(freeList_t *pList, block_t *p1stBlock, block_t *p2ndBlock)
{
	bool_t kBoll = kFalse;
	if ( pList != NULL_ptr && p1stBlock != NULL_ptr && p2ndBlock != NULL_ptr)
	{
		if ( p2ndBlock == (block_t*) ( (char *)p1stBlock + sizeof(block_t) + p1stBlock->length) )
		{
			kBoll = kTrue;
		}
	}
	return kBoll;
}


error_t FreeList_MergeBlocks(freeList_t *pList, block_t *p1stBlock, block_t *p2ndBlock)
{
	error_t kErrorState = kNoError;
	if ( pList != NULL_ptr && p1stBlock != NULL_ptr && p2ndBlock != NULL_ptr)
	{
		p1stBlock->length = p1stBlock->length + p2ndBlock->length + sizeof(block_t);
		kErrorState = FreeList_DeleteBlock(pList, p2ndBlock);
	}
	return kErrorState;
}
 

void *FreeList_SplitBlock(freeList_t *pList, block_t *pBlock, uint32_t splittedSize)
{
	void *pReturnAddress = NULL_ptr;
	error_t kErrorState = kNoError;
	if ( pList != NULL_ptr && pBlock != NULL_ptr && 
		splittedSize < pBlock->length)
	{
		// Delete old block from the free list
		kErrorState &= FreeList_DeleteBlock(pList, pBlock);
		// Check if there is enough space to insert the meta data of the splitted block
		if ( pBlock->length - splittedSize > sizeof(block_t))
		{
			block_t *pNewBlock = ((block_t *) (  ((char*)pBlock)+sizeof(block_t) + splittedSize ) );
			pNewBlock->length = pBlock->length - splittedSize - sizeof(block_t);
			kErrorState &= FreeList_InsertBlock(pList, pNewBlock);
		}else
		{
			// There is no enough space to insert the meta data of the new block [RIP]
			// The remaining part of the oroginal block will be fragment
		}
		pReturnAddress = (void *)pBlock;
	}
	if( kErrorState == kError )
	{
		pReturnAddress = NULL_ptr;
	}
	return pReturnAddress;
}

block_t *FreeList_FindSuitableBlock(freeList_t *pList, uint32_t blockLength)
{
	block_t *pSuitableBlock = NULL_ptr;
	if ( pList != NULL_ptr )
	{
		/* Traverse the list  to find Suitable Block which
			its size must my grater than of equal 
		*/
		block_t *pIteratorBlock = pList->pHead;
		#if SEARCHING_ALGORITHM == FIRST_FIT
		while (pIteratorBlock != NULL_ptr)
		{
			if (pIteratorBlock->length >= blockLength)
			{
				// Case a block with an equal or grater size is founded
				pSuitableBlock = pIteratorBlock;
				break;
			}
			pIteratorBlock = pIteratorBlock->pNext;
		}
		#elif SEARCHING_ALGORITHM == BEST_FIT
		/*
			TODO: Add implementation of best fit 
		*/
		#endif
	}
	return pSuitableBlock;
}
