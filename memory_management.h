// Including relevant libraries and modules
#include "kernel/types.h"
#include "user/user.h"

// Defining a struct (/linked list) to store meta information about each memory block
typedef struct memoryBlockHead {
    int space;                              // Size of the block
    struct memoryBlockHead *nextSpace;      // Pointer to the next block
    int free;                               // Indicates if the block is free or not
} memHead;

// Defining a term for the size of the structure
#define HEAD_SIZE sizeof(struct memoryBlockHead)

// Declaring function prototypes
void* _malloc(int size);
void _free(void *ptr);