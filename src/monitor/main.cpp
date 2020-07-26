void main() {
    asm volatile("int $0x80");
}

// Have to define because there is no nice libc that does this for us; see
// https://web.archive.org/web/20200421041419/http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html
// for more info on what _start() does.
extern "C" void _start() {
    main();
}
