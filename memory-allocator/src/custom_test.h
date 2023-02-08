#ifndef MEMORY_ALLOCATOR_TEST_H
#define MEMORY_ALLOCATOR_TEST_H

#include "mem.h"
#include "mem_internals.h"
#include "util.h"
#include <sys/mman.h>

int simple_memory_allocation_test();

int freeing_one_of_several_test();

int freeing_two_of_several_test();

int heap_growth_test();

int heap_not_continuous_growth_test();

#endif
