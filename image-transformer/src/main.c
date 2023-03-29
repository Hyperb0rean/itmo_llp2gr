#include <stdio.h>

#include "../include/bmp_image_functions.h"

int close_file_and_print_errors(FILE* file) {
    enum close_status file_close_status = close_file( file );

    switch (file_close_status) {
        case CLOSE_OK: printf("Successful closing of file\n");
            return 0;
        case CLOSE_ERROR: perror("Error occurred while closing file\n");
            return 4;
        default:
            return -1;
    }
}


int main( int argc, char** argv ) {



    // Arguments checking
    if ( argc != 3 ) {
        fprintf(stderr,"Invalid number of arguments");
        return 5;
    }

    // File opening

    FILE* in_file = NULL;
    FILE* out_file = NULL;

    enum open_status input_file_open_status = open_file( &in_file, argv[1], "rb" );


    switch (input_file_open_status) {
        case OPEN_OK: printf("Successful opening of input file\n");
            break;
        case OPEN_ERROR: perror("Error occurred while opening input file\n");
            return 1;
        default:
            return -1;
    }


    enum open_status output_file_open_status = open_file( &out_file, argv[2], "wb");

    switch (output_file_open_status) {
        case OPEN_OK: printf("Successful opening of output file\n");
            break;
        case OPEN_ERROR: perror("Error occurred while opening output file\n");
            close_file_and_print_errors(in_file);
            return 1;
        default:
            close_file_and_print_errors(in_file);
            return -1;
    }

    // Reading from BMP file to image struct

    struct image *img = malloc( sizeof( struct image ) );

    enum read_status input_file_read_status = from_bmp( in_file, img );


    switch (input_file_read_status) {
        case READ_OK: printf("Success reading in file\n");
            break;
        case READ_INVALID_SIGNATURE:
            perror("Error: BMP file invalid signature\n");
            break;
        case READ_INVALID_BITS:
            perror("Error: BMP file invalid bits\n");
            break;
        case READ_INVALID_HEADER:
            perror("Error: BMP file invalid header\n");
            break;
        case READ_ERROR:
            perror("Error: unknown\n");
            break;
        default:
            break;

    }

    if(input_file_read_status != READ_OK) {
        destroy_image(img);
        close_file_and_print_errors(in_file);
        close_file_and_print_errors(out_file);
        return 2;
    }


    // Close input file

    close_file_and_print_errors(in_file);

    //Creating new image
    struct image* edited_img = malloc( sizeof( struct image ) );

    if(!img) {
        perror("Error: failed to edit image, image is null\n");
        destroy_image(img);
        destroy_image(edited_img);
        close_file_and_print_errors(out_file);
        return 5;
    }

    //Editing image


    *edited_img = box_blur(img,1);
    //*edited_img = median( edited_img,1);
    //*edited_img = static_treshold( edited_img,167);
    //*edited_img = edge_detection(edited_img);


    //

    if(!edited_img->data) {
        perror("Error: failed to rotate image, data is null\n");
        destroy_image(img);
        destroy_image(edited_img);
        close_file_and_print_errors(out_file);
        return 5;
    }

    printf("Successfully edited image\n");

    // Writing from image struct to BMP file

    enum write_status output_file_write_status = to_bmp( out_file, edited_img );

    destroy_image(img);
    destroy_image(edited_img);

    switch (output_file_write_status) {
        case WRITE_OK: printf("Successful writing to output file\n");
            break;
        case WRITE_ERROR: perror("Error occurred while writing to output file\n");
            close_file_and_print_errors(out_file);
            return 3;
        default:
            close_file_and_print_errors(out_file);
            return -1;
    }

    //Close output file

    close_file_and_print_errors(out_file);


    return 0;

}
