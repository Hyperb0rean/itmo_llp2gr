CFLAGS     = -O2 -Wall  -std=c17
LDFLAGS    = -lm
CC         = gcc
LD         = gcc
ASM        = nasm
ASMFLAGS   =-felf64
SOURCES	   =./src/*.c
INCLUDES   = ./include/*.h
TARGET = transform
OBJFILES = bmp_image_functions.o file_functions.o image_functions.o utils.o main.o maths.o

all:
	make $(TARGET)
	make clean

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

$(OBJFILES): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) -c $@ $(SOURCES) $(LDFLAGS)

clean:
	$(RM) $(OBJFILES)

.PHONY: clean all

