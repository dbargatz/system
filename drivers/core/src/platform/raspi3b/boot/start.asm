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
    mrs    x2, mpidr_el1       // Grab the equivalent of an ID for the current
    and    x2, x2, #0xFF       // core (technically its affinity level 0), then
    cbz    x2, _bootstrap      // bootstrap the primary core (id == 0) only and
    b      _hang               // hang all non-primary cores (id != 0).

_hang:
    wfe                        // Core will sleep until signalled; much more
    b _hang                    // power-efficient than a spin-loop. If core IS
                               // signalled, for now just loop back to waiting
                               // for an event (wfe).

_bootstrap:
    // set stack before our code
    ldr     x5, =_start
    mov     sp, x5
 
    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
_bss_loop:
    cbz     w6, _core_entry
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, _bss_loop

_core_entry:
    mov     x1, x0             // x0 contains the 32-bit physical address of
    mov     x0, x2             // the DTB, and x2 contains the processor ID.
                               // Move the args to their expected registers for
                               // the core_entry() function.

    // jump to C code, should not return
    bl      core_entry
    // for failsafe, halt this core too
    b       _hang
