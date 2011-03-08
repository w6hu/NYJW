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

dbug.o: dbug.c dbug.h
	$(CC) $(CFLAGS) -c dbug.c 

.PHONY: clean
clean:
	rm -f *.o
