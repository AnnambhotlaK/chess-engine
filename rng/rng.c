#include "rng.h"

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