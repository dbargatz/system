#ifdef PLATFORM_QEMU_PC
  #include "platform/qemu_pc/platform.hpp"
  constexpr static const char8_t* PLATFORM_NAME = u8"QEMU PC";
#elif PLATFORM_RASPI3B
  #include "platform/raspi3b/platform.hpp"
  constexpr static const char8_t* PLATFORM_NAME = u8"Raspberry Pi Model 3B";
#else
  #error PLATFORM compile-time define was not specified or was invalid
#endif
