section .multiboot_header
header_start:
    dd 0xE85250D6     ; Magic number for Multiboot 2 specification
    dd 0              ; Architecture type 0 (protected mode x86)
    dd header_end - header_start    ; Header length

    ; Calculate the checksum for the Multiboot header.
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

    ; Other Multiboot tags would go here if we had any.

    ; End with the required end tag.
    dw 0                            ; Type
    dw 0                            ; Flags
    dd 8                            ; Size
header_end:
