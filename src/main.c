// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"

void keyboard(registers_t regs)
{
	monitor_write("Keyboard struck!\n");
	inb(0x60);
}

int main(struct multiboot *mboot_ptr)
{
	int i;
	// All our initialisation calls will go in here.
	//asm("movl $0xDEADBABA, %eax");
	monitor_clear();
	init_descriptor_tables();
	init_isr_handlers();

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	asm volatile("sti");
	init_timer(50);

	register_interrupt_handler(IRQ1, &keyboard);

	return 0;
}
