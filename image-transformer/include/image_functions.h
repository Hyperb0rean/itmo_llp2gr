
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

struct image create_image (uint64_t width, uint64_t height);

struct image rotate( struct image* img );

struct image negative( struct image* img);

struct image sepia( struct  image* img);

struct image greyscale( struct  image* img);

struct image box_blur( struct  image* img, uint8_t radius);

struct image median(struct image* img, uint8_t radius);

void destroy_image (struct image* img);

#endif //IMAGE_FUNCTIONS_H
