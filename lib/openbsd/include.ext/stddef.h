#ifndef _STDDEF_H_
#define _STDDEF_H_

#include        <stdint.h>

typedef uint32_t        size_t;
typedef uint16_t        wint_t;
typedef uint16_t        wchar_t;
typedef int             bool;

//typedef char *          va_list;

#if 0
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#endif

#define NULL            ((void*)0)
#define false           0

#endif
