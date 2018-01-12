ENTRY(start)

SECTIONS {
    . = 1M;
    .boot  ALIGN(0x4000)
    {
        /* ensure that the Multiboot header is at the beginning, and
           that we force it to be kept despite any --gc-section option
           provided to ld that would cause it to strip any sections it
           thinks is unnecessary */
        KEEP(*(.multiboot_header))
    }

    .rodata ALIGN(0x1000)
    {
        *(.rodata)
        *(.rodata.*)
    }

    .bss ALIGN(0x1000)
    {
        *(.bss)
    }

    .text ALIGN(0x1000)
    {
        *(.text)
    }

    /DISCARD/ : { *(.comment) }
}
