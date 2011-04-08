// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "multiboot.h"
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "keyboard.h"

extern u32int kstart;
extern u32int ebss;

void heapTest()
{
	kprint("Jeff should probably get off his ass and write a heap before you go testing it.\n");
}

void meminfo()
{
	if(mboot_ptr->flags & 0x1)
	{
		kprint("Lower Memory:");
		padprint(mboot_ptr->mem_lower, 10, 10, ' ');
		kprint(" K\n");

		kprint("Upper Memory:");
		padprint(mboot_ptr->mem_upper, 10, 10, ' ');
		kprint(" K\n");
		kprint("\n");
		if(mboot_ptr->flags & (0x1 << 6))
		{
			u32int mmap_len = mboot_ptr->mmap_length;
			u32int start_addr = mboot_ptr->mmap_addr;

			kprint("Memory map available.\n");
			multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)start_addr;

			kprint("/******************************************************\\\n");
			kprint("|    Begin         End         Length            Type  |\n");
			kprint("|------------------------------------------------------|\n");

			while((u32int)mmap < start_addr + mmap_len)
			{
				u32int begin = mmap->addr;
				u32int end = mmap->addr + mmap->len - 1;

				kprint("|  ");
				padprint(begin, 16, 8, '_');
				kprint("  |  ");
				padprint(end, 16, 8, '_');
				kprint("  |  ");
				padprint((u32int)mmap->len, 10, 10, '_');
				kprint(" B  |  ");
				kdec(mmap->type);
				switch(mmap->type)
				{
					case 1:
						kprint("  Avbl ");
						break;
					case 2:
						kprint("  Rsvd ");
						break;
					case 3:
						kprint("  Recl ");
						break;
					case 4:
						kprint(" ACPI N");
						break;
					kprint(    "  Undef");
				}
				kprint(" |\n");
				mmap += 1;
			}

			kprint("\\******************************************************/\n");
			kprint("Kernel starts at 0x"); khex((u32int)&kstart);
			kprint("\n");
			kprint("Kernel ends at   0x"); khex((u32int)&ebss);
			kprint("\n");
		}
	}
	else
	{
		kprint("No memory information available\n");
	}
}

int kmain(multiboot_info_t *mboot_info, unsigned int magic)
{
	monitor_clear();
	monitor_put('\n');			// Reserve the top row for the clock ticks
	mboot_ptr = mboot_info;

	if(mboot_ptr->flags & 0x1)
	{
		kprint("Lower Memory:");
		padprint(mboot_ptr->mem_lower, 10, 10, ' ');
		kprint(" K\n");

		kprint("Upper Memory:");
		padprint(mboot_ptr->mem_upper, 10, 10, ' ');
		kprint(" K\n");
	}

	init_descriptor_tables();
	init_timer(50);
	register_interrupt_handler(IRQ1, &keyboard);
	asm volatile("sti");

	kprint("\nWelcome to TUPAHM F1\n");
	kprint("Press T to toggle registration of the timer interrupt handler.\n");
	kprint("Press U to generate an unhandled interrupt.\n");
	kprint("Press P to generate a page fault.\n");
	kprint("Press A to cause an assertion failure\n");
	kprint("Press H to test the heap.\n");
	kprint("Press M for memory map.\n");
	kprint("Press F1 to toggle unhandled scancode printing.\n");

	return 0;
}
