# Heap Memory Manager
This project was developed as a part of training at STMicroelectronics. It served as a practical exercise to enhance understanding of memory management concepts and their implementation on Linux.

The Heap Memory Manager is a module that provides memory allocation and deallocation functions for managing dynamic memory on the heap. It includes functions such as `malloc`, `realloc`, `calloc`, and `free` to allocate, reallocate, and deallocate memory blocks.

## Usage

To use the Heap Memory Manager in your project, follow these steps:

1. Include the necessary header file(s) in your source file:

```c
#include "HMM.h"
```

2. Link the Heap Memory Manager module with your project. This may involve compiling and linking the source file(s) provided with the module.

3. Use the provided functions for memory allocation and deallocation:

### `void *malloc(uint32_t size)`

Allocates a block of memory of the specified size and returns a pointer to the allocated memory block.

Parameters:
- `size`: The size of memory to allocate.

Returns:
- A pointer to the allocated memory block. Returns `NULL` if the allocation fails.

### `void *realloc(void *ptr, uint32_t size)`

Reallocates memory for a previously allocated block. If the reallocation is successful, the contents of the original block are preserved and a pointer to the reallocated memory block is returned. If the reallocation fails, `NULL` is returned, and the original block is left untouched.

Parameters:
- `ptr`: A pointer to the previously allocated memory block.
- `size`: The new size of memory to allocate.

Returns:
- A pointer to the reallocated memory block. Returns `NULL` if the reallocation fails.

### `void *calloc(uint32_t nmemb, uint32_t size)`

Allocates memory for an array of elements, each of the specified size. The allocated memory is initialized to zero.

Parameters:
- `nmemb`: The number of elements in the array.
- `size`: The size of each element.

Returns:
- A pointer to the allocated memory block. Returns `NULL` if the allocation fails.

### `void free(void *ptr)`

Frees the memory block pointed to by `ptr`. If `ptr` is `NULL`, no action is performed.

Parameters:
- `ptr`: A pointer to the memory block to free.

## Example

Here's an example usage of the Heap Memory Manager:

```c
#include <stdio.h>
#include "heap_memory_manager.h"

int main()
{
    // Allocate memory for an integer
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr != NULL)
    {
        *ptr = 42;
        printf("Value: %d\n", *ptr);

        // Reallocate memory for a larger integer
        ptr = (int *)realloc(ptr, sizeof(int) * 2);
        if (ptr != NULL)
        {
            *ptr = 99;
            *(ptr + 1) = 88;
            printf("Values: %d, %d\n", *ptr, *(ptr + 1));

            // Free the allocated memory
            free(ptr);
        }
    }

    return 0;
}
```


## Acknowledgments

The Heap Memory Manager module is based on the functions and utilities provided by [your name or organization]. Many thanks for their contribution!

## Contact

For any questions or inquiries, please contact [mo7amedrefaat2001@gmail.com].

---

Feel free to customize this README file according to your specific needs and add any additional information or sections that are relevant to your heap memory manager module.