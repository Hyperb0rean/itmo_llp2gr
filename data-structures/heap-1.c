/* heap-1.c */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_BLOCKS 16
#define BLOCK_CAPACITY 1024

enum block_status { BLK_FREE = 0, BLK_ONE, BLK_FIRST, BLK_CONT, BLK_LAST };

struct heap {
    struct block {
        char contents[BLOCK_CAPACITY];
    } blocks[HEAP_BLOCKS];
    enum block_status status[HEAP_BLOCKS];
} global_heap = {0};

struct block_id {
    size_t       value;
    bool         valid;
    struct heap* heap;
};

struct block_id block_id_new(size_t value, struct heap* from) {
    return (struct block_id){.valid = true, .value = value, .heap = from};
}
struct block_id block_id_invalid() {
    return (struct block_id){.valid = false};
}

bool block_id_is_valid(struct block_id bid) {
    return bid.valid && bid.value < HEAP_BLOCKS;
}

/* Find block */

bool block_is_free(struct block_id bid) {
    if (!block_id_is_valid(bid))
        return false;
    return bid.heap->status[bid.value] == BLK_FREE;
}

/* Allocate */
struct block_id block_allocate(struct heap* heap, size_t size) {

    for(size_t i = 0;i<HEAP_BLOCKS;i++){
        size_t index = i;
        while(heap->status[i] == BLK_FREE){
            i++;
        }
        if((i-index)*BLOCK_CAPACITY < size) continue;
        else{
            if(size == 1*BLOCK_CAPACITY ){
                heap->status[index] = BLK_ONE;

            } else{
                heap->status[index] = BLK_FIRST;
                size_t j = 1;
                for(; j*BLOCK_CAPACITY < size; ++j){
                    heap->status[index+j] = BLK_CONT;
                }
                heap->status[index+j-1] = BLK_LAST;
            }

            return block_id_new(index,heap);
        }
    }
    return block_id_invalid();
}
/* Free */
bool block_free(struct block_id blockId) {
    if(!block_id_is_valid(blockId)) return false;
    if(blockId.heap->status[blockId.value] == BLK_ONE){
        blockId.heap->status[blockId.value] = BLK_FREE;
        return true;
    }
    if (blockId.heap->status[blockId.value] != BLK_FIRST) return false;
    size_t i = blockId.value;
    for(; blockId.heap->status[i] != BLK_LAST;i++){
        if(i>=HEAP_BLOCKS) return false;
        blockId.heap->status[i] = BLK_FREE;
    }
    blockId.heap->status[i] = BLK_FREE;
    return true;
}

/* Printer */
const char* block_repr(struct block_id b) {
    static const char* const repr[] = {[BLK_FREE] = " .",
            [BLK_ONE] = " *",
            [BLK_FIRST] = "[=",
            [BLK_LAST] = "=]",
            [BLK_CONT] = " ="};
    if (b.valid)
        return repr[b.heap->status[b.value]];
    else
        return "INVALID";
}

void block_debug_info(struct block_id b, FILE* f) {
    fprintf(f, "%s", block_repr(b));
}

void block_foreach_printer(struct heap* h, size_t count,
                           void printer(struct block_id, FILE* f), FILE* f) {
    for (size_t c = 0; c < count; c++)
        printer(block_id_new(c, h), f);
}

void heap_debug_info(struct heap* h, FILE* f) {
    block_foreach_printer(h, HEAP_BLOCKS, block_debug_info, f);
    fprintf(f, "\n");
}
/*  -------- */

int main() {
    heap_debug_info(&global_heap, stdout);
    struct block_id blockId = block_allocate(&global_heap,1024*2);
    struct block_id blockId2 = block_allocate(&global_heap,1024*2);
    struct block_id blockId23 = block_allocate(&global_heap,1024*3);
    block_free(blockId2);
    struct block_id blockId3 = block_allocate(&global_heap,1024*1);
    heap_debug_info(&global_heap, stdout);
    return 0;
}
