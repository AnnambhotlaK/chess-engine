#include "slider_attacks.h"

// generates bishop attacks on the fly
U64 bishop_attacks_on_the_fly(int square, U64 block)
{
    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;
    // init target rank & files
    int tRank = square / 8;
    int tFile = square % 8;

    // generate bishop attacks
    for (r = tRank + 1, f = tFile + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) {
            break;
        }
    }
    for (r = tRank - 1, f = tFile + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }
    for (r = tRank + 1, f = tFile - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }
    for (r = tRank - 1, f = tFile - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    return attacks;
}

U64 rook_attacks_on_the_fly(int square, U64 block)
{
    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;
    // init target rank & files
    int tRank = square / 8;
    int tFile = square % 8;

    // generate rook attacks
    for (r = tRank + 1; r <= 6; r++)
    {
        attacks |= (1ULL << (r * 8 + tFile));
        if ((1ULL << (r * 8 + tFile)) & block)
            break;
    }
    for (r = tRank - 1; r >= 1; r--)
    {
        attacks |= (1ULL << (r * 8 + tFile));
        if ((1ULL << (r * 8 + tFile)) & block)
            break;
    }
    for (f = tFile + 1; f <= 6; f++)
    {
        attacks |= (1ULL << (tRank * 8 + f));
        if ((1ULL << (tRank * 8 + f)) & block)
            break;
    }
    for (f = tFile - 1; f >= 1; f--)
    {
        attacks |= (1ULL << (tRank * 8 + f));
        if ((1ULL << (tRank * 8 + f)) & block)
            break;
    }
    return attacks;
}