#if PLATFORM == qemu-pc
  #include "platform/qemu_pc/platform.hpp"
  constexpr static const char8_t* PLATFORM_NAME = u8"QEMU PC";
#elif PLATFORM == raspi3b
  #include "platform/raspi3b/platform.hpp"
  constexpr static const char8_t* PLATFORM_NAME = u8"Raspberry Pi Model 3B";
#else
  #error PLATFORM compile-time define was not specified or was invalid
#endif
