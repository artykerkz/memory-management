
#include "fscMalloc.h"
#include <assert.h>
//#include <sys/mman.h>
#include <stdlib.h> /* for rand() */
#include <time.h> /* for the init of rand */
#include <stddef.h> /* for size_t */

/* student written memory test */
int memoryTestThree() {
    /* If you want to add a test, feel free to add it here.
     * Returning 0 = success
     * Returning non-zero = failure
     */
    return 0;
}
void* fscMemorySetup(memoryStructure* m, fscAllocationMethod am, size_t sizeInBytes) {
    /* Students Fill in */
    if (m == NULL)
    {
        return NULL;
    }
    m->head = NULL;
    m->base = NULL;
    m->totalSize = 0;

    if (am != FIRST_FIT_RETURN_FIRST ||
        sizeInBytes < sizeof(fsc_free_node_t)) {
        return NULL;
        }
        void* memory = malloc (sizeInBytes);
    if (memory == NULL)
    {
        return NULL;
    }
    m->base = memory;
    m->totalSize = sizeInBytes;
    m->head = (fsc_free_node_t*)memory;
    m->head->size = sizeInBytes - sizeof (fsc_free_node_t);
    m->head->next = NULL;
    return memory;

}

/* very clearly for FIRST_FIT_RETURN_FIRST */
void* fscMalloc(memoryStructure* m, size_t requestedSizeInBytes) {
    /* Students Fill in */
    if (m == NULL || m->head == NULL || requestedSizeInBytes == 0) {
        return NULL;
}
    if (requestedSizeInBytes > SIZE_MAX - sizeof (fsc_alloc_header_t))
    {
        return NULL;
    }
     size_t totalNeeded = requestedSizeInBytes + sizeof (fsc_alloc_header_t);

    fsc_free_node_t* current = m->head;
    fsc_free_node_t* previous = NULL;

    size_t available = 0;

    while (current != NULL) {
        available = current->size + sizeof(fsc_free_node_t);

        if (available >= totalNeeded) {
            break;
        }

        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return NULL;
    }
    size_t remaining = available - totalNeeded;
    if (remaining >= sizeof (fsc_free_node_t))
    {
        fsc_free_node_t* newNode = (fsc_free_node_t*)((char*)current + totalNeeded);

        newNode->size = remaining - sizeof (fsc_free_node_t);
        newNode->next = current->next;

        if (previous == NULL)
        {
            m->head = newNode;
            } else {
                previous->next = newNode;
        }
        } else {
            if (previous == NULL)
            {
                m->head = current->next;
            } else
            {
                previous->next = current->next;
            }
    }
    fsc_alloc_header_t* header = (fsc_alloc_header_t*)current;

    header->size = (remaining >= sizeof (fsc_free_node_t)) ? requestedSizeInBytes : available - sizeof (fsc_alloc_header_t);
    header->magic = 1234567;

    return (void*)(header + 1);
    }

void fscFree(memoryStructure* m, void * returnedMemory) {
    /* Students Fill in */
    if (m == NULL || returnedMemory == NULL) {
        return;
    }

    fsc_alloc_header_t* header =
        (fsc_alloc_header_t*)returnedMemory - 1;

    if (header->magic != 1234567) {
        return;
    }

    size_t blockSize = header->size + sizeof(fsc_alloc_header_t);

    if (blockSize < sizeof(fsc_free_node_t)) {
        return;
    }

    header->magic = 0;

    fsc_free_node_t* newNode = (fsc_free_node_t*)header;
    newNode->size = blockSize - sizeof(fsc_free_node_t);
    newNode->next = NULL;

    fsc_free_node_t* current = m->head;
    fsc_free_node_t* previous = NULL;

    while (current != NULL &&
           (char*)current < (char*)newNode) {
        previous = current;
        current = current->next;
           }

    newNode->next = current;

    if (previous == NULL) {
        m->head = newNode;
    } else {
        previous->next = newNode;
    }

    if (current != NULL &&
        (char*)newNode + sizeof(fsc_free_node_t) +
        newNode->size == (char*)current) {

        newNode->size += sizeof(fsc_free_node_t) +
                         current->size;
        newNode->next = current->next;
        }

    if (previous != NULL &&
        (char*)previous + sizeof(fsc_free_node_t) +
        previous->size == (char*)newNode) {

        previous->size += sizeof(fsc_free_node_t) +
                          newNode->size;
        previous->next = newNode->next;
        }
} // fscfree()

/* keep for students */
void printFreeList(FILE * out, fsc_free_node_t* head) {
    /* given a node, prints the list. */
    fsc_free_node_t* current = head;
    fprintf(out, "About to dump the free list:\n");
    while (0 != current) {
        fprintf(out,
                "Node address: %'u\t Node size (stored): %'u\t Node size (actual) %'u\t Node next:%'u\n",
                current,
                current->size,
                current->size + sizeof (fsc_free_node_t),
                current->next);
        current = current->next;
    }
}


void fscMemoryCleanup(memoryStructure* m) {
    if (m == NULL) {
        return;
    }

    if (m->base != NULL) {
        free(m->base);
    }

    m->base = NULL;
    m->head = NULL;
    m->totalSize = 0;
}