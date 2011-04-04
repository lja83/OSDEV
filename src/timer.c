// timer.c -- Initialises the PIT and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "monitor.h"

u32int tick = 0;

static void timer_callback(registers_t regs)
{
	tick++;
	u8int *origLocation;
	int i;
	u8int origX;
	u8int origY;

	origLocation = monitor_get_cursor();
	origX = origLocation[0];
	origY = origLocation[1];

	monitor_move_cursor(0, 0);
	monitor_write("Tick: ");
	monitor_write_dec(tick);
	for(i = 0; i < 80 - monitor_get_cursor()[0]; i++)
	{
		monitor_put(' ');
	}
	monitor_move_cursor(origX, origY);
	//monitor_put('\n');
}

void init_timer(u32int frequency)
{
	monitor_write("Setting up the timer...\n");

	// Firstly, register our timer callback
	register_interrupt_handler(IRQ0, &timer_callback);

	// the value we send to the PIT is the value to divide its input clock
	// (1193180 Hz) by to get our required frequency. Important to note is
	// that the divisor must be small enough to fit into 16-bits.
	u32int divisor = 1193180 / frequency;

	// send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	u8int l = (u8int)(divisor & 0xFF);
	u8int h = (u8int)((divisor >> 8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
}
