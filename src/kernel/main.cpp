typedef unsigned long long uint64_t;

extern "C" int main(void * inBootInfo)
{
    *((uint64_t *)0xB8000) = 0x4F204F4B4F4F4F20;

    while(true)
    {
        // Spin forever!
    }
}