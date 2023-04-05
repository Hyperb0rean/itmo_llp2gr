#ifndef MATHS_H
#define MATHS_H


#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct __attribute__((__packed__)) kernel {
    uint8_t width,height;
    double_t* data;
};


struct __attribute__((packed)) pixel {
    uint8_t b, g, r;
};

void basic_convolution(struct pixel* const input, struct pixel*  output, uint64_t width, uint64_t height, struct kernel*);

struct kernel create_kernel(uint64_t width,uint64_t height);

void destroy_kernel (struct kernel*);

#endif //MATHS_H

