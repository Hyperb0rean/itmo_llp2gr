CFLAGS     = -O2 -Wall  -std=c17
LDFLAGS    = -lm
CC         = gcc
LD         = gcc
ASM        = nasm
ASMFLAGS   =-felf64
SOURCES	   =./src/*.c
INCLUDES   = ./include/*.h
TARGET = transform
OBJFILES = ./*.o

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

