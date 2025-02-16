#ifndef SLIDERATTACKS_H_
#define SLIDERATTACKS_H_

#include "../ull.h"

extern U64 bishop_attacks_on_the_fly(int square, U64 block);
extern inline U64 get_bishop_attacks(int square, U64 occupancy);
extern U64 rook_attacks_on_the_fly(int square, U64 block);
extern inline U64 get_rook_attacks(int square, U64 occupancy);
extern void init_sliders_attacks(int bishop);

#endif