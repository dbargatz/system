#if PLATFORM == qemu-pc
  #include "platform/qemu_pc/platform.hpp"
#elif PLATFORM == raspi3b
  #include "platform/raspi3b/platform.hpp"
#else
  #error PLATFORM compile-time define was not specified or was invalid
#endif
