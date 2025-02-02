#include <stdio.h>

// make debug && ./bbc
// (cmd to compile, debug and run)

// define bitboard data type
// 64-bit unsigned int
#define U64 unsigned long long

// set, get, pop macros for bitboard
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

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
enum { white, black };

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

/*
    ATTACKS
*/

/*
 8  0 1 1 1 1 1 1 1
 7  0 1 1 1 1 1 1 1
 6  0 1 1 1 1 1 1 1
 5  0 1 1 1 1 1 1 1
 4  0 1 1 1 1 1 1 1
 3  0 1 1 1 1 1 1 1
 2  0 1 1 1 1 1 1 1
 1  0 1 1 1 1 1 1 1

    a b c d e f g h
*/
const U64 not_a_file = 18374403900871474942ULL;

/*
 8  1 1 1 1 1 1 1 0
 7  1 1 1 1 1 1 1 0
 6  1 1 1 1 1 1 1 0
 5  1 1 1 1 1 1 1 0
 4  1 1 1 1 1 1 1 0
 3  1 1 1 1 1 1 1 0
 2  1 1 1 1 1 1 1 0
 1  1 1 1 1 1 1 1 0

    a b c d e f g h
*/
const U64 not_h_file = 9187201950435737471ULL;

/*
 8  1 1 1 1 1 1 0 0
 7  1 1 1 1 1 1 0 0
 6  1 1 1 1 1 1 0 0
 5  1 1 1 1 1 1 0 0
 4  1 1 1 1 1 1 0 0
 3  1 1 1 1 1 1 0 0
 2  1 1 1 1 1 1 0 0
 1  1 1 1 1 1 1 0 0

    a b c d e f g h
*/
const U64 not_hg_file = 4557430888798830399ULL;

/*
 8  0 0 1 1 1 1 1 1
 7  0 0 1 1 1 1 1 1
 6  0 0 1 1 1 1 1 1
 5  0 0 1 1 1 1 1 1
 4  0 0 1 1 1 1 1 1
 3  0 0 1 1 1 1 1 1
 2  0 0 1 1 1 1 1 1
 1  0 0 1 1 1 1 1 1

    a b c d e f g h
*/
const U64 not_ab_file = 18229723555195321596ULL;


// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

// knight attacks table [square]
U64 knight_attacks[64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square) {
    
    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // white pawns
    if (!side) {
        if ((bitboard >> 7) & not_a_file) {
            attacks |= (bitboard >> 7);
        }
        if ((bitboard >> 9) & not_h_file) {
            attacks |= (bitboard >> 9);
        }
    }
    // black pawns
    else {
        if ((bitboard << 7) & not_h_file) {
            attacks |= (bitboard << 7);
        }
        if ((bitboard << 9) & not_a_file) {
            attacks |= (bitboard << 9);
        }
    }

    // return attack map
    return attacks;
}

// generate knight attacks
U64 mask_knight_attacks(int square) {

    // resulting attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate moves
    if ((bitboard >> 17) & not_h_file) {
        attacks |= (bitboard >> 17);
    }
    if ((bitboard >> 15) & not_a_file) {
        attacks |= (bitboard >> 15);
    }
    if ((bitboard >> 10) & not_hg_file) {
        attacks |= (bitboard >> 10);
    }
    if ((bitboard >> 6) & not_ab_file) {
        attacks |= (bitboard >> 6);
    }
    if ((bitboard << 17) & not_a_file) {
        attacks |= (bitboard << 17);
    }
    if ((bitboard << 15) & not_h_file) {
        attacks |= (bitboard << 15);
    }
    if ((bitboard << 10) & not_ab_file) {
        attacks |= (bitboard << 10);
    }
    if ((bitboard << 6) & not_hg_file) {
        attacks |= (bitboard << 6);
    }

    return attacks;

}

// init leaper pieces attacks
void init_leapers_attacks() {
    // loop over 64 board squares
    for (int square = 0; square < 64; square++) {
        // init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);
    }
}

int main(void)
{
    // init leaper pieces attacks
    init_leapers_attacks();

    // loop over board
    for (int square = 0; square < 64; square++) {
        print_bitboard(knight_attacks[square]);
    }
}