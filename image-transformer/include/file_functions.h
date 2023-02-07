
#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H


enum open_status {
    OPEN_OK = 0,
    OPEN_ERROR
};

enum open_status open_file( FILE** file, char* path, char* mode );

enum close_status {
    CLOSE_OK = 0,
    CLOSE_ERROR
};

enum close_status close_file( FILE* file );

#endif //FILE_FUNCTIONS_H
