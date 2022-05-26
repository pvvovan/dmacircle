ENTRY(reset_handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);    /* end of RAM */

MEMORY
{
	FLASH (rx)	: ORIGIN = 0x08000000, LENGTH = 512K
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 128K
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
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
		_end_bss = .;
	} > RAM
}
