
#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



struct __attribute__((__packed__)) image {
    uint64_t width, height;
    struct pixel* data;
};

struct __attribute__((packed)) pixel {
    uint8_t b, g, r;
};

struct __attribute__((__packed__)) kernel {
    uint8_t width,height;
    double_t* data;
};

struct image create_image (uint64_t width, uint64_t height);

struct image rotate( struct image*  );

struct image convolution(struct image* , struct kernel*);

struct image negative( struct image* );

struct image sepia( struct  image* );

struct image greyscale( struct  image* );

struct image box_blur( struct  image* , uint8_t radius);

struct image median(struct image* , uint8_t radius);

void destroy_image (struct image* );

#endif //IMAGE_FUNCTIONS_H
