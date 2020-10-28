/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Malloc Source

The source includes:
- malloc
- alligned_alloc
- calloc
- realloc
- free

*/

#include <cstdlib>
#include "malloc.hpp"


extern char &__heap_start;
extern char &__heap_end;

freelist flp(static_cast<void*>(&__heap_start));

void* std::malloc(size_t size)
{

	return flp.alloc(size);

}

void std::free(void* ptr)
{
	return flp.dealloc(ptr);
}