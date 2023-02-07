#include <stdio.h>

#include "../include/bmp_image_functions.h"

int close_file_and_print_errors(FILE* file) {
    enum close_status file_close_status = close_file( file );

    switch (file_close_status) {
        case CLOSE_OK: fprintf(stdout,"Successful closing of file\n");
       	    return 0;
            break;
        case CLOSE_ERROR: fprintf(stderr,"Error occurred while closing file\n");
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

    enum open_status input_file_open_status = open_file( &in_file, argv[1], "r" );


    switch (input_file_open_status) {
        case OPEN_OK: fprintf(stdout,"Successful opening of input file\n");
            break;
        case OPEN_ERROR: fprintf(stderr,"Error occurred while opening input file\n");
            return 1;
        default:
            return -1;
    }
    
    
    enum open_status output_file_open_status = open_file( &out_file, argv[2], "w");

    switch (output_file_open_status) {
        case OPEN_OK: fprintf(stdout,"Successful opening of output file\n");
            break;
        case OPEN_ERROR: fprintf(stderr,"Error occurred while opening output file\n");
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
        case READ_OK: fprintf(stdout,"Success reading in file\n");
            break;
        case READ_INVALID_SIGNATURE:
            fprintf(stderr,"Error: BMP file invalid signature\n");
        case READ_INVALID_BITS:
            fprintf(stderr,"Error: BMP file invalid bits\n");
        case READ_INVALID_HEADER:
            fprintf(stderr,"Error: BMP file invalid header\n");
        case READ_ERROR:
            fprintf(stderr,"Error: unknown\n");
        default:
            
    }
    
    if(input_file_read_status != READ_OK) {
    	close_file_and_print_errors(in_file);
        close_file_and_print_errors(out_file);
        return 2;
    }
    
    
    // Close input file

    close_file_and_print_errors(in_file);


    // Rotating image struct
    struct image* rotated_img = malloc( sizeof( struct image ) );

    *rotated_img = rotate( img );
    //*rotated_img = negative(rotated_img);
    
    if(!rotated_img->data) {
    	fprintf(stderr,"Error: failed to rotate image\n");
    	close_file_and_print_errors(out_file);
    }

    fprintf(stdout,"Success rotated image\n");

    destroy_image(img);
    // Writing from image struct to BMP file

    enum write_status output_file_write_status = to_bmp( out_file, rotated_img );

    destroy_image(rotated_img);

    switch (output_file_write_status) {
        case WRITE_OK: fprintf(stdout,"Successful writing to output file\n");
            break;
        case WRITE_ERROR: fprintf(stderr,"Error occurred while writing to output file\n");
            return 3;
        default:
            return -1;
    }

    //Close output file
    
    close_file_and_print_errors(out_file);


    return 0;

}

