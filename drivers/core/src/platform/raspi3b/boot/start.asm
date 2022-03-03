// Derived from bzt's "raspi3-tutorial" on GitHub; original file located at:
// https://github.com/bztsrc/raspi3-tutorial/blob/dd48f11c74d6ea1fc013c206f99c9e23d643b6fa/01_bareminimum/start.S
// Modifications: Copyright (C) 2022 Dylan Bargatze.
// Original copyright/MIT License text follows:

// Copyright (C) 2018 bzt (bztsrc@github)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


// AArch64 mode
 
// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.globl _start
 
 .org 0x80000
_start:
    mrs    x0, mpidr_el1        
    and    x0, x0, #0xFF       // Check processor id
    cbz    x0, _bootstrap      // Hang for all non-primary CPU
    b      _hang

_hang:
    b _hang

_bootstrap:
    // set stack before our code
    ldr     x5, =_start
    mov     sp, x5
 
    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    // The processor ID is still in x0, and we don't have any boot information
    // for core_entry() on this platform, so zero out the x1 register.
    mov     x1, xzr
 
    // jump to C code, should not return
4:  bl      core_entry
    // for failsafe, halt this core too
    b _hang
