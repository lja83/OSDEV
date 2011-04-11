SOURCES=boot.o gdt.o interrupt.o main.o common.o monitor.o descriptor_tables.o string.o isr.o timer.o keyboard.o bitset.o pmm.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -g
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
