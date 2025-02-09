/* ***********************************
   ***********************************
        BITBOARD CHESS ENGINE IN C
   ***********************************
   ***********************************/

#include <stdio.h>
#include <string.h>

// include important constants, enums, functions
#include "ull.h"
#include "enums\squares.h"
#include "enums\colors.h"
#include "enums\rook_or_bishop.h"
#include "constants.h"
#include "macros.h"

#include "mask_attacks.c"

// count bits in bitboard
static inline int count_bits(U64 bitboard)
{
    // bit counter
    int count = 0;

    // consecutively reset least significant 1st bit
    // while bitboard has bits turned on
    while (bitboard)
    {
        count++;
        // reset bit
        bitboard &= bitboard - 1;
    }

    return count;
}

// get ls 1st bit index
static inline int get_ls1b_index(U64 bitboard)
{
    // make sure bitboard != 0
    if (bitboard)
    {
        return count_bits(((bitboard & -bitboard) - 1));
    }
    // otherwise return illegal index
    return -1;
}

/* ***********************************
    RANDOM NUMBER GENERATION PROCESS
   ***********************************/

// pseudorandom number state
unsigned int random_state = 1804289383;

// generate 32-bit psuedorandom legal nums
// necessary for magic numbers
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = random_state;

    // use XORSHIFT32 algo
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update random state
    random_state = number;

    return number;
}

// generate 64 bit pseudorandom legal nums
U64 get_random_U64_numbers()
{
    // def 4 random nums
    U64 n1, n2, n3, n4;

    // init random nums
    // slicing only first 16 bits
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;

    // return bitboard comprised of all numbers concatenated
    // Use left shifts and bitwise or
    return (n1 | (n2 << 16) | (n3 << 32) | (n4 << 48));
}

// generate magic number candidate
U64 generate_magic_number()
{
    return (get_random_U64_numbers() & get_random_U64_numbers() & get_random_U64_numbers());
}

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

        IMPORTANT MOVE ARRAYS

   ***********************************/

// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

// knight attacks table [square]
U64 knight_attacks[64];

// king attacks table [square]
U64 king_attacks[64];

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
        if ((1ULL << (r * 8 + f)) & block)
            break;
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

// generates possible occupancy bits based on a piece's index
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask)
{

    // occupancy map
    U64 occupancy = 0ULL;

    // loop over range of bits within attack_mask
    for (int count = 0; count < bits_in_mask; count++)
    {
        // get index of ls1b of attacks mask
        int square = get_ls1b_index(attack_mask);

        // pop ls1b
        pop_bit(attack_mask, square);

        // make sure occupancy is on board
        if (index & (1 << count))
        {
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}

/* ***********************************
   ***********************************
        MAGIC NUMBERS ROUTINE
   ***********************************
   ***********************************/

// find appropriate magic number for piece on square
U64 find_magic_number(int square, int relevant_bits, int bishop)
{
    // init occupancy and attacks arrays
    U64 occupancies[4096];
    U64 attacks[4096];

    // init used attacks
    U64 used_attacks[4096];

    // init attack mask for current piece
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    int occupancy_indices = 1 << relevant_bits;

    // loop over occupancy indices
    for (int index = 0; index < occupancy_indices; index++)
    {
        // init occupancies and attacks
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);
        attacks[index] = bishop ? bishop_attacks_on_the_fly(square, occupancies[index]) : rook_attacks_on_the_fly(square, occupancies[index]);
    }

    // test magic numbers loop
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // generate magic number candidates
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) {
            continue;
        }

        // init used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        int index, fail;
        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancy_indices; index++) {
            // define magic index with universal formula
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if (used_attacks[magic_index] == 0ULL) {
                // init new used_attack
                used_attacks[magic_index] = attacks[index];
            }
            // otherwise, 
            else if (used_attacks[magic_index] != attacks[index]) {
                // magic index doesn't work
                fail = 1;
            }
        }
        // if we didn't fail, return the successful magic number
        if (!fail) {
            return magic_number;
        }
    }
    // if magic number doesn't work and we did fail
    printf("     Magic number failed...");
    return 0ULL;
}

// init magic numbers
void init_magic_numbers() {
    // loop over 64 board squares
    for (int square = 0; square < 64; square++) {
        // init rook magic numbers
        //rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], rook);
    }
    for (int square = 0; square < 64; square++) {
        // init bishop magic numbers
        //bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], bishop);
    }
}

void init_all() {
    init_leapers_attacks();
}


/* ***********************************
   ***********************************
              MAIN DRIVER
   ***********************************
   ***********************************/

int main(void)
{
    init_all();
    U64 bitboard = 0ULL;
    set_bit(bitboard, e4);
    print_bitboard(bitboard);
    return 0;
}