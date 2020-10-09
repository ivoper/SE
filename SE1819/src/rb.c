/*
 * rb.c
 *
 *  Created on: 16/04/2019
 *      Author: Ivo Pereira
 */

#include "rb.h"

void RB_Reset(RINGBUFF_DSC *buffer){
	buffer->full = false;
	buffer->get = 0;
	buffer->put = 0;
}

bool RB_Full(RINGBUFF_DSC *rb){
	//return (buffer->get == (buffer->put + 1));
	return rb->full;
}

bool RB_Empty(RINGBUFF_DSC *rb){
return ((rb->get == rb->put) && !rb->full);
}

void RB_Push(RINGBUFF_DSC *rb, unsigned char val){
	rb->buffer[rb->put++] = val;
	if(rb->put == rb->get)
		rb->full = true;
}

unsigned char RB_Pop(RINGBUFF_DSC *rb){
	if(rb->full)
		rb->full = false;
	return rb->buffer[rb->get++];
}
