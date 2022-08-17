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
		*(.glue_7)		/* glue arm to thumb code */
		*(.glue_7t)		/* glue thumb to arm code */
		*(.eh_frame)
		*(.tm_clone_table)
		KEEP(*(.init))
		KEEP(*(.fini))
		. = ALIGN(4);
	} > FLASH

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	} > FLASH

	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH
	.ARM :
	{
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} > FLASH
	.preinit_array :
	{
		PROVIDE_HIDDEN(__preinit_array_start = .);
		KEEP(*(.preinit_array*))
		PROVIDE_HIDDEN(__preinit_array_end = .);
	} > FLASH
	.init_array :
	{
		PROVIDE_HIDDEN(__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array*))
		PROVIDE_HIDDEN(__init_array_end = .);
	} > FLASH
	.fini_array :
	{
		PROVIDE_HIDDEN(__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array*))
		PROVIDE_HIDDEN(__fini_array_end = .);
	} > FLASH

	_siram_exec = LOADADDR(.ram_exec);
	.ram_exec :
	{
		. = ALIGN(4);
		_startram_exec = .;
		*(.ram_exec)
		*(.ram_exec*)
		. = ALIGN(4);
		_endram_exec = .;
	} > RAM AT > FLASH

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


/*	/DISCARD/ :			*/
/*	{				*/
/*		*(.debug*)		*/
/*		*(.comment)		*/
/*		*(.ARM.attributes)	*/
/*		*(.iplt)		*/
/*		*(.rel.iplt)		*/
/*		*(.igot.plt)		*/
/*		*(.vfp11_veneer)	*/
/*		*(.v4_bx)		*/
/*	}				*/

}
