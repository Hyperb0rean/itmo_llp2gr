#include "../include/maths.h"



const double_t idm[] =  {0, 0, 0, 0, 1, 0, 0, 0, 0};
const double_t ed0m[] =  {1, 0, -1, 0, 0, 0, -1, 0, 1};
const double_t ed1m[] =  {0, 1, 0, 1, -4, 1, 0, 1, 0};
const double_t ed2m[] =  {-1., -1., -1., -1., 8., -1., -1., -1., -1.};
const double_t shrpm[] =  {0, -1, 0, -1, 5, -1, 0, -1, 0};
const double_t gb3m[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
const double_t embm[] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};




void basic_convolution(struct pixel* const input, struct pixel*  output, uint64_t width, uint64_t height, struct kernel* const kernel){
    if(!input || !output) return;

    for (uint64_t y = 0; y < height; ++y) {
        for (uint64_t x = 0; x < width; ++x) {

            double_t sumR = 0,sumG = 0,sumB = 0, sumK =0;

            for (uint8_t i = 0; i <= kernel->width; ++i) {
                for (uint8_t j = 0; j <= kernel->height; ++j) {

                    int64_t my = y+(j-(kernel->height)/2);
                    int64_t mx = x+(i-(kernel->width)/2);

                    if(mx<0 || my < 0||
                       mx >= width || my >= height) continue;

                    sumR+= kernel->data[i*kernel->width +j]*input[my*width + mx].r;
                    sumG+= kernel->data[i*kernel->width +j]*input[my*width + mx].g;
                    sumB+= kernel->data[i*kernel->width +j]*input[my*width + mx].b;
                    sumK+=kernel->data[i*kernel->width +j];
                }
            }

            if(sumK<=0) sumK=1;
            sumR/=sumK;
            sumG/=sumK;
            sumB/=sumK;
            output[y*width + x] = (struct pixel) {
                    .b = (uint8_t) round(sumB<0 ? 0 : sumB>255 ? 255 : sumB),
                    .g = (uint8_t) round(sumG<0 ? 0 : sumG>255 ? 255 : sumG),
                    .r = (uint8_t) round(sumR<0 ? 0 : sumR>255 ? 255 : sumR)
            };
        }
    }
}


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