#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

struct PanicInfo {
    const char *file;
    const char *function;
    int line;
};

[[noreturn]]
inline void panic(const PanicInfo& info, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n\nPanic at %s:%d (%s)\n", info.file, info.line, info.function);
    abort();
}

// debug macros

#define SM_DEBUG_ASSERTF(EXPR, ...) \
    do { \
        if (EXPR) { break; } \
        constexpr PanicInfo kPanicInfo = { __FILE__, __FUNCTION__, __LINE__ }; \
        panic(kPanicInfo, __VA_ARGS__); \
    } while (false)

#define SM_ASSERTF(EXPR, ...) SM_DEBUG_ASSERTF(EXPR, __VA_ARGS__)
#define SM_ASSERT(EXPR) SM_ASSERTF(EXPR, #EXPR)
#define SM_NEVER(...) SM_ASSERTF(false, __VA_ARGS__)
