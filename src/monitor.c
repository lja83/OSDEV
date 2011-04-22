#include "monitor.h"
#include "common.h"

u16int *videoMemory = (u16int *)(0xB8000 + KERNEL_VIRT_BASE);
//u16int *videoMemory = (u16int *)0xB8000;
u8int cursor_y = 0;
u8int cursor_x = 0;

u8int max_rows = 25;
u8int max_cols = 80;

void move_cursor()
{
	outb(0x3D4, 14);
	outb(0x3D5, CURSOR_LOCATION >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, CURSOR_LOCATION);
}

u8int *monitor_get_cursor()
{
	u8int location[2];
	location[0] = cursor_x;
	location[1] = cursor_y;
	return location;
}

void kprint(char *c)
{
	monitor_write(c);
}

void khex(u32int num)
{
	monitor_write_base(num, 16);
}

void kdec(u32int num)
{
	monitor_write_base(num, 10);
}

void monitor_move_cursor(u8int x, u8int y)
{
	cursor_x = x;
	cursor_y = y;
	move_cursor();
}

void monitor_put(char c)
{
	u8int backColor = 0;
	u8int foreColor = 15;

	u8int attributeByte = (backColor << 4) | (foreColor & 0x0F);
	u16int attribute = attributeByte << 8;
	u16int *location;

	if(c == 0x08 && cursor_x)
		cursor_x--;

	else if (c == 0x09)
		cursor_x = (cursor_x+8) & ~(8-1);

	else if (c == '\r')
		cursor_x = 0;

	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if(c >= ' ')
	{
		location = videoMemory + CURSOR_LOCATION;
		*location = c | attribute;
		cursor_x++;
	}

	if(cursor_x >= max_cols)
	{
		cursor_x = 0;
		cursor_y++;
	}
	if(cursor_y >= max_rows)
	{
		monitor_scroll();
	}

	move_cursor();
}

void monitor_write(char c[])
{
	int i = 0;
	while(c[i])
	{
		monitor_put(c[i++]);
	}
}

int lzeros(u32int num, u32int base, u32int len)
{
	while(num != 0)
	{
		len--;
		num /= base;
	}
	return len;
}

void monitor_write_base(u32int num, u32int base)
{
	u32int baseNum;

	if(num > (base - 1))
	{
		monitor_write_base(num / base, base);
		baseNum = num % base;
	}
	else
	{
		baseNum = num;
	}

	if(baseNum > 9)
	{
		// Capital letters start at 65 but we have to
		// subtract 10 to get the offset
		monitor_put(baseNum + 65 - 10);
	}
	else
	{
		// Numbers start at 0x30 in ASCII
		monitor_put(baseNum + 0x30);
	}
}

void padprint(u32int num, u32int base, u32int len, char pad)
{
	int i = lzeros(num, base, len);
	if (i == len)
		i--;
	while(i > 0)
	{
		monitor_put(pad);
		i--;
	}
	monitor_write_base(num, base);
}

void monitor_clear()
{
	int i;
	for(i = 0; i < max_rows * max_cols; i++)
	{
		monitor_put(' ');
	}
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void monitor_scroll()
{
	// Get a space character with the default color attributes.
	u8int attributeByte = (0 << 4)|(15 & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	// Move the current text chunk that makes up the screen
	// back in the buffer by a line
	int i;
	for(i = 0; i < (max_cols-1) * max_rows; i++)
		videoMemory[i] = videoMemory[i+max_cols];

	for(i = 0; i < max_cols; i++)
		videoMemory[i+(max_cols*(max_rows-1))] = blank;

	cursor_y = 24;
}

void monitor_write_to_line(char c[], u8int line)
{
	int i;
	int j;

	u8int old_x = cursor_x;
	u8int old_y = cursor_y;

	cursor_x = 0;
	cursor_y = line;

	for(i = 0; c[i] != 0; i++)
	{
		monitor_put(c[i]);
	}

	for(j = 0;j < max_cols - i; j++)
	{
		monitor_put(' ');
	}

	cursor_x = old_x;
	cursor_y = old_y;
}
