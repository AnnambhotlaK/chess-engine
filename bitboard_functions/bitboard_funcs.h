#ifndef BITBOARDFUNCS_H_
#define BITBOARDFUNCS_H_

#include "../ull.h"

extern inline int count_bits(U64 bitboard);
extern inline int get_ls1b_index(U64 bitboard);
extern void print_bitboard(U64 bitboard);
extern U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

#endif