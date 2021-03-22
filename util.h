#ifndef UTIL_H
#define UTIL_H
#include <stdlib.h>

#define printf_clr(str) printf(str KWHT"\n")
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

union {
	char hex[2];
	uint16_t uint;
} __char_int16_t;

union {
	char hex[4];
	uint32_t uint;
} __char_int32_t;

__attribute__ ((noinline)) uint16_t ctoui16(char hex0, char hex1);
__attribute__ ((noinline)) uint32_t ctoui32(char hex0, char hex1, char hex2, char hex3);

#endif
