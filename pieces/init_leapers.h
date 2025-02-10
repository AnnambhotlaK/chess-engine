#ifndef INITLEAPERS_H_
#define INITLEAPERS_H_

#include "../ull.h"

// pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];

// knight attacks table [square]
extern U64 knight_attacks[64];

// king attacks table [square]
extern U64 king_attacks[64];

extern void init_leapers_attacks();

#endif