// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"

void keyboard(registers_t regs)
{
	u8int scanCode;
	scanCode = inb(0x60);
	monitor_write("Scancode: ");
	monitor_write_hex(scanCode);
	monitor_put('\n');
}

int main(struct multiboot *mboot_ptr)
{
	int i;
	monitor_clear();
	monitor_put('\n');			// Reserve the top row for the clock ticks


	init_descriptor_tables();
	init_isr_handlers();
	init_timer(50);

	register_interrupt_handler(IRQ1, &keyboard);

	asm volatile("sti");

	return 0;
}
