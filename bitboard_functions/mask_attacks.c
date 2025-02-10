#include "mask_attacks.h"
#include "../macros.h"

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square)
{

    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // white pawns
    if (!side)
    {
        if ((bitboard >> 7) & not_a_file)
        {
            attacks |= (bitboard >> 7);
        }
        if ((bitboard >> 9) & not_h_file)
        {
            attacks |= (bitboard >> 9);
        }
    }
    // black pawns
    else
    {
        if ((bitboard << 7) & not_h_file)
        {
            attacks |= (bitboard << 7);
        }
        if ((bitboard << 9) & not_a_file)
        {
            attacks |= (bitboard << 9);
        }
    }

    // return attack map
    return attacks;
}

// generate knight attacks
U64 mask_knight_attacks(int square)
{

    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate moves
    if ((bitboard >> 17) & not_h_file)
    {
        attacks |= (bitboard >> 17);
    }
    if ((bitboard >> 15) & not_a_file)
    {
        attacks |= (bitboard >> 15);
    }
    if ((bitboard >> 10) & not_hg_file)
    {
        attacks |= (bitboard >> 10);
    }
    if ((bitboard >> 6) & not_ab_file)
    {
        attacks |= (bitboard >> 6);
    }
    if ((bitboard << 17) & not_a_file)
    {
        attacks |= (bitboard << 17);
    }
    if ((bitboard << 15) & not_h_file)
    {
        attacks |= (bitboard << 15);
    }
    if ((bitboard << 10) & not_ab_file)
    {
        attacks |= (bitboard << 10);
    }
    if ((bitboard << 6) & not_hg_file)
    {
        attacks |= (bitboard << 6);
    }

    return attacks;
}

U64 mask_king_attacks(int square)
{

    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate king attacks
    if (bitboard >> 8)
    {
        attacks |= (bitboard >> 8);
    }
    if ((bitboard >> 9) & not_h_file)
    {
        attacks |= (bitboard >> 9);
    }
    if ((bitboard >> 7) & not_a_file)
    {
        attacks |= (bitboard >> 7);
    }
    if ((bitboard >> 1) & not_h_file)
    {
        attacks |= (bitboard >> 1);
    }
    if (bitboard << 8)
    {
        attacks |= (bitboard << 8);
    }
    if ((bitboard << 9) & not_a_file)
    {
        attacks |= (bitboard << 9);
    }
    if ((bitboard << 7) & not_h_file)
    {
        attacks |= (bitboard << 7);
    }
    if ((bitboard << 1) & not_a_file)
    {
        attacks |= (bitboard << 1);
    }

    return attacks;
}

U64 mask_bishop_attacks(int square)
{
    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;
    // init target rank & files
    int tRank = square / 8;
    int tFile = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tRank + 1, f = tFile + 1; r <= 6 && f <= 6; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
    }
    for (r = tRank - 1, f = tFile + 1; r >= 1 && f <= 6; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
    }
    for (r = tRank + 1, f = tFile - 1; r <= 6 && f >= 1; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
    }
    for (r = tRank - 1, f = tFile - 1; r >= 1 && f >= 1; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
    }

    return attacks;
}

U64 mask_rook_attacks(int square)
{
    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;
    // init target rank & files
    int tRank = square / 8;
    int tFile = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tRank + 1; r <= 6; r++)
    {
        attacks |= (1ULL << (r * 8 + tFile));
    }
    for (r = tRank - 1; r >= 1; r--)
    {
        attacks |= (1ULL << (r * 8 + tFile));
    }
    for (f = tFile + 1; f <= 6; f++)
    {
        attacks |= (1ULL << (tRank * 8 + f));
    }
    for (f = tFile - 1; f >= 1; f--)
    {
        attacks |= (1ULL << (tRank * 8 + f));
    }

    return attacks;
}