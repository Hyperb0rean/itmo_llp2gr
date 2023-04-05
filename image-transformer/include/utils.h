#ifndef UTILS_H
#define UTILS_H

#include "maths.h"


#define ID   (struct kernel) { .data = (void*)&idm, .height=3, .width=3 }
#define ED0  (struct kernel) { .data = (void*)&ed0m, .height=3, .width=3 }
#define ED1  (struct kernel) { .data = (void*)&ed1m, .height=3, .width=3 }
#define ED2  (struct kernel) { .data = (void*)&ed2m, .height=3, .width=3 }
#define SHRP (struct kernel) { .data = (void*)&shrpm, .height=3, .width=3 }
#define GB3  (struct kernel) { .data = (void*)&gb3m, .height=3, .width=3 }
#define EMB  (struct kernel) { .data = (void*)&embm, .height=3, .width=3 }

int int8_void_comparer_reversed(const void* _x, const void* _y );


#endif //UTILS_H
