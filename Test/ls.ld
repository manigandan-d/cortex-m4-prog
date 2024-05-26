ENTRY(Reset_Handler)

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
	FLASH(rx): ORIGIN = 0x08000000, LENGTH = 1024K
	SRAM(rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.isr_vector : 
	{
		. = ALIGN(4);
		*(.isr_vector)
		. = ALIGN(4);
	} >FLASH
	
	.text : 
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		*(.init)
		*(.fini)
		. = ALIGN(4);
		_etext = .;
	} >FLASH

	.rodata : 
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	} >FLASH
	
	_sidata = LOADADDR(.data);

	.data : 
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data*)	
		. = ALIGN(4);
		_edata = .;
	} >SRAM AT> FLASH

	.bss : 
	{
		_sbss = .;
		__bss_start__ = _sbss;
		. = ALIGN(4);
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
		. = ALIGN(4);
		end = .;
		__end__ = .;
	} >SRAM
}


