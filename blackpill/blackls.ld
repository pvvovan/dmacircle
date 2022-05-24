ENTRY(reset_handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);    /* end of RAM */

MEMORY
{
	FLASH (rx)	: ORIGIN = 0x08000000, LENGTH = 512K
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	}
	.text :
	{
		. = ALIGN(4);
		*(.text)
	} > FLASH
	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
	}
	.data :
	{
		. = ALIGN(4);
		*(.data)
	} > RAM AT > FLASH
	.bss :
	{
		. = ALIGN(4);
		*(.bss)
	} > RAM
}
