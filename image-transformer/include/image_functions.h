
#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H


#include <stdint.h>
#include <stdlib.h>

struct image {
    uint64_t width, height;
    struct pixel* data;
};

struct __attribute__((packed)) pixel {
    uint8_t b, g, r;
};

struct image create_image (uint64_t width, uint64_t height);

struct image rotate( struct image* img );

struct image negative( struct image* img);

struct pixel get_pixel(const struct image* img, uint64_t x, uint64_t y);

void set_pixel(struct image* img, uint64_t x, uint64_t y, struct pixel pixel);

void destroy_image (struct image* img);

#endif //IMAGE_FUNCTIONS_H

