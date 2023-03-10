// Including relevant libraries and modules
#include "kernel/types.h"
#include "user/user.h"
#include "memory_management.h"

// Head for the linked list
void *globalHead = 0;

// A function that iterates through the linked list to check if there's a free block that's large enough to use and returns it
memHead* getBlock(memHead **previous, int size) {
    memHead *current = globalHead;  // The linked list is initialised using globalHead

    while (previous && !(current->free && current->space >= size)) {    // While the block is free and is larger than the requested size
        *previous = current;
        current = current->nextSpace;
    }

    return current;
}

// A function used to request space from the OS and add this new space / block at the end of the linked list
memHead* getSpace(memHead* previous, int size) {
    memHead *memBlock;

    // Using sbrk() to allocate space from the OS
    memBlock = (memHead*)sbrk(0);
    void *requestSpace = sbrk(size + HEAD_SIZE);

    // Error checking
    if ((void*)memBlock != requestSpace) {
        return 0;
    }
    if (requestSpace == (void*) - 1) {
        return 0;
    }

    if (previous) {
        previous->nextSpace = memBlock;
    }

    // Initialising the structure variables
    memBlock->space = size;
    memBlock->nextSpace = 0;
    memBlock->free = 0;

    return memBlock;
}

// Implementation of malloc
void* _malloc(int size) {
    memHead *memBlock;

    // Returning unique void pointer if the requested size is 0
    if (size <= 0) {
        return (void*) 0;
    }

    // If globalHead is null, request space and set the pointer to the new block
    if (globalHead == 0) {
        memBlock = getSpace(0, size);
        if (memBlock == 0) {    // error checking
            return 0;
        }
        globalHead = memBlock;
        
    // If globalHead is not null, check to see if you can re-use space and if not, request new space.
    } else {
        memHead *previous = globalHead;
        memBlock = getBlock(&previous, size);
        if (memBlock == 0) {
            memBlock = getSpace(previous, size);
            if (memBlock == 0) {
                return 0;
            }

        } else {
            memBlock->free = 0;
        }
    }

    // Returning a void pointer. +1 is used to point to the region after the head (the block itself)
    return ((void*)(memBlock + 1));
}

// Implementation of free
void _free(void *ptr) {
    // No operation is performed if the pointer is null
    if (ptr <= 0) {
        return;
    }

    // Getting the address of the struct
    memHead* blockPtr = (memHead*)ptr - 1;

    // Error checking to ensure the block is not free
    if (blockPtr->free != 0) {
        return;
    }
    
    blockPtr->free = 1;     // 'frees' the block
}