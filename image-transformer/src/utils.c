#include "../include/utils.h"




static int int8_comparer_reversed( const int8_t* x, const int8_t* y ) {
    if (*x < *y) return 1;
    if (*x > *y) return -1;
    return 0;
}

int int8_void_comparer_reversed(const void* _x, const void* _y ) {
    return int8_comparer_reversed(_x, _y );
}

