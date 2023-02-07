
#ifndef BMP_IMAGE_FUNCTIONS_H
#define BMP_IMAGE_FUNCTIONS_H


#include "bmp_image_header_format.h"
#include "image_functions.h"
#include "file_functions.h"

enum read_status  {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_ERROR
};

enum read_status from_bmp( FILE* in, struct image* img );

enum  write_status  {
    WRITE_OK = 0,
    WRITE_ERROR
};

enum write_status to_bmp( FILE* out, struct image const* img );



#endif //BMP_IMAGE_FUNCTIONS_H

