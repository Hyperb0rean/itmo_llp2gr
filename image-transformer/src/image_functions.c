
#include "../include/image_functions.h"

struct image create_image(uint64_t width, uint64_t height) {
    struct image img = {
            .data =  malloc(sizeof(struct pixel) * width * height),
            .width = width,
            .height = height
    };

    return img;
}

struct image rotate( struct image* const img ){
    if(!img) return NULL;
    struct image rotated_img = create_image( img->height, img->width );
    for (uint64_t y = 0; y < rotated_img.height; y++) {
        for (uint64_t x = 0; x < rotated_img.width; x++) {
            set_pixel(&rotated_img,x,y,get_pixel(img,y,img->height-1-x));
        }
    }
    return rotated_img;
}

struct image negative(struct image* const img){
    if(!img) return NULL;
    struct image negatived_img = create_image( img->width, img->height );
    for (uint64_t y = 0; y < negatived_img.height; y++) {
        for (uint64_t x = 0; x < negatived_img.width; x++) {
            struct pixel p = get_pixel(img,x,y);
            
            set_pixel(&negatived_img,x,y,(struct pixel) {
            	.b = 255 - p.b,
            	.g = 255 - p.g,
            	.r = 255 - p.r
            	});
        }
    }
    return negatived_img;
}

 struct pixel get_pixel(const struct image* img, uint64_t x, uint64_t y) {
    return img->data[y*img->width + x];
}

void set_pixel(struct image* img, uint64_t x, uint64_t y, const struct pixel pixel) {
    img->data[y*img->width + x] = pixel;
}

void destroy_image (struct image* img){
    if(!img) return;
    free(img->data);
}

