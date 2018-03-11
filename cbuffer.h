#ifndef __CBUFFER_H
# define __CBUFFER_H
#ifdef __KERNEL__
# include <linux/types.h>
#else
# define u8 unsigned char
# include <assert.h>
#include <stdlib.h>
#include <string.h>
#endif

// Custom types.
#define bool int 
#define true 1 
#define false 0

struct cbuffer {
    u8 *buffer;
    u8 *head;
    u8 *tail;
    size_t size;
};

/**
 * Allocate a new cbuffer on the heap of the given size.
 * Must be deallocated with cbuffer_free(cbuffer*);
 * 
 * @param size Number of elements the ring buffer can contain.
 * @returns cbuffer* NULL if not enough memory or corrupt memory error. 
 */
struct cbuffer* cbuffer_create(const size_t size);

/**
 * Allocate new cbuffer on the kernel heap of the given size.
 * Must be deallocated with cbuffer_kfree(cbuffer*); 
 * 
 * @param size Number of elements the ring buffer can contain.
 * @returns cbuffer* NULL if not enough memory or corrupt memory error.
 */
struct cbuffer* cbuffer_kcreate(const size_t size);

/**
 * Reset the buffer. 
 * For performance, it will not deallocate the existing memory, only 
 * resets head and tails.
 * @param buffer Existing buffer.
 */
void cbuffer_flush(struct cbuffer * const buffer);

/**
 * Frees the cbuffer given as parameter. 
 * @param buffer cbuffer allocated on normal heap.
 */
void cbuffer_free(struct cbuffer * const buffer);

/**
 * Frees the cbuffer created with cbuffer_kcreate given as parameter. 
 * @param buffer cbuffer allocated on kernel heap
 */
void cbuffer_kfree(struct cbuffer * const buffer);

/**
 * Determines if the buffer is full.
 * @param buffer Cbuffer structure.
 * @returns true if full.
 */
bool cbuffer_full(const struct cbuffer * const buffer);

/**
 * Determines if the buffer is empty. 
 * @param buffer cbuffer structure.
 * @returns 1 if empty.
 */
bool cbuffer_empty(const struct cbuffer * const buffer);

/**
 * Inserts an element in the ring buffer. 
 * @param buffer cbuffer structure. 
 * @param data Data to insert. 
 */
void cbuffer_push(struct cbuffer * const buffer, const u8 data);

/**
 * Take (read and remove) the oldest data in the ring buffer and returns it.
 * @param buffer cbuffer structure. 
 * @returns u8 Data 
 */
u8 cbuffer_pop(struct cbuffer * const buffer);

/**
 * Advance to next position (either from tail or head).
 * This function should be strictly used within the cbuffer_* functions, 
 * nowhere else, although there is no side effect.
 * 
 * This function contains no assert.
 * 
 * @param buffer cbuffer structure 
 * @param position Pass either head or tail and it will return next position.
 * @returns next position (just like a i+1).
 */
inline u8* _cbuffer_next(const struct cbuffer * const buffer, const u8 * const position);

#endif /* __CBUFFER_H */
