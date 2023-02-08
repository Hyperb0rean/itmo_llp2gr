#define _DEFAULT_SOURCE

#include "custom_test.h"

#define INIT_HEAP(size) heap_init(capacity_from_size((block_size){size}).bytes)

int64_t some_number = 1488;
int64_t answer_number = 42;


int simple_memory_allocation_test(){
    printf("Test 1 : Testing simple memory allocation in heap ... \n");

    void *heap = INIT_HEAP(REGION_MIN_SIZE);

    if(!heap){
        perror("Test 1 failed: Heap wasn't initialized. ");
        return 1;
    }

    debug_heap(stdout,heap);

    void *data = _malloc(some_number);

    if(!data){
        perror("Test 1 failed: Memory wasn't allocated");
        return 1;
    }

    debug_heap(stdout,heap);

    _free(data);

    munmap(heap, size_from_capacity((block_capacity) {.bytes = REGION_MIN_SIZE}).bytes);

    printf("Test 1 passed succefully! \n");
    return 0;
}

int freeing_one_of_several_test() {

    printf("Test 2 : Testing freeing one of several allocated...\n");

    void *heap = INIT_HEAP(REGION_MIN_SIZE);

    debug_heap(stdout, heap);

    void *data1 = _malloc(some_number);
    void *data2 = _malloc(answer_number);

    if (!data1 || !data2) {
        perror("Test 2 failed: Memory wasn't allocated\n");
        return 1;
    }

    debug_heap(stdout, heap);

    _free(data1);

    
    debug_heap(stdout, heap);

    _free(data2);
    

    munmap(heap, size_from_capacity((block_capacity) {.bytes = REGION_MIN_SIZE}).bytes);

    printf("Test 2 passed successfully!\n");

    return 0;
}

int freeing_two_of_several_test() {

    printf("Test 3 : Testing freeing two of several allocated...\n");

    void *heap = INIT_HEAP(REGION_MIN_SIZE);

    debug_heap(stdout, heap);

    void *data1 = _malloc(answer_number);
    void *data2 = _malloc(some_number);
    void *data3 = _malloc(answer_number);


    if (!data1 || !data2 || !data3) {
        perror("Test 3 failed: Memory wasn't allocated\n");
        return 1;
    }

    debug_heap(stdout, heap);

    _free(data3);
    _free(data1);

    debug_heap(stdout, heap);

   

    _free(data2);

    munmap(heap, size_from_capacity((block_capacity) {.bytes = REGION_MIN_SIZE}).bytes);

    printf("Test 3 passed successfully!\n");

    return 0;
}

int heap_growth_test() {

    printf("Test 4 : Testing automatic growth of heap...\n");

    void *heap = INIT_HEAP(REGION_MIN_SIZE);

    debug_heap(stdout, heap);

    void *data = _malloc(REGION_MIN_SIZE * 3 / 2);


    if(!data){
        perror("Test 4 failed: Memory wasn't allocated");
        return 1;
    }

    debug_heap(stdout, heap);

    struct block_header *header = heap;

    if (data != HEAP_START + offsetof(struct block_header, contents) || header->capacity.bytes <= REGION_MIN_SIZE) {
	    perror("Test 4 failed: New region wasn't allocated properly\n");
	    return 1;
    }

    _free(data);
    munmap(heap, size_from_capacity((block_capacity) {.bytes = REGION_MIN_SIZE*2}).bytes);

    printf("Test 4 passed successfully!\n");

    return 0;
}


int heap_not_continuous_growth_test() {


    printf("Test 5 : Testing non continuous region allocation...\n");

    void *heap = INIT_HEAP(REGION_MIN_SIZE);

    debug_heap(stdout, heap);

    (void) mmap(heap + REGION_MIN_SIZE, REGION_MIN_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);

    void *data = _malloc(some_number*answer_number);

    if(!data){
        perror("Test 5 failed: Memory wasn't allocated");
        return 1;
    }

    debug_heap(stdout, heap);

    printf("Test 5 passed successfully!\n");

    return 0;

}


