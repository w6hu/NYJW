
# Makefile
# David Grant, 2004
# Irene Huang, 2011/01/04

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS=-Wall -m5307 -pipe -nostdlib
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
OBJDMP=m68k-elf-objdump
ASM=../shared/start.s
LDFLAGS = -T../shared/rtx.ld -Wl,-Map=main.map

# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.
main.s19: main.s
	$(CC) $(CFLAGS) $(LDFLAGS) -o main.bin $(ASM) main.s 
	$(OBJCPY) --output-format=srec main.bin main.s19
	$(OBJDMP) -xdC main.bin > main.lst
	chmod u+x main.s19

.PHONY: clean
clean:
	rm -f *.s19 *.bin *.lst *.map
