# Makefile Winter 2011 
# by
# David Grant
# Thomas Reidemeister
# Irene Huang

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS= -Wall -m5307 -pipe -nostdlib
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
OBJDUMP=m68k-elf-objdump
ASM=./start.s
LDFLAGS_RTX = -Trtx.ld -Wl,-Map=rtx.map
LDFLAGS_RTX_LOADER = -Trtx_loader.ld -Wl,-Map=rtx_loader.map
LDFLAGS_RTX_TEST = -Trtx_test.ld -Wl,-Map=rtx_test.map

RTX_OBJS = rtx.o dbug.o main_rtx.o
RTX_LOADER_OBJS = dbug.o rtx_loader.o
RTX_TEST_DUMMY_OBJS = dbug.o rtx_test_dummy.o

all: mdummy.s19 


# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.

rtx.s19: $(RTX_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS_RTX) -o rtx.bin $(ASM) $(RTX_OBJS) 
	$(OBJCPY) --output-format=srec rtx.bin rtx.s19
	$(OBJDUMP) -xdC rtx.bin > rtx.lst

rtx_loader.s19: $(RTX_LOADER_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS_RTX_LOADER) -o rtx_loader.bin $(ASM) $(RTX_LOADER_OBJS) 
	$(OBJCPY) --output-format=srec rtx_loader.bin rtx_loader.s19
	$(OBJDUMP) -xdC rtx_loader.bin > rtx_loader.lst

rtx_test_dummy.s19: $(RTX_TEST_DUMMY_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS_RTX_TEST) -o rtx_test_dummy.bin $(ASM) $(RTX_TEST_DUMMY_OBJS) 
	$(OBJCPY) --output-format=srec rtx_test_dummy.bin rtx_test_dummy.s19
	$(OBJDUMP) -xdC rtx_test_dummy.bin > rtx_test_dummy.lst

m2.s19: rtx_test_dummy.s19 rtx.s19
	python merge.py m2.s19 rtx.s19 rtx_test_dummy.s19 
	chmod u+x m2.s19

mdummy.s19: m2.s19 rtx_loader.s19
	python merge.py mdummy.s19 m2.s19 rtx_loader.s19 
	chmod u+x mdummy.s19

.c.o:
	$(CC) $(CFLAGS) -c $< 

.PHONY: clean
clean:
	rm -f *.bin *.s19 *.o *.map *.lst
