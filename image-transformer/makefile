CFLAGS     = -g -O2 -Wall -Werror -std=c17 
CC         = gcc
LD         = gcc
ASM        = nasm
ASMFLAGS   =-felf64 -g
SOURCES	   =./src/main.c ./src/image_functions.c ./src/bmp_image_functions.c
INCLUDES   = ./include/image_functions.h ./include/bmp_image_functions.h ./include/bmp_image_header_format.h
TARGET =run
OBJFILES = main.o image_functions.o bmp_image_functions.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
	make clean

$(OBJFILES): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) -c $@ $(SOURCES) $(LDFLAGS)

clean:
	$(RM) $(OBJFILES) 
	
.PHONY: clean all

