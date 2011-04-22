#include "common.h"
#include "monitor.h"
#include "isr.h"
#include "timer.h"
#include "pmm.h"

extern void heapTest();

int print_scancode = 0;

void keyboard(registers_t regs)
{
	u8int scanCode;
	scanCode = inb(0x60);

	if(scanCode == 0x14)
	{
		if(is_registered(IRQ0))
			unregister_interrupt_handler(IRQ0);
		else
			register_interrupt_handler(IRQ0, &timer_callback);
	}
	else if(scanCode == 0x19)
	{
		u32int *ptr = (u32int*)0xA0000000;
		u32int do_page_fault = *ptr;
	}
	else if (scanCode == 0x16)
	{
		asm volatile("int $0x05");
	}
	else if (scanCode == 0x23)
	{
		heapTest();
	}
	else if (scanCode == 0x1E)
	{
		ASSERT(1 == 0);
	}
	else if (scanCode == 0x32)
	{
		meminfo();
	}
	else if (scanCode == 0x33)
	{
		while(1)
		{
			kprint("0x");
			khex((u32int)palloc());
			kprint(" ");
		}
	}
	else if (scanCode == 0x34)
	{
		u32int *page1;
		u32int *page2;
		u32int *page3;
		u32int *page4;
		u32int *page5;

		kprint("Testing page allocation and deallocation.\n");

		page1 = palloc();
		kprint("Allocated 0x"); khex((u32int)page1); kprint("\n");
		page2 = palloc();
		kprint("Allocated 0x"); khex((u32int)page2); kprint("\n");
		page3 = palloc();
		kprint("Allocated 0x"); khex((u32int)page3); kprint("\n");
		page4 = palloc();
		kprint("Allocated 0x"); khex((u32int)page4); kprint("\n");

		kprint("Freeing 0x"); khex((u32int)page1); kprint("\n");
		pfree(page1);
		page1 = 0;
		kprint("Freeing 0x"); khex((u32int)page3); kprint("\n");
		pfree(page3);
		page3 = 0;

		page1 = palloc();
		kprint("Allocated 0x"); khex((u32int)page1); kprint("\n");
		page3 = palloc();
		kprint("Allocated 0x"); khex((u32int)page3); kprint("\n");

		page5 = palloc();
		kprint("Allocated 0x"); khex((u32int)page5); kprint("\n");
	}
	else if(scanCode == 0x3B)
	{
		if (print_scancode == 1)
		{
			print_scancode = 0;
			kprint("Scancodes OFF\n");
		}
		else if(print_scancode == 0)
		{
			print_scancode = 1;
			kprint("Sancodes ON\n");
		}
	}
	else if(print_scancode == 1)
	{
		kprint("Unhandled Scancode: 0x");
		khex(scanCode);
		monitor_put('\n');
	}
}
