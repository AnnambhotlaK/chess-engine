#ifndef SLIDERATTACKS_H_
#define SLIDERATTACKS_H_

#include "../ull.h"

extern U64 bishop_attacks_on_the_fly(int square, U64 block);
extern U64 rook_attacks_on_the_fly(int square, U64 block);

#endif