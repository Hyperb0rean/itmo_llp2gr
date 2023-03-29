#include "../include/utils.h"


const double_t idm[] =  {0, 0, 0, 0, 1, 0, 0, 0, 0};
const double_t ed0m[] =  {1, 0, -1, 0, 0, 0, -1, 0, 1};
const double_t ed1m[] =  {0, 1, 0, 1, -4, 1, 0, 1, 0};
const double_t ed2m[] =  {-1., -1., -1., -1., 8., -1., -1., -1., -1.};
const double_t shrpm[] =  {0, -1, 0, -1, 5, -1, 0, -1, 0};
const double_t gb3m[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
const double_t embm[] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};


struct kernel create_kernel(uint64_t width,uint64_t height){
    struct kernel ker = {
            .data =  malloc(sizeof(double_t) * width * height),
            .width = width,
            .height = height
    };

    return ker;
}

void destroy_kernel (struct kernel* ker){
    if(!ker->data || !ker) return;
    free(ker->data);
    //free(ker);
}

static int int8_comparer_reversed( const int8_t* x, const int8_t* y ) {
    if (*x < *y) return 1;
    if (*x > *y) return -1;
    return 0;
}

int int8_void_comparer_reversed(const void* _x, const void* _y ) {
    return int8_comparer_reversed(_x, _y );
}