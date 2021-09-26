ENTRY(_start)
 
SECTIONS {
    . = 512K;

    __start = .;
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(0x1000);
    __text_end = .;
 
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(0x1000);
    __rodata_end = .;
 
    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(0x1000);
    __data_end = .;
 
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(0x1000);
    __bss_end = .;
    __end = .;

    __bss_size = __bss_end - __bss_start;
}
