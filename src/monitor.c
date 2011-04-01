#include "monitor.h"
#include "common.h"

u16int *videoMemory = (u16int *)0xB8000;
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
