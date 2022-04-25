ENTRY(_start)
 
SECTIONS {
    . = 0x80000;

    __start = .;
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text .text.*)
        KEEP(*(.note.gnu.build-id))
    }
    . = ALIGN(0x1000);
    __text_end = .;
 
    __rodata_start = .;
    .rodata :
    {
        *(.rodata .rodata.*)
    }
    . = ALIGN(0x1000);
    __rodata_end = .;
 
    __data_start = .;
    .data :
    {
        *(.data .data.*)
    }
    . = ALIGN(0x1000);
    __data_end = .;
 
    __bss_start = .;
    .bss (NOLOAD):
    {
        bss = .;
        *(.bss .bss.*)
    }
    . = ALIGN(0x1000);
    __bss_end = .;
    __end = .;

    __bss_size = __bss_end - __bss_start;
}
