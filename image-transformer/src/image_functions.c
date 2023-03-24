#include "../include/image_functions.h"



static struct pixel get_pixel(const struct image* img, uint64_t x, uint64_t y);
static void set_pixel(struct image* img, uint64_t x, uint64_t y, struct pixel pixel);
static int int8_void_comparer_reversed(const void* _x, const void* _y );
static int int8_comparer_reversed( const int8_t* x, const int8_t* y );

struct image create_image(uint64_t width, uint64_t height) {
    struct image img = {
            .data =  malloc(sizeof(struct pixel) * width * height),
            .width = width,
            .height = height
    };

    return img;
}


//Image rotation on 90 degrees
struct image rotate( struct image* const img ){
    if(!img->data) return (struct image) {0};
    struct image rotated_img = create_image( img->height, img->width );
    for (uint64_t y = 0; y < rotated_img.height; y++) {
        for (uint64_t x = 0; x < rotated_img.width; x++) {
            set_pixel(&rotated_img,x,y,get_pixel(img,y,img->height-1-x));
        }
    }
    return rotated_img;
}

//Implementation of standard negative algorithm:
//each pixel set on 255 - pixel value
struct image negative(struct image* const img){
    if(!img->data) return (struct image) {0};
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


// Implementation of standard sepia algorithm:
// Transforming pixels with weights
struct image sepia(struct image* const img){
    if(!img->data) return (struct image) {0};
    struct image sepia_image = create_image( img->width, img->height );

    for (uint64_t y = 0; y < sepia_image.height; y++) {
        for (uint64_t x = 0; x < sepia_image.width; x++) {
            struct pixel p = get_pixel(img,x,y);

            uint16_t tr = (uint16_t)round((0.393*(double )p.r) + (0.769*(double )p.g) + (0.189*(double )p.b));
            uint16_t tg = (uint16_t)round((0.349*(double )p.r) + (0.686*(double )p.g) + (0.168*(double )p.b));
            uint16_t tb = (uint16_t)round((0.272*(double )p.r) + (0.534*(double )p.g) + (0.131*(double )p.b));



            set_pixel(&sepia_image,x,y,(struct pixel) {
                    .b = tb>255 ? 255 : tb,
                    .g = tg>255 ? 255 : tg,
                    .r = tr>255 ? 255 : tr
            });
        }
    }
    return sepia_image;
}


// Implementation of standard greyscale algorithm:
// Transforming pixels with weights
struct image greyscale(struct image* const img){
    if(!img->data) return (struct image) {0};
    struct image greyscale_image = create_image( img->width, img->height );

    for (uint64_t y = 0; y < greyscale_image.height; y++) {
        for (uint64_t x = 0; x < greyscale_image.width; x++) {
            struct pixel p = get_pixel(img,x,y);

            uint8_t grey = (uint8_t)round(0.299*(double)p.r + 0.587*(double)p.g + 0.114*(double)p.b);

            set_pixel(&greyscale_image,x,y,(struct pixel) {
                    .b = grey,
                    .g = grey,
                    .r = grey
            });
        }
    }
    return greyscale_image;
}

// Implementation of median algorithm with given radius:
// Each pixel set on median value of its neighbours
// WARNING: Making radius more than 4 might cause some artifacts or issues
struct image median(struct image* const img, const uint8_t radius){
    if(!img->data) return (struct image) {0};
    struct image median_image = create_image( img->width, img->height );

    for (uint64_t y = 0; y < median_image.height; y++) {
        for (uint64_t x = 0; x < median_image.width; x++) {

            uint64_t size = 4*radius*radius + 2*radius + 1;
            uint64_t count = 0;

            uint8_t arrayR[size];
            uint8_t arrayG[size];
            uint8_t arrayB[size];

            memset (arrayR, 0, sizeof(arrayR));
            memset (arrayG, 0, sizeof(arrayG));
            memset (arrayB, 0, sizeof(arrayB));


            for (int16_t my = (int16_t)-radius; my <= radius; ++my) {
                for (int16_t mx = (int16_t)-radius; mx <= radius; ++mx) {

                    uint16_t i = my+radius;
                    uint16_t j = mx+radius;

                    if(x+mx<0 || y+my<0 || x+mx > median_image.width || y+my > median_image.height) continue;

                    arrayR[i*(2*radius+1) + j] = get_pixel(img,x+mx,y+my).r;
                    arrayG[i*(2*radius+1) + j] = get_pixel(img,x+mx,y+my).g;
                    arrayB[i*(2*radius+1) + j] = get_pixel(img,x+mx,y+my).b;

                    count++;
                }
            }

            qsort(arrayR,size,sizeof (uint8_t), int8_void_comparer_reversed);
            qsort(arrayG,size,sizeof (uint8_t), int8_void_comparer_reversed);
            qsort(arrayB,size,sizeof (uint8_t), int8_void_comparer_reversed);


            set_pixel(&median_image,x,y,(struct pixel) {
                    .b = arrayB[count/2],
                    .g = arrayG[count/2],
                    .r = arrayR[count/2]
            });

        }
    }
    return median_image;
}

// Implementation of standard box blurring algorithm with given radius:
// Each pixel set on arithmetic mean of uts neighbours
// WARNING: Making radius more than 8 might cause some artifacts
struct image box_blur(struct image* const img, const uint8_t radius){
    if(!img->data) return (struct image) {0};
    struct image blur_image = create_image( img->width, img->height );

    for (uint64_t y = 0; y < blur_image.height; y++) {
        for (uint64_t x = 0; x < blur_image.width; x++) {

            uint16_t sumR = 0;
            uint16_t sumG = 0;
            uint16_t sumB = 0;

            uint64_t count = 0;

            for (int16_t my = (int16_t)-radius; my <= radius; ++my) {
                for (int16_t mx = (int16_t)-radius; mx <= radius; ++mx) {

                    if(x+mx<0 || y+my<0 || x+mx+1 == blur_image.width || y+my+1 == blur_image.height) continue;

                    sumR+= get_pixel(img,x+mx,y+my).r;
                    sumG+= get_pixel(img,x+mx,y+my).g;
                    sumB+= get_pixel(img,x+mx,y+my).b;

                    count++;
                }
            }


            set_pixel(&blur_image,x,y,(struct pixel) {
                    .b = sumB/count,
                    .g = sumG/count,
                    .r = sumR/count
            });

        }
    }
    return blur_image;
}


static struct pixel get_pixel(const struct image* img, uint64_t x, uint64_t y) {
    return img->data[y*img->width + x];
}

static void set_pixel(struct image* img, uint64_t x, uint64_t y, const struct pixel pixel) {
    img->data[y*img->width + x] = pixel;
}

void destroy_image (struct image* img){
    if(!img->data || !img) return;
    free(img->data);
    free(img);
}

static int int8_comparer_reversed( const int8_t* x, const int8_t* y ) {
    if (*x < *y) return 1;
    if (*x > *y) return -1;
    return 0;
}

static int int8_void_comparer_reversed(const void* _x, const void* _y ) {
    return int8_comparer_reversed(_x, _y );
}