#include "common.h"
#include "monitor.h"
#include "isr.h"
#include "timer.h"

void keyboard(registers_t regs)
{
	u8int scanCode;
	scanCode = inb(0x60);
	monitor_write("Scancode: ");
	monitor_write_hex(scanCode);
	monitor_put('\n');

	if(scanCode == 0x14)
	{
		if(is_registered(IRQ0))
			unregister_interrupt_handler(IRQ0);
		else
			register_interrupt_handler(IRQ0, &timer_callback);
	}
}
