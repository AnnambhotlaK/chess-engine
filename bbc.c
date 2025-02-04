#include <stdio.h>

/* ***********************************
         MACROS, DATA TYPES
   ***********************************/

// define bitboard data type
// 64-bit unsigned int
#define U64 unsigned long long

// set, get, pop macros for bitboard
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

/* ***********************************
            ENUM DEFINITIONS
   ***********************************/

// board squares
enum
{
    a8,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8,
    a7,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a6,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a5,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a4,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a3,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a2,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a1,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
};

// sides to move (colors)
enum
{
    white,
    black
};

/* Need for future
"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
*/

// print bitboard function
// for debugging
void print_bitboard(U64 bitboard)
{
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            // print ranks
            if (!file)
            {
                printf(" %d ", 8 - rank);
            }
            // print bit state (1 for piece, 0 for none)
            printf(" %d", get_bit(bitboard, square) ? 1 : 0);
        }
        // print newline every rank
        printf("\n");
    }
    // print files
    printf("\n    a b c d e f g h\n");
    printf("\n");

    // print bitboard as unsigned decimal number
    printf("Bitboard as Decimal: %llud\n\n", bitboard);
}

/* ***********************************

   OFF-BOARD MOVE PREVENTION CONSTANTS

   ***********************************/

const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 18229723555195321596ULL;

// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

// knight attacks table [square]
U64 knight_attacks[64];

// king attacks table [square]
U64 king_attacks[64];

/* ***********************************
   ***********************************
        ATTACK GENERATION FUNCTIONS
   ***********************************
   ***********************************/

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
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tRank - 1, f = tFile + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tRank + 1, f = tFile - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tRank - 1, f = tFile - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
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
        if ((1ULL << (r * 8 + tFile)) & block) break;
    }
    for (r = tRank - 1; r >= 1; r--)
    {
        attacks |= (1ULL << (r * 8 + tFile));
        if ((1ULL << (r * 8 + tFile)) & block) break;
    }
    for (f = tFile + 1; f <= 6; f++)
    {
        attacks |= (1ULL << (tRank * 8 + f));
        if ((1ULL << (tRank * 8 + f)) & block) break;
    }
    for (f = tFile - 1; f >= 1; f--)
    {
        attacks |= (1ULL << (tRank * 8 + f));
        if ((1ULL << (tRank * 8 + f)) & block) break;
    }
    return attacks;
}




// init leaper pieces attacks
void init_leapers_attacks()
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

/* ***********************************
   ***********************************
              MAIN DRIVER
   ***********************************
   ***********************************/

int main(void)
{
    // init leaper pieces attacks
    init_leapers_attacks();

    // init occupancy bitboard
    U64 block = 0ULL;
    set_bit(block, d7);
    set_bit(block, d3);
    set_bit(block, g4);
    set_bit(block, b4);
    print_bitboard(block);
    print_bitboard(rook_attacks_on_the_fly(d4, block));
}