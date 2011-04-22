global loader							; make entry point visible to linker
global stack
global stack_begin
extern kmain							; kmain is defined elsewhere
extern init_vmm

;setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0					; align loaded modules on page boundaries
MEMINFO		equ  1<<1					; provide memory map
FLAGS		equ  MODULEALIGN | MEMINFO	; this is the Multiboot 'flag' field
MAGIC		equ  0x1BADB002				; 'magic number' lets bootloader find the header
CHECKSUM	equ  -(MAGIC + FLAGS)		; checksum required

;setting up paging
PAGING_PRESENT		equ 1<<0												; the mapped pages will be present
PAGING_WRITE		equ	1<<1												; allow reading/writing to pages/directories
PAGE_FLAGS			equ	PAGING_PRESENT | PAGING_WRITE						; attribute flags for pages and page directories
KERNEL_VIRTUAL_BASE equ 0xC0000000											; ultimate virtual location of kernel (as-linked)
KERNEL_PAGE_NUMBER 	equ (KERNEL_VIRTUAL_BASE / 0x400000)					; page number for relocated kernel = virtual base location / 4 meg

section .data
align 0x1000
kernel_page_dir:
	times 1024 dd 0															; reserve 4k for kernel page dir (1024 4-byte entries)
kernel_first_page_table:
	times 1024 dd 0															; reserve 4k for first page table (1024 4-byte entries)

section .text
align 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; reserve initial kernel stack space
STACKSIZE equ 0x4000					; that's 16k.

loader:
	xchg bx, bx
	; Create first page directory entries 1 for ID mapping and 1 for virtual mapping
	mov	ecx, (kernel_first_page_table - KERNEL_VIRTUAL_BASE)				; save physical memory addr of first page table
	mov edx, (kernel_page_dir - KERNEL_VIRTUAL_BASE)						; save physical memory addr of kernel page dir
	add	ecx, PAGE_FLAGS														; set attribute flags for the following entries, all entries are on 4k boundaries so we can simply add
	mov [edx+0], ecx														; id-map kernel
	mov [edx+(KERNEL_PAGE_NUMBER*4)], ecx									; also map kernel to higher half
	sub ecx, PAGE_FLAGS														; clean up attribute flags for upcoming loop, 4k boundaries again so simple subtraction is ok

	; Create first page table entries
	mov edx, PAGE_FLAGS														; set edx page present and page writeable bits, same as above
page_table_loop:															; each iteration creates one page table entry
	mov [ecx], edx															; store page table entry into current memory address
	add edx, 0x1000															; move to next page mapping
	add ecx, 4																; move to next page table entry
	cmp ecx, (kernel_first_page_table - KERNEL_VIRTUAL_BASE) + 0x1000		; check if we've exceeded the 4k space for a page table
	jb	page_table_loop														; if not, reiterate the loop

	; Turn on paging
	mov ecx, (kernel_page_dir - KERNEL_VIRTUAL_BASE)						; get physical kernel page directory location
	mov cr3, ecx															; point cr3 at page directory
	mov	ecx, cr0															; get current value of cr0
	or	ecx, 0x80000000														; flip paging bit
	mov cr0, ecx															; set cr0, turning on paging in the process

	; Jump to higher half
	lea ecx, [start_in_higher_half]											; get location of next label (in higher half)
	jmp ecx																	; jump to the higher half

start_in_higher_half:
	; Unmap lower half
	mov dword [kernel_page_dir], 0											; unmap identity-mapped kernel
	invlpg [0]																; invalidate id-mapped kernel page

	; Start kernel
	mov esp, stack_begin													; set up the stack
	push eax																; pass Multiboot magic number
	push ebx																; pass Multiboot info structure

	call kmain																; call kernel proper
hang:
	hlt																		; halt machine should kernel return
	jmp	hang

section .bss
align 4
stack:
	resb STACKSIZE															; reserve 16k stack on a doubleword boundary
stack_begin:
