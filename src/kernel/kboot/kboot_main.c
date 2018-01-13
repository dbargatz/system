typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool;

#define true  1
#define false 0

#define APIC_SUPPORTED_BIT (1 << 9)

static inline void cpuid(uint32_t inCode, uint32_t * eax, uint32_t * edx)
{
    asm volatile("cpuid":"=a"(*eax),"=d"(*edx):"a"(inCode):"ecx","ebx");
}

bool is_apic_supported(void)
{
    uint32_t eax, edx;
    cpuid(1, &eax, &edx);
    return (bool)(0 != (edx & APIC_SUPPORTED_BIT));
}

int kboot_main(void * inBootInfo)
{
    if(is_apic_supported())
    {
        *((uint64_t *)0xb8000) = 0x0A430A490A500A41;
    }
    else
    {
        *((uint64_t *)0xb8000) = 0x4F4F4F4E4F4F4F4E;
    }
    
    return -1;
}