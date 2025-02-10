#ifndef MAGICNUMS_H_
#define MAGICNUMS_H_

#include "../ull.h"

extern U64 generate_magic_number();
extern U64 find_magic_number(int square, int relevant_bits, int bishop);
extern void init_magic_numbers();


#endif