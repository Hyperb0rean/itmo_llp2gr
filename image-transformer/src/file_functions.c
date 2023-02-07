

#include "../include/file_functions.h"


enum open_status open_file( FILE** const file, char* const path, char* const mode ){
    *file = fopen(path,mode);
    if(*file == NULL){
        return OPEN_ERROR;
    }
    else{
        return OPEN_OK;
    }
}

enum close_status close_file( FILE* const file ){
    int status = fclose ( file );
    if ( status == EOF ) {
        return CLOSE_ERROR;
    } else {
        return CLOSE_OK;
    }
}
