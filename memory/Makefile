
# Makefile
# David Grant, 2004
# Irene Huang, 2011/01/07

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS= -Wall -m5307 -pipe -nostdlib
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
ASM=./start.s
LDFLAGS = -Trtx.ld -Wl,-Map=main.map

OBJS=dbug.o memory.o main.o

# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.
main.s19: $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o main.bin $(ASM) $(OBJS) 
	$(OBJCPY) --output-format=srec main.bin main.s19

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) -c memory.c

dbug.o: dbug.c dbug.h
	$(CC) $(CFLAGS) -c dbug.c

.PHONY: clean
clean:
	rm -f *.s19 *.o *.bin *.map
