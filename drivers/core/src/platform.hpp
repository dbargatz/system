#ifndef _CORE_PLATFORM_HPP
#define _CORE_PLATFORM_HPP

#ifdef PLATFORM_QEMU_PC
  #include "platform/qemu_pc/platform.hpp"
  constexpr static const char * PLATFORM_NAME = "QEMU PC";
#elif PLATFORM_RASPI3B
  #include "platform/raspi3b/platform.hpp"
  constexpr static const char * PLATFORM_NAME = "Raspberry Pi Model 3B";
#else
  #error PLATFORM compile-time define was not specified or was invalid
#endif

#endif // _CORE_PLATFORM_HPP
