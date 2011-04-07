SOURCES=boot.o main.o common.o monitor.o descriptor_tables.o gdt.o string.o interrupt.o isr.o timer.o keyboard.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -g -Wall
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
