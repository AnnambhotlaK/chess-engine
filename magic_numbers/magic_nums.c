#include "magic_nums.h"
#include "../rng/rng.c"
#include "../bitboard_functions/bitboard_funcs.c"

// generate magic number candidate
U64 generate_magic_number()
{
    return (get_random_U64_numbers() & get_random_U64_numbers() & get_random_U64_numbers());
}

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