typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long long uint64_t;

namespace kernel::platform::x86_64
{
    static uint16_t * BASE_ADDRESS = ((uint16_t *)0xB8000);
    constexpr uint8_t COLUMNS = 80;
    constexpr uint8_t ROWS = 25;

    class VGA
    {
    private:
        uint8_t curColumn = 0;
        uint8_t curRow = 0;

    public:
        enum class Color : uint8_t
        {
            BLACK          = 0x0,
            BLUE           = 0x1,
            GREEN          = 0x2,
            CYAN           = 0x3,
            RED            = 0x4,
            MAGENTA        = 0x5,
            BROWN          = 0x6,
            GRAY           = 0x7,
            DARK_GRAY      = 0x8,
            BRIGHT_BLUE    = 0x9,
            BRIGHT_GREEN   = 0xA,
            BRIGHT_CYAN    = 0xB,
            BRIGHT_RED     = 0xC,
            BRIGHT_MAGENTA = 0xD,
            YELLOW         = 0xE,
            WHITE          = 0xF,
        };

        void SetPosition(uint8_t inColumn, uint8_t inRow)
        {
            // TODO: ensure these are in-bounds
            curRow = inRow;
            curColumn = inColumn;
        }

        void Clear(Color inColor)
        {
            SetPosition(0, 0);
            for(uint16_t i = 0; i < ROWS * COLUMNS; i++)
            {
                Write((unsigned char *)" ", inColor, inColor);
            }
            SetPosition(0, 0);
        }

        void Scroll(void)
        {
            // Copy lines 2-80 to lines 1-79.
            uint16_t * dest = BASE_ADDRESS;
            uint16_t * src = dest + (sizeof(uint16_t) * COLUMNS);
            uint16_t * end = dest + (sizeof(uint16_t) * ((ROWS - 1) * COLUMNS));

            while(dest != end)
            {
                *dest++ = *src++;
            }
        }

        void Write(const void * inString, Color inFgColor, Color inBgColor)
        {
            uint16_t color = ((uint8_t)inBgColor << 4 | (uint8_t)inFgColor);
            uint8_t * curChar = (uint8_t *)inString;
            while('\0' != *curChar)
            {
                // Move to the next line (row) if we see a newline or are past
                // the screen width.
                if('\n' == *curChar || curColumn >= COLUMNS)
                {
                    curRow++;
                    if(curRow >= ROWS)
                    {
                        Scroll();
                    }
                    curColumn = 0;
                }

                if('\n' != *curChar)
                {
                    uint16_t * addr = BASE_ADDRESS + curColumn + (COLUMNS * curRow);
                    *addr = color << 8 | *curChar;
                    curColumn++;
                }
                
                curChar++;
            }
            
        }
    };
};

extern "C" int kmain(void * inBootInfo)
{
    using kernel::platform::x86_64::VGA;

    VGA vga;
    vga.Clear(VGA::Color::DARK_GRAY);

    vga.Write("Booting\n", VGA::Color::WHITE, VGA::Color::RED);
    vga.Write("Hello, world!", VGA::Color::WHITE, VGA::Color::RED);

    uint8_t count = 0;
    while(true)
    {
        // Spin forever!
        switch(count)
        {
            case 0:
                vga.Write("0", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 1:
                vga.Write("1", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 2:
                vga.Write("2", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 3:
                vga.Write("3", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 4:
                vga.Write("4", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 5:
                vga.Write("5", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 6:
                vga.Write("6", VGA::Color::WHITE, VGA::Color::RED);
                break;
            case 7:
                vga.Write("7", VGA::Color::WHITE, VGA::Color::RED);
                break;
            default:
                count = -1;
                vga.Write("8", VGA::Color::WHITE, VGA::Color::RED);
                break;
        }
        
        count++;
    }
}