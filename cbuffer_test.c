#include "cbuffer.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	const size_t size = 5;
	cbuffer *buffer = cbuffer_create(size);
	
	// TEST initialize and pointers
	assert(buffer != NULL);
	assert(buffer->buffer != NULL);
	assert(buffer->head != NULL);
	assert(buffer->tail != NULL);
	
	// TEST default data size 
	assert(buffer->size == size);
	assert(cbuffer_empty(buffer) == true);
	assert(cbuffer_full(buffer) == false);
	
	// TEST insert 
	const u8 correct_insert_value = 8;
	cbuffer_push(buffer, correct_insert_value);
	assert(cbuffer_empty(buffer) == false);
	assert(cbuffer_full(buffer) == false);
	
	// TEST pop 
    u8 last_insert_value = cbuffer_pop(buffer); 
    assert(last_insert_value == correct_insert_value);
    assert(cbuffer_empty(buffer) == true); 
    assert(cbuffer_full(buffer) == false);
    
    // TEST full - nowrap
	size_t i = 0;
    for(i = 0; i < buffer->size; ++i) {
		cbuffer_push(buffer, (u8)i);
	}
	assert(cbuffer_empty(buffer) == false);
	assert(cbuffer_full(buffer) == true);
	
	// TEST - wrap. 
	cbuffer_push(buffer, 48);
	cbuffer_push(buffer, 50);
	assert(cbuffer_empty(buffer) == false);
	assert(cbuffer_full(buffer) == true); // will crash
	
	// TEST - pop (wrapped) 
	u8 third_value_entered = cbuffer_pop(buffer);
	assert(third_value_entered == 2); // 0..2 
	assert(cbuffer_empty(buffer) == false);
	assert(cbuffer_full(buffer) == false);
	
	// TEST - pop 'em all (let us get that 50) 
	u8 last_value;
	for(i = 0; i < size - 1; ++i) {
		last_value = cbuffer_pop(buffer);
	}
	assert(last_value == 50);
	assert(cbuffer_empty(buffer) == true);
	assert(cbuffer_full(buffer) == false);

	// TEST free (can only hope it doesn't crash)
	cbuffer_free(buffer);
	
	return 0;
}
