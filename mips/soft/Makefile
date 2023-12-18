SOURCE = test.c

OS = $(shell lsb_release -si)
ifeq ($(OS), Ubuntu)
	CC = mips-linux-gnu-gcc
	LD = mips-linux-gnu-ld
	AS = mips-linux-gnu-as
	DUMP = mips-linux-gnu-objdump
	COPY = mips-linux-gnu-objcopy
else
	PREFIX = /home/md401/aa205875/usr/bin/
	CC = $(PREFIX)/mips-gcc
	LD = $(PREFIX)/mips-ld
	AS = $(PREFIX)/mips-as
	DUMP = $(PREFIX)/mips-objdump
	COPY = $(PREFIX)/mips-objcopy
endif

CFLAGS = -O0 -march=r2000 -msoft-float -fno-stack-protector -mno-abicalls -fno-pic -G 0 -mno-gpopt
LDFLAGS =

all:
	export LD_LIBRARY_PATH=/usr/mips-linux-gnu/lib/;\
	$(CC) $(CFLAGS) -c -S $(SOURCE) -o program.asm
	$(AS) -mno-pdr program.asm -o program.o
	# Extract data section to _data.o
	$(COPY) -O binary --only-section=.data program.o _data.o
	# Add the data section (_data.o) as backup section
	$(COPY) --add-section .backup=_data.o program.o program_edit.o
	# Link
	$(LD) -T mips.ld program_edit.o -o program.bin $(LDFLAGS)
	# Extract the linked data section to _data_linked.o
	$(COPY) -O binary --only-section=.data program.bin _data_linked.o
	# Remote old backup section
	$(COPY) --remove-section .backup program.bin program_bin_removed.o
	# Add the linked data section (_data_linked.o) as backup section
	$(COPY) --add-section .backup=_data_linked.o program_bin_removed.o program.bin
	# Disassemble
	$(DUMP) -D --disassemble-zeroes program.bin > program.dump
	@echo "*** Generating program.dat from program.dump ***"
	@cat program.dump | \
	grep --only-matching "^ *[0-9a-fA-F]\+:[^0-9a-fA-F]*[0-9a-fA-F ]\+" | \
	gawk 'BEGIN{last=0;} {curr=strtonum("0x"$$1); diff=(curr-last)/4; for(i=1; i<diff; i=i+1){print("00000000");} if(curr>=last){print $$2;last=curr;}}' > program.dat
	@wc -l program.dat | \
	gawk '{printf("*** Your program includes %d words (default RAM size is 16384 words) ***\n", $$1);}'
	@cat program.dat | awk '{print $0} END{for(i=0;i<16384-NR;i++){printf("00000000\n");}}' > program.tmp
	@mv program.tmp program.dat
clean:
	rm -f *.o *.dump *.asm *.bin *.dat
