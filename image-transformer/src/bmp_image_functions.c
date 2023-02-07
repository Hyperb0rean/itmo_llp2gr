
#include "../include/bmp_image_functions.h"


#define BM 0x4d42
#define RESERVED 0
#define OFFSET 54
#define HEADER_SIZE 40
#define PLANES 1
#define COMPRESSION 0
#define PPM 2834
#define BIT_COUNT 24
#define COLORS_USED 0
#define COLORS_IMPORTANT 0

static long padding_calc(uint32_t const width){
        return (width % 4 != 0) * (4 - ((long)(3 * width) % 4)) ;
}

enum read_status from_bmp( FILE* const in, struct image* const  img ){
    if(!in) return READ_ERROR;
    if(!img) return READ_INVALID_SIGNATURE;	

    struct bmp_header header;
    if(!fread(&header, sizeof(struct bmp_header), 1, in)) return READ_INVALID_HEADER;
    if(header.bfType != BM) return READ_INVALID_SIGNATURE;
    if(header.biBitCount != BIT_COUNT) return READ_INVALID_BITS;
    if(!img->data) return READ_INVALID_SIGNATURE;

    *img = create_image(header.biWidth,header.biHeight);

    long padding = padding_calc(header.biWidth);
    for ( uint32_t y = 0; y < img->height; y++ ) {
        if ( fread( img->data + img->width * y, sizeof(struct pixel), img->width, in) != img->width ) {
            destroy_image(img);
            return READ_ERROR;
        }
        if ( fseek( in, padding, SEEK_CUR ) ) {
            destroy_image(img);
            return READ_ERROR;
        }
    }
    return READ_OK;
}

enum write_status to_bmp( FILE* const out, struct image const* img ){

    if(!out) return WRITE_ERROR;
    if(!img) return WRITE_ERROR;	

    long padding = padding_calc(img->width);
    struct bmp_header header;
    header.bfType = BM;
    header.biSizeImage = (img->width*3 + padding) * img->height;
    header.bfileSize = header.biSizeImage + sizeof(struct bmp_header);
    header.bfReserved = RESERVED;
    header.bOffBits = OFFSET;
    header.biSize = HEADER_SIZE;
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biPlanes = PLANES;
    header.biBitCount = BIT_COUNT;
    header.biCompression = COMPRESSION;
    header.biXPelsPerMeter = PPM;
    header.biYPelsPerMeter = PPM;
    header.biClrUsed = COLORS_USED;
    header.biClrImportant = COLORS_IMPORTANT;

    if ( !fwrite( &header, sizeof(struct bmp_header), 1, out ) ) {
        return WRITE_ERROR;
    }

    struct pixel* garbage = img->data;

    for (uint64_t y = 0; y < img->height; y++) {
        if ( !fwrite( img->data + y*img->width, sizeof(struct pixel) * img->width, 1, out ) ) {
            return WRITE_ERROR;
        }
        if (  padding != 0 && !fwrite(  garbage, padding, 1, out )) {
            return WRITE_ERROR;
        }
    }

    return WRITE_OK;
}



