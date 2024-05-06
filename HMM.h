/**
 * @file HMM.h
 * @author Mohamed Refat
 * @brief 
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HMM_H
#define HMM_H

/**
 * @brief Structure representing metadata for an allocated memory block.
 *        Contains length, signature, and alignment information.
 */
typedef struct
{
    uint32_t length;    /**< Length of the allocated memory block. */
    uint32_t signature; /**< Signature to verify the validity of the block. */
    uint32_t res[4];    /**< Used for alignment. */
} metaData_t;

/**
 * @brief Allocates memory of the specified size.
 *
 * @param size The size of memory to allocate.
 * @return A pointer to the allocated memory block.
 */
void *malloc(uint32_t size);

/**
 * @brief Reallocates memory for a previously allocated block.
 *
 * @param ptr  A pointer to the previously allocated memory block.
 * @param size The new size of memory to allocate.
 * @return A pointer to the reallocated memory block.
 */
void *realloc(void *ptr, uint32_t size);

/**
 * @brief Allocates memory for an array of elements, each of the specified size.
 *
 * @param nmemb The number of elements in the array.
 * @param size The size of each element.
 * @return A pointer to the allocated memory block.
 */
void *calloc(uint32_t nmemb, uint32_t size);

/**
 * @brief Frees the memory block pointed to by ptr.
 *
 * @param ptr A pointer to the memory block to free.
 */
void free(void *ptr);

 


#endif /* HMM_H */