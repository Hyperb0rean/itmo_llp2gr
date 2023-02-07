ASM=nasm
ASMFLAGS=-f elf64
LD=ld

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

main.o: main.asm lib.inc words.inc colon.inc
	$(ASM) $(ASMFLAGS) -o $@ $<

dict.o: dict.asm lib.inc
	$(ASM) $(ASMFLAGS) -o $@ $<

main: main.o lib.o dict.o
	ld -o main $^
	make clean

.PHONY: clean
clean:
	$(RM) *.o
