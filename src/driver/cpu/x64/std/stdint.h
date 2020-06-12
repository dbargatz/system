#ifndef _STD_STDINT_H
#define _STD_STDINT_H

#define UINT8_MIN (0x00)
#define UINT8_MAX (0xFF)

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef uint64_t            size_t;

typedef float               float32_t;
typedef double              float64_t;

#endif // _STD_STDINT_H
