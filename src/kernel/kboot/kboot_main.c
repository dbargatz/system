int kboot_main(void * inBootInfo)
{
    *((long long *)0xb8000) = 0x0A4B0A4F0A4B0A4F;
    return -1;
}