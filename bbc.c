/* ***********************************
   ***********************************
        BITBOARD CHESS ENGINE IN C
   ***********************************
   ***********************************/

#include <stdio.h>
#include <string.h>

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

// rook or bishop (for magic num generation)
enum
{
    rook,
    bishop
};

const char *square_to_coordinates[] = {
    "a8",
    "b8",
    "c8",
    "d8",
    "e8",
    "f8",
    "g8",
    "h8",
    "a7",
    "b7",
    "c7",
    "d7",
    "e7",
    "f7",
    "g7",
    "h7",
    "a6",
    "b6",
    "c6",
    "d6",
    "e6",
    "f6",
    "g6",
    "h6",
    "a5",
    "b5",
    "c5",
    "d5",
    "e5",
    "f5",
    "g5",
    "h5",
    "a4",
    "b4",
    "c4",
    "d4",
    "e4",
    "f4",
    "g4",
    "h4",
    "a3",
    "b3",
    "c3",
    "d3",
    "e3",
    "f3",
    "g3",
    "h3",
    "a2",
    "b2",
    "c2",
    "d2",
    "e2",
    "f2",
    "g2",
    "h2",
    "a1",
    "b1",
    "c1",
    "d1",
    "e1",
    "f1",
    "g1",
    "h1",
};

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

   IMPORTANT MOVE ARRAYS, BITWISE CONSTANTS

   ***********************************/

const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 18229723555195321596ULL;

// bishop relevant occupancy bit counts for each square
const int bishop_relevant_bits[64] = {
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    7,
    7,
    7,
    7,
    5,
    5,
    5,
    5,
    7,
    9,
    9,
    7,
    5,
    5,
    5,
    5,
    7,
    9,
    9,
    7,
    5,
    5,
    5,
    5,
    7,
    7,
    7,
    7,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    6,
};

// rook relevant occupancy bit counts for each square
const int rook_relevant_bits[64] = {
    12,
    11,
    11,
    11,
    11,
    11,
    11,
    12,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    12,
    11,
    11,
    11,
    11,
    11,
    11,
    12,
};

// rook magic nums
const U64 rook_magic_numbers[64] = {
    0x98060280800000ULL,
    0x200102080410000ULL,
    0x1084003808010000ULL,
    0x240040040880000ULL,
    0x400024020140206ULL,
    0x400a11002040000ULL,
    0x400100140844800ULL,
    0x2080003444810200ULL,
    0x3010200800000ULL,
    0x4006422210000ULL,
    0x944100402080000ULL,
    0x8104100012c0a000ULL,
    0x4800800400088000ULL,
    0x802200040000ULL,
    0x800108024800ULL,
    0x40200092004400ULL,
    0xe40840026010000ULL,
    0x10020c020100002ULL,
    0x2010040014ULL,
    0x10240014088036ULL,
    0x818080040b0000ULL,
    0x805000a010020000ULL,
    0x4250012041800400ULL,
    0x4001200800409200ULL,
    0x100010a08040000ULL,
    0x20601008003ULL,
    0x20000043a0008001ULL,
    0x8000030030082001ULL,
    0x2808083444ULL,
    0x10e701041201800ULL,
    0x4480000c02020000ULL,
    0x124010108000ULL,
    0x600108400410001ULL,
    0x6000043000200002ULL,
    0xa804850c04004002ULL,
    0x14008080000ULL,
    0x4000240004004001ULL,
    0x2004120008101000ULL,
    0x10000c4285003000ULL,
    0xc00000020100d000ULL,
    0x1c0040a021220006ULL,
    0x8000004020028011ULL,
    0x8060c20003ULL,
    0x8020008400440001ULL,
    0xa62040000ULL,
    0x1418000202060010ULL,
    0x2404000401010000ULL,
    0x42100820000ULL,
    0xc000004000800080ULL,
    0x100002000400106ULL,
    0x900002048a401200ULL,
    0x201001000082100ULL,
    0x40000042a010a00ULL,
    0x130000409508200ULL,
    0x800000111000408ULL,
    0x80c000891220040ULL,
    0x280048440122066ULL,
    0x600004010920104ULL,
    0x308001008020500cULL,
    0x820000425001001ULL,
    0x8200002c0201084ULL,
    0x802024004209040cULL,
    0x1060000801100204ULL,
    0x88102100488402ULL,
};

// bishop magic nums
const U64 bishop_magic_numbers[64] = {
    0x2020448008100ULL,
    0x1820843102002050ULL,
    0x8908108112005000ULL,
    0x4042088010220ULL,
    0x4124504000060141ULL,
    0x2001042240440002ULL,
    0x104008884100082ULL,
    0x61208020210d0ULL,
    0x1015060a1092212ULL,
    0x10b4080228004900ULL,
    0x8200040822004025ULL,
    0x2022082044800ULL,
    0xa002411140224800ULL,
    0x120084802500004ULL,
    0x412804606104280ULL,
    0xcab0088088080250ULL,
    0x480081010c202ULL,
    0x108803602244400ULL,
    0x20884a1003820010ULL,
    0x44044824001061ULL,
    0x2200400a000a0ULL,
    0x6000248020802ULL,
    0x181020200900400ULL,
    0x8802208200842404ULL,
    0x120226064040400ULL,
    0x4021004284808ULL,
    0x4001404094050200ULL,
    0x480a040008010820ULL,
    0x2194082044002002ULL,
    0x2008a20001004200ULL,
    0x40908041041004ULL,
    0x881002200540404ULL,
    0x4001082002082101ULL,
    0x8110408880880ULL,
    0x8000404040080200ULL,
    0x200020082180080ULL,
    0x1184440400114100ULL,
    0xc220008020110412ULL,
    0x4088084040090100ULL,
    0x8822104100121080ULL,
    0x100111884008200aULL,
    0x2844040288820200ULL,
    0x90901088003010ULL,
    0x1000a218000400ULL,
    0x1102010420204ULL,
    0x8414a3483000200ULL,
    0x6410849901420400ULL,
    0x201080200901040ULL,
    0x204880808050002ULL,
    0x1001008201210000ULL,
    0x16a6300a890040aULL,
    0x8049000441108600ULL,
    0x2212002060410044ULL,
    0x100086308020020ULL,
    0x484241408020421ULL,
    0x105084028429c085ULL,
    0x4282480801080cULL,
    0x81c098488088240ULL,
    0x1400000090480820ULL,
    0x4444000030208810ULL,
    0x1020142010820200ULL,
    0x2234802004018200ULL,
    0xc2040450820a00ULL,
    0x2101021090020ULL,
};

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
        rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], rook);
    }
    for (int square = 0; square < 64; square++) {
        // init bishop magic numbers
        bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], bishop);
    }
}

/* ***********************************
   ***********************************
            INIT ALL VARIABLES
   ***********************************
   ***********************************/
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
    return 0;
}