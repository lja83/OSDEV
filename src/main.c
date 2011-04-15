// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "multiboot.h"
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "keyboard.h"
#include "bitset.h"
#include "pmm.h"
#include "vmm.h"

extern u32int kstart;
extern u32int ebss;
extern u32int stack;
extern u32int stack_begin;

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
	int i = 0;

	init_descriptor_tables();

	init_isr_handlers();
	monitor_write("ISR Handlers Initialised.\n");
	register_interrupt_handler(14, &page_fault_handler);

	monitor_clear();
	monitor_put('\n');			// Reserve the top row for the clock ticks
	mboot_ptr = mboot_info;

	init_timer(50);

	if(mboot_ptr->flags & 0x1)
	{
		kprint("Lower Memory:");
		padprint(mboot_ptr->mem_lower, 10, 10, ' ');
		kprint(" K\n");

		kprint("Upper Memory:");
		padprint(mboot_ptr->mem_upper, 10, 10, ' ');
		kprint(" K\n");

		u32int mmap_len = mboot_ptr->mmap_length;
		u32int start_addr = mboot_ptr->mmap_addr;

		multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)start_addr;
		memory_region_t *regions = (memory_region_t*)&ebss;
		i = 0;
		while((u32int)mmap < start_addr + mmap_len)
		{
			if(mmap->type == 1)
			{
				regions[i].start_addr = (u32int *)mmap->addr;
				regions[i].length = mmap->len;
				i++;
			}
			mmap++;
		}
		regions[i].start_addr = 0;
		regions[i].length = 0;
		kprint("Stack: 0x"); khex((u32int)&stack); kprint("\n");
		kprint("Stack Begin: 0x"); khex((u32int)&stack_begin); kprint("\n");
		kprint("EBSS: 0x"); khex((u32int)&ebss); kprint("\n");
		kprint("Start of regions: 0x"); khex((u32int)regions); kprint("\n");
		kprint("End of regions: 0x"); khex((u32int)&regions[i]); kprint("\n");
		kprint("One plus end of regions: 0x"); khex((u32int)&regions[i+1]); kprint("\n");
		init_pmm((u32int*)&regions[i+1], regions); // put the memory map directly after the regions array.
	}

	register_interrupt_handler(IRQ1, &keyboard);
	asm volatile("sti");

	kprint("Press T to toggle registration of the timer interrupt handler.\n");
	kprint("Press U to generate an unhandled interrupt.\n");
	kprint("Press P to generate a page fault.\n");
	kprint("Press A to cause an assertion failure\n");
	kprint("Press H to test the heap.\n");
	kprint("Press M for memory map.\n");
	kprint("Press F1 to toggle unhandled scancode printing.\n");

	return 0;
}
