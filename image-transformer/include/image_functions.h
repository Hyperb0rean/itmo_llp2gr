
#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include "utils.h"


struct __attribute__((__packed__)) image {
    uint64_t width, height;
    struct pixel* data;
};

struct image image_convolution(struct image* , struct kernel*);

struct image rotate( struct image*  );

struct image negative( struct image* );

struct image sepia( struct  image* );

struct image greyscale( struct  image* );

struct image static_threshold(struct image*, uint8_t value);

struct image box_blur( struct  image* , uint8_t radius);

struct image gaussian_blur( struct  image*);

struct image edge_detection(struct image*);

struct image median(struct image* , uint8_t radius);

struct image create_image (uint64_t width, uint64_t height);

void destroy_image (struct image* );

#endif //IMAGE_FUNCTIONS_H
