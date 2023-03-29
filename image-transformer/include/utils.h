#ifndef KERNEL_FUNCTIONS_H
#define KERNEL_FUNCTIONS_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define ID   (struct kernel) { .data = (void*)&idm, .height=3, .width=3 }
#define ED0  (struct kernel) { .data = (void*)&ed0m, .height=3, .width=3 }
#define ED1  (struct kernel) { .data = (void*)&ed1m, .height=3, .width=3 }
#define ED2  (struct kernel) { .data = (void*)&ed2m, .height=3, .width=3 }
#define SHRP (struct kernel) { .data = (void*)&shrpm, .height=3, .width=3 }
#define GB3  (struct kernel) { .data = (void*)&gb3m, .height=3, .width=3 }
#define EMB  (struct kernel) { .data = (void*)&embm, .height=3, .width=3 }


struct __attribute__((__packed__)) kernel {
    uint8_t width,height;
    double_t* data;
};


struct kernel create_kernel(uint64_t width,uint64_t height);

void destroy_kernel (struct kernel* ker);

int int8_void_comparer_reversed(const void* _x, const void* _y );

#endif //KERNEL_FUNCTIONS_H
