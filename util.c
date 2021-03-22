#include "util.h"

uint16_t ctoui16(char hex0, char hex1)
{
	union char_int16_t;
	__char_int16_t.hex[0] = hex0;
	__char_int16_t.hex[1] = hex1;
	return __char_int16_t.uint;
}

uint32_t ctoui32(char hex0, char hex1, char hex2, char hex3)
{
	union char_int32_t;
	__char_int32_t.hex[0] = hex0;
	__char_int32_t.hex[1] = hex1;
	__char_int32_t.hex[2] = hex2;
	__char_int32_t.hex[3] = hex3;
	return __char_int32_t.uint;
}

