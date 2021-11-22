#ifndef USERINFO_H
#define USERINFO_H 1

#include <stddef.h>

#define NAME_MAX 256

#if defined(__cplusplus)
   extern "C" {
#endif

size_t get_username(char* dst, size_t sz);

size_t get_hostname(char* dst, size_t sz);

size_t get_entire_name(char* dst, size_t sz, size_t sz_username);

#if defined(__cplusplus)
   }
#endif

#endif

