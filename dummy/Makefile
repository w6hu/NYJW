# Makefile Winter 2011
# by
# David Grant
# Thomas Reidemeister
# Irene Huang

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS= -Wall -m5307 -pipe -nostdlib -w
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

#RTX_OBJS = dbug.o memory.o init.o rtx.o main_rtx.o
RTX_OBJS = process.o init.o memory.o dbug.o kcd.o priority.o hotkeys.o crt.o processA.o processBC.o messaging.o rtx.o main_rtx.o iprocess.o timer.o
RTX_LOADER_OBJS = dbug.o rtx_loader.o
RTX_TEST_DUMMY_OBJS = dbug.o rtx_test_dummy.o

all: mdummy.s19 
#all: Gid_rtx.s19

# Note, GCC builds things in order, it's important to put yhe
# ASM first, so that it is located at the beginning of our program.

#memory.o : ../memory/memory.c ../memory/memory.h
# $(CC) $(CFLAGS) -c $(LDFLAGS_RTX_TEST) ../memory/memory.c -o memory.o

#init.o : ../init/init.c ../init/init.h
# $(CC) $(CFLAGS) -c ../init/init.c -o init.o

rtx.s19: trap_entry.s  keyboard_i_proc_entry.s timer_entry.s $(RTX_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS_RTX) -o rtx.bin $(ASM) trap_entry.s keyboard_i_proc_entry.s timer_entry.s $(RTX_OBJS) 
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

Gid_rtx.s19: mlight.s19 rtx.s19
	python merge.py Gid_rtx.s19 mlight.s19 rtx.s19
	chmod u+x Gid_rtx.s19	
	
.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	#mv mlight.s19 mlight_back	
	rm -f *.bin *.s19 *.o *.map *.lst	
	#mv mlight_back mlight.s19
