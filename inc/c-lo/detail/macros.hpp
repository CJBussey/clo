#pragma once

#if defined(_MSC_VER)
#define DETAIL_CLO_UNREACHABLE() __assume(0)
#elif defined(__GNUC__)
#define DETAIL_CLO_UNREACHABLE() __builtin_unreachable()
#else
#error "no unreachable support"
#endif