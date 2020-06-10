typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool;

#define true  1
#define false 0

#define CPUID_APIC_SUPPORTED_BIT     (1 << 9)

typedef struct idtDescriptor 
{
    uint16_t offset_0_15;
    uint16_t seg_selector;
    uint8_t  ist_index;
    uint8_t  type;
    uint16_t offset_16_31;
    uint32_t offset_32_63;
    uint32_t reserved;
} __attribute__((packed)) idtDescriptor_t;

struct idt
{
    idtDescriptor_t descriptors[256];
} __attribute__((packed)) gIdt;

struct idtr 
{
    uint16_t limit;
    void *   offset;
} __attribute__((packed));

static inline void cpuid(uint32_t inCode, uint32_t * eax, uint32_t * edx)
{
    asm volatile("cpuid":"=a"(*eax),"=d"(*edx):"a"(inCode):"ecx","ebx");
}

bool is_apic_supported(void)
{
    uint32_t eax, edx;
    cpuid(1, &eax, &edx);
    return (bool)(0 != (edx & CPUID_APIC_SUPPORTED_BIT));
}

void interrupts_enable(bool inEnable)
{
    if(inEnable) 
    {
        asm volatile("sti");
    }
    else
    {
        asm volatile("cli");
    }
    
}

void load_idtr(void * inIdt)
{
    struct idtr idt_info;
    idt_info.limit = sizeof(struct idt);
    idt_info.offset = inIdt;
    asm volatile("lidt %0": :"m"(idt_info));
}

int kboot_main(void * inBootInfo)
{
    if(!is_apic_supported())
    {
        *((uint64_t *)0xB8000) = 0x4F414F204F4F4F4E;
        *((uint64_t *)0xB8008) = 0x0F204F434F494F50;
        return -1;
    }
    
    load_idtr(&gIdt);

    *((uint64_t *)0xB8000) = 0x4F204F4B4F4F4F20;

    return -10;
}
