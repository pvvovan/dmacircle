ENTRY(reset_handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);    /* end of RAM */

MEMORY
{
	FLASH (rx)	: ORIGIN = 0x08000000, LENGTH = 64K
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
	.isr_stack :
	{
		. = ALIGN(4);
		KEEP(*(.isr_stack))
		. = ALIGN(4);
	} > FLASH

	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} > FLASH

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		. = ALIGN(4);
	} > FLASH

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	}

	_sidata = LOADADDR(.data);
	.data :
	{
		. = ALIGN(4);
		_start_data = .;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_end_data = .;
	} > RAM AT > FLASH

	.bss :
	{
		. = ALIGN(4);
		_start_bss = .;
		__bss_start__ = _start_bss;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
		_end_bss = .;
		end = .;
		__bss_end__ = _end_bss;
		__end__ = .;
	} > RAM
}
