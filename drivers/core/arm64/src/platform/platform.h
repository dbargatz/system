#if PLATFORM == raspi3b
  #include "raspi3b/platform.h"
#else
  #error PLATFORM compile-time define was not specified or was invalid
#endif
