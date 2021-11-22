#ifndef UTIL_H
#define UTIL_H 1

#include <stdint.h>

#ifdef _WIN32
#  include <synchapi.h>
#else
#  include <unistd.h>
#endif

// avoiding name conflict
void _sleep(uint32_t milliseconds);

#endif
