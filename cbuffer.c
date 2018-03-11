#ifdef __KERNEL__ 
# include <linux/slab.h>
#else
# define kmalloc(a,b) malloc(a)
# define kfree(a) free(a)
# define spin_lock(l)
# define spin_unlock(l)
# define spin_lock_irqsave(l, f)
# define spin_unlock_irqrestore(l, f)
# define ENOMEM 1
#endif

#include "cbuffer.h"

struct cbuffer* cbuffer_create(const size_t size) {
	if(size == 0) return NULL; 
	struct cbuffer *buffer;
        buffer = malloc(sizeof(*buffer));
	
	if(buffer == NULL) return NULL;
	
	buffer->buffer = malloc((size + 1) * sizeof(u8));
	
	if(buffer->buffer == NULL) {
		free(buffer);
		return NULL;
	}
	
	buffer->head = buffer->tail = buffer->buffer;
	buffer->size = size;
	return buffer;
}

struct cbuffer* cbuffer_kcreate(const size_t size) {
	if(size == 0) return NULL;
	
	struct cbuffer *buffer;
	buffer = kmalloc(sizeof(*buffer), GFP_KERNEL);
	
	if(buffer == NULL) return NULL;
	
	buffer->buffer = kmalloc((size + 1) * sizeof(u8), GFP_KERNEL);
	
	if(buffer->buffer == NULL) {
		kfree(buffer); 
		return NULL;
	}
	
	buffer->head = buffer->tail = buffer->buffer;
	buffer->size = size;
	return buffer;
}

void cbuffer_flush(struct cbuffer * const buffer) {
	assert(buffer != NULL);
	
	buffer->head = buffer->buffer;
	buffer->tail = buffer->buffer;
}

void cbuffer_free(struct cbuffer * const buffer) {
	assert(buffer != NULL);
	
	free(buffer->buffer);
	free(buffer);
}

void cbuffer_kfree(struct cbuffer * const buffer) {
	assert(buffer != NULL);
	
	kfree(buffer->buffer); 
	kfree(buffer);
}

bool cbuffer_full(const struct cbuffer * const buffer) {
	assert(buffer != NULL);
	
	u8 *next_head = _cbuffer_next(buffer, buffer->head);;

	return next_head == buffer->tail;
}

bool cbuffer_empty(const struct cbuffer * const buffer) {
	assert(buffer != NULL);
	
	return buffer->head == buffer->tail;
}

void cbuffer_push(struct cbuffer * const buffer, const u8 data) {
	assert(buffer != NULL);
	*buffer->head = data;
	buffer->head = _cbuffer_next(buffer, buffer->head);
	
	if(buffer->head == buffer->tail) {
		buffer->tail = _cbuffer_next(buffer, buffer->tail);
	}
}

u8 cbuffer_pop(struct cbuffer * const buffer) {
	assert(buffer != NULL);
	assert(cbuffer_empty(buffer) == false);

	u8 value = *buffer->tail; // todo asserts.
	buffer->tail = _cbuffer_next(buffer, buffer->tail);
	
	return value;
}

inline u8* _cbuffer_next(const struct cbuffer * const buffer, const u8 * const position) {
	return (buffer->buffer + ((position - buffer->buffer + 1) % (buffer->size + 1))); 
}
