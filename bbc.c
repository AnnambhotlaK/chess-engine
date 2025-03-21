/* ***********************************
   ***********************************
        BITBOARD CHESS ENGINE IN C
   ***********************************
   ***********************************/

#include <stdio.h>
#include <string.h>
#ifdef WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

/* ***********************************
         MACROS, DATA TYPES
   ***********************************/

// define bitboard data type
// 64-bit unsigned int
#define U64 unsigned long long

// set, get, pop macros for bitboard
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

// FEN positions to debug code
#define empty_board "8/8/8/8/8/8/8/8 b - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define repetitions "2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 "

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
    no_sq,
};

// sides to move (colors)
enum
{
    white,
    black,
    both
};

// rook or bishop
enum
{
    rook,
    bishop
};

// castling
enum
{
    wk = 1,
    wq = 2,
    bk = 4,
    bq = 8
};

// encode pieces
enum
{
    P,
    N,
    B,
    R,
    Q,
    K,
    p,
    n,
    b,
    r,
    q,
    k,
};

// CONSTANTS
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

char ascii_pieces[12] = "PNBRQKpnbrqk";
char *unicode_pieces[12] = {"♙", "♘", "♗", "♖", "♕", "♔", "♟︎", "♞", "♝", "♜", "♛", "♚"};

int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k};

// rook magic numbers
const U64 rook_magic_numbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL};

// bishop magic numbers
const U64 bishop_magic_numbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL};

// define bitboard and board state variables
U64 bitboards[12];
U64 occupancies[3];
int side = -1;
int enpassant = no_sq;
int castle;

/* ***********************************
    RANDOM NUMBER GENERATION PROCESS
   ***********************************/

// pseudorandom number state
unsigned int state = 1804289383;

// generate 32-bit psuedorandom legal nums
// necessary for magic numbers
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = state;

    // use XORSHIFT32 algo
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update random state
    state = number;

    return number;
}

// generate 64 bit pseudorandom legal nums
U64 get_random_U64_numbers()
{
    // def 4 random nums
    U64 n1, n2, n3, n4;

    // init random nums
    // slicing only first 16 bits
    n1 = (U64)(get_random_U32_number() & 0xFFFF);
    n2 = (U64)(get_random_U32_number() & 0xFFFF);
    n3 = (U64)(get_random_U32_number() & 0xFFFF);
    n4 = (U64)(get_random_U32_number() & 0xFFFF);

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

// print board
void print_board()
{
    // print offset
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop ober board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);

            // define piece variable
            int piece = -1;

            // loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                // if there is a piece on current square
                if (get_bit(bitboards[bb_piece], square)) {
                    // get piece code
                    piece = bb_piece;
                }
            }
            printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
        }
        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // print side to move
    printf("     Side:     %s\n", !side ? "white" : "black");

    // print enpassant square
    printf("     Enpassant:   %s\n", ((enpassant != no_sq) ? square_to_coordinates[enpassant] : "no"));

    // print castling rights
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) ? 'K' : '-',
                                           (castle & wq) ? 'Q' : '-',
                                           (castle & bk) ? 'k' : '-',
                                           (castle & bq) ? 'q' : '-');
}

// parse FEN string
void parse_fen(char *fen)
{
    // reset board position (bitboards)
    memset(bitboards, 0ULL, sizeof(bitboards));

    // reset occupancies (bitboards)
    memset(occupancies, 0ULL, sizeof(occupancies));

    // reset game state variables
    side = 0;
    enpassant = no_sq;
    castle = 0;

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init current square
            int square = rank * 8 + file;

            // match ascii pieces within FEN string
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                // init piece type
                int piece = char_pieces[*fen];

                // set piece on corresponding bitboard
                set_bit(bitboards[piece], square);

                // increment pointer to FEN string
                fen++;
            }

            // match empty square numbers within FEN string
            if (*fen >= '0' && *fen <= '9')
            {
                // init offset (convert char 0 to int 0)
                int offset = *fen - '0';

                // define piece variable
                int piece = -1;

                // loop over all piece bitboards
                for (int bb_piece = P; bb_piece <= k; bb_piece++)
                {
                    // if there is a piece on current square
                    if (get_bit(bitboards[bb_piece], square))
                        // get piece code
                        piece = bb_piece;
                }

                // on empty current square
                if (piece == -1)
                    // decrement file
                    file--;

                // adjust file counter
                file += offset;

                // increment pointer to FEN string
                fen++;
            }

            // match rank separator
            if (*fen == '/')
                // increment pointer to FEN string
                fen++;
        }
    }

    // got to parsing side to move (increment pointer to FEN string)
    fen++;

    // parse side to move
    (*fen == 'w') ? (side = white) : (side = black);

    // go to parsing castling rights
    fen += 2;

    // parse castling rights
    while (*fen != ' ')
    {
        switch (*fen)
        {
        case 'K':
            castle |= wk;
            break;
        case 'Q':
            castle |= wq;
            break;
        case 'k':
            castle |= bk;
            break;
        case 'q':
            castle |= bq;
            break;
        case '-':
            break;
        }

        // increment pointer to FEN string
        fen++;
    }

    // got to parsing enpassant square (increment pointer to FEN string)
    fen++;

    // parse enpassant square
    if (*fen != '-')
    {
        // parse enpassant file & rank
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');

        // init enpassant square
        enpassant = rank * 8 + file;
    }

    // no enpassant square
    else
        enpassant = no_sq;

    for (int piece = P; piece <= K; piece++)
        // populate white occupancy bitboard
        occupancies[white] |= bitboards[piece];

    for (int piece = p; piece <= k; piece++)
        // populate black occupancy bitboard
        occupancies[black] |= bitboards[piece];

    occupancies[both] |= occupancies[white];
    occupancies[both] |= occupancies[black];
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

// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

// knight attacks table [square]
U64 knight_attacks[64];

// king attacks table [square]
U64 king_attacks[64];

// bishop attacks masks
U64 bishop_masks[64];

// rook attacks masks
U64 rook_masks[64];

// bishop/rook attack table [square][occupancies]
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];

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
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    // return attack map
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

// generate rook attacks on the fly
U64 rook_attacks_on_the_fly(int square, U64 block)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // generate rook attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    // return attack map
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

// find appropriate magic number
U64 find_magic_number(int square, int relevant_bits, int bishop)
{
    // init occupancies
    U64 occupancies[4096];

    // init attack tables
    U64 attacks[4096];

    // init used attacks
    U64 used_attacks[4096];

    // init attack mask for a current piece
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // init occupancy indicies
    int occupancy_indicies = 1 << relevant_bits;

    // loop over occupancy indicies
    for (int index = 0; index < occupancy_indicies; index++)
    {
        // init occupancies
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

        // init attacks
        attacks[index] = bishop ? bishop_attacks_on_the_fly(square, occupancies[index]) : rook_attacks_on_the_fly(square, occupancies[index]);
    }

    // test magic numbers loop
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // generate magic number candidate
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6)
            continue;

        // init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index & fail flag
        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++)
        {
            // init magic index
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if (used_attacks[magic_index] == 0ULL)
                // init used attacks
                used_attacks[magic_index] = attacks[index];

            // otherwise
            else if (used_attacks[magic_index] != attacks[index])
                // magic index doesn't work
                fail = 1;
        }

        // if magic number works
        if (!fail)
            // return it
            return magic_number;
    }

    // if magic number doesn't work
    printf("  Magic number fails!\n");
    return 0ULL;
}

void init_sliders_attacks(int bishop)
{
    // init bishop & rook masks
    for (int square = 0; square < 64; square++)
    {
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        // init current mask
        U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        int relevant_bits_count = count_bits(attack_mask);

        int occupancy_indices = (1 << relevant_bits_count);

        // loop over occupancy indices
        for (int index = 0; index < occupancy_indices; index++)
        {
            if (bishop)
            {
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
            else
            {
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

// get bishop attacks
static inline U64 get_bishop_attacks(int square, U64 occupancy)
{
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];
    return bishop_attacks[square][occupancy];
}

static inline U64 get_rook_attacks(int square, U64 occupancy)
{
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];
    return rook_attacks[square][occupancy];
}

static inline U64 get_queen_attacks(int square, U64 occupancy)
{
    return (get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy));
}

// returns 1 if square is being attacked by side, 0 if otherwise
static inline int is_square_attacked(int square, int side)
{

    // attacked by white/black pawn
    if ((side == white) && (pawn_attacks[black][square] & bitboards[P]))
        return 1;
    if ((side == black) && (pawn_attacks[white][square] & bitboards[p]))
        return 1;

    // attacked by white/black knight
    if (knight_attacks[square] & ((side == white) ? bitboards[N] : bitboards[n]))
        return 1;

    // attacked by white/black bishop
    if (get_bishop_attacks(square, occupancies[both]) & ((side == white) ? bitboards[B] : bitboards[b]))
        return 1;

    // attacked by white/black rook
    if (get_rook_attacks(square, occupancies[both]) & ((side == white) ? bitboards[R] : bitboards[r]))
        return 1;

    // attacked by white/black rook
    if (get_queen_attacks(square, occupancies[both]) & ((side == white) ? bitboards[Q] : bitboards[q]))
        return 1;

    // attacked by white/black king
    if (king_attacks[square] & ((side == white) ? bitboards[K] : bitboards[k]))
        return 1;

    // if none ever returned 1, must not be attacked
    return 0;
}

void print_attacked_squares(int side)
{
    // print offset
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);

            printf(" %d", (is_square_attacked(square, side)) ? 1 : 0);
        }
        // print new line every rank
        printf("\n");
    }
    // print board files
    printf("\n     a b c d e f g h\n\n");
}

/* ***********************************
   ***********************************
     MOVE ENCODING + PRINTING MOVES
   ***********************************
   ***********************************/

/*
    Things to be represented in binary formatting of a move (bit ranges shown in 1s)
    0000 0000 0000 0000 0011 1111    source square (6)     --> 0x3F
    0000 0000 0000 1111 1100 0000    target square (6)     --> 0xFC0
    0000 0000 1111 0000 0000 0000    piece (4)             --> 0xF000
    0000 1111 0000 0000 0000 0000    promoted piece (4)    --> 0xF0000
    0001 0000 0000 0000 0000 0000    capture flag (1)      --> 0x100000
    0010 0000 0000 0000 0000 0000    double push flag (1)  --> 0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag (1)    --> 0x400000
    1000 0000 0000 0000 0000 0000    castling flag (1)     --> 0x800000
*/

// encode move
#define encode_move(source, target, piece, promoted, capture, double_push, enpassant, castling) \
    (source) |               \
    (target << 6) |          \
    (piece << 12) |          \
    (promoted << 16) |       \
    (capture << 20) |        \
    (double_push << 21) |    \
    (enpassant << 22) |      \
    (castling << 23)         \

// extract values from move
#define get_move_source(move) (move & 0x3F)
#define get_move_target(move) ((move & 0xFC0) >> 6)
#define get_move_piece(move) ((move & 0xF000) >> 12)
#define get_move_promoted(move) ((move & 0xF0000) >> 16)
#define get_move_capture(move) ((move & 0x100000) >> 20)
#define get_move_double_push(move) ((move & 0x200000) >> 21) 
#define get_move_enpassant(move) ((move & 0x400000) >> 22)
#define get_move_castling(move) ((move & 0x800000) >> 23)

// move list structure
typedef struct {
    int moves[256];
    int count;
} moves;

// add move to move list
static inline void add_move(moves *move_list, int move) {
    move_list->moves[move_list->count] = move;

    // increment move count
    move_list->count++;
}

// promoted pieces
char promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

// print move (for UCI protocol purposes)
void print_move(int move) {
    printf("%s%s%c\n", square_to_coordinates[get_move_source(move)],
                     square_to_coordinates[get_move_target(move)], 
                     promoted_pieces[get_move_promoted(move)]);
}

// print move list (for debugging purposes)
void print_move_list(moves *move_list) {

    // do nothing if move list is empty
    if (!move_list->count) {
        printf("     No move in the list!\n");
        return;
    }

    printf("\nmove    piece  capture  double_push  enpassant  castling\n\n");
    // loop over moves in move list
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        int move = move_list->moves[move_count];
        printf("%s%s%c      %c       %d          %d           %d          %d\n", square_to_coordinates[get_move_source(move)],
                           square_to_coordinates[get_move_target(move)], 
                           get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
                           ascii_pieces[get_move_piece(move)],
                           get_move_capture(move),
                           get_move_double_push(move),
                           get_move_enpassant(move),
                           get_move_castling(move));
    }
    printf("\n\nTotal number of moves: %d\n", move_list->count);
}

// define copy_board macro
#define copy_board() \
    U64 bitboards_copy[12], occupancies_copy[3]; \
    int side_copy, enpassant_copy, castle_copy; \
    memcpy(bitboards_copy, bitboards, 96); \
    memcpy(occupancies_copy, occupancies, 24); \
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle; \

// define restore_board macro
#define take_back() \
    memcpy(bitboards, bitboards_copy, 96); \
    memcpy(occupancies, occupancies_copy, 24); \
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy; \

// move types
enum { all_moves, only_captures };

// constant for updating castling rights
// move on square represents change of castling rights
const int castling_rights[64] = {
    7, 15, 15, 15,  3, 15, 15, 11,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   13, 15, 15, 15, 12, 15, 15, 14
};

// make move on corresponding bitboards
static inline int make_move(int move, int move_flag) {
    // quiet moves
    if (move_flag == all_moves) {
        // preserve board state in case move is illegal
        copy_board();

        // parse the move
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double_push(move);
        int enpass = get_move_enpassant(move);
        int castling = get_move_castling(move);

        // move piece
        pop_bit(bitboards[piece], source_square);
        set_bit(bitboards[piece], target_square);

        // handling captures
        if (capture) {
            // pick up bb piece index ranges
            int start_piece, end_piece;
            // white to move
            if (side == white) {
                start_piece = p;
                end_piece = k;
            }
            // black to move
            else {
                start_piece = P;
                end_piece = K;
            }
            // loop over bitboards OPPOSITE side to move
            for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) {
                if (get_bit(bitboards[bb_piece], target_square)) {
                    pop_bit(bitboards[bb_piece], target_square);
                    break;
                }
            }
        }

        // handle pawn promotions
        if (promoted_piece) {
            // erase pawn from target_square
            pop_bit(bitboards[(side == white) ? white : black], target_square);

            // set up promoted piece on board
            set_bit(bitboards[promoted_piece], target_square);
        }

        if (enpass) {
            // erase the pawn based on side to move
            (side == white) ? pop_bit(bitboards[p], target_square + 8) : pop_bit(bitboards[P], target_square - 8);
        }

        // reset enpassant square
        enpassant = no_sq;

        // handle double push
        if (double_push) {
            // set enpassant square depending on side to move
            (side == white) ? (enpassant = target_square + 8) : (enpassant = target_square - 8);
        }

        // handle castling
        if (castling) {
            switch (target_square) {
                // white kingside
                case (g1):
                    pop_bit(bitboards[R], h1);
                    set_bit(bitboards[R], f1);
                    break;
                // white queenside
                case (c1):
                    pop_bit(bitboards[R], a1);
                    set_bit(bitboards[R], d1);
                    break;
                // black kingside
                case (g8):
                    pop_bit(bitboards[r], h8);
                    set_bit(bitboards[r], f8);
                    break;
                // black queenside
                case (c8):
                    pop_bit(bitboards[r], a8);
                    set_bit(bitboards[r], d8);
                    break;
            }
        }
        
        // update castling rights
        castle &= castling_rights[source_square];
        castle &= castling_rights[target_square];

        // reset occupancies
        memset(occupancies, 0ULL, 24);
        // loop over white pieces, update bitboards
        for (int bb_piece = P; bb_piece <= K; bb_piece++) {
            occupancies[white] |= bitboards[bb_piece];
        }
        // loop over black piece, update bitboards
        for (int bb_piece = p; bb_piece <= k; bb_piece++) {
            occupancies[black] |= bitboards[bb_piece];
        }
        // update both
        occupancies[both] |= occupancies[white];
        occupancies[both] |= occupancies[black];

        // change side
        side ^= 1;
        // make sure king has not been exposed to check (illegal)
        if (is_square_attacked((side == white) ? get_ls1b_index(bitboards[k]) : get_ls1b_index(bitboards[K]), side))
        {
            // take move back
            take_back();
            // return illegal move
            return 0;
        }
        else {
            // if not illegal, everything good
            return 1;
        }
    }

    // captures
    else {
        // make sure move is a capture
        if (get_move_capture(move)) {
            make_move(move, all_moves);
        }
        else {
            // don't capture
            return 0;
        }
    }
}

/* ***********************************
   ***********************************
            MOVE GENERATOR
   ***********************************
   ***********************************/

// generate all moves
static inline void generate_moves(moves *move_list)
{
    move_list->count = 0;

    // define source/target squares
    int source_square, target_square;

    // define current pieces bitboard copy (can't change original board)
    U64 bitboard, attacks;

    // loop over all bitboards
    for (int piece = P; piece <= k; piece++)
    {
        bitboard = bitboards[piece];

        // generate white pawns and white king castling moves
        if (side == white)
        {

            if (piece == P)
            {
                // loop over white pawns within bitboard
                while (bitboard)
                {
                    source_square = get_ls1b_index(bitboard);
                    target_square = source_square - 8;

                    // generate quiet pawn moves
                    // make sure target square on board and target_square isn't occupied by either white or black
                    if (!(target_square < a8) && !get_bit(occupancies[both], target_square))
                    {
                        // pawn promotion
                        if (source_square >= a7 && source_square <= h7)
                        {
                            // print moves
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }
                        else
                        {

                            // one square ahead
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            // two squares ahead (starting move)
                            // make sure double push target isn't occupied and currently on starting rank

                            if ((source_square >= a2 && source_square <= h2) && (!get_bit(occupancies[both], target_square - 8)))
                            {
                                add_move(move_list, encode_move(source_square, target_square-8, piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }

                    // init pawn attacks on black pieces
                    attacks = pawn_attacks[side][source_square] & occupancies[black];

                    // generate pawn captures
                    while (attacks)
                    {
                        // init target square
                        target_square = get_ls1b_index(attacks);

                        // pawn capture promotion
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                        }
                        else
                        {
                            // one square ahead capture
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }

                        pop_bit(attacks, target_square);
                    }

                    // generate enpassant captures
                    if (enpassant != no_sq)
                    {
                        // lookup pawn attacks and bitwise AND with enpassant bit
                        U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);
                        if (enpassant_attacks)
                        {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            pop_bit(enpassant_attacks, target_enpassant);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    pop_bit(bitboard, source_square);
                }
            }

            // handle castling moves
            if (piece == K)
            {
                // king side available
                if (castle & wk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1))
                    {
                        if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
                        {
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
                // queen side available
                if (castle & wq)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1) && !get_bit(occupancies[both], b1))
                    {
                        if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                        {
                            add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        }

        // generate black pawns and black king castling moves
        else
        {
            if (piece == p)
            {
                // loop over black pawns within bitboard
                while (bitboard)
                {
                    source_square = get_ls1b_index(bitboard);
                    target_square = source_square + 8;

                    // generate quiet pawn moves
                    // make sure target square on board and target_square isn't occupied by either white or black
                    if (!(target_square > h1) && !get_bit(occupancies[both], target_square))
                    {
                        // pawn promotion
                        if (source_square >= a2 && source_square <= h2)
                        {
                            // print moves
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                        }
                        else
                        {

                            // one square ahead
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            // two squares ahead (starting move)
                            // make sure double push target isn't occupied and currently on starting rank
                            if ((source_square >= a7 && source_square <= h7) && (!get_bit(occupancies[both], target_square + 8)))
                            {
                                add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }

                    // init pawn attacks on white pieces
                    attacks = pawn_attacks[side][source_square] & occupancies[white];

                    // generate pawn captures
                    while (attacks)
                    {
                        // init target square
                        target_square = get_ls1b_index(attacks);

                        // pawn capture promotion
                        if (source_square >= a2 && source_square <= h2)
                        {
                            // print moves
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                        }
                        else
                        {
                            // one square ahead capture
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        pop_bit(attacks, target_square);
                    }

                    // generate enpassant captures
                    if (enpassant != no_sq)
                    {
                        // lookup pawn attacks and bitwise AND with enpassant bit
                        U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);
                        if (enpassant_attacks)
                        {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            pop_bit(enpassant_attacks, target_enpassant);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    pop_bit(bitboard, source_square);
                }
            }

            // handle castling moves
            if (piece == k)
            {
                // king side available
                if (castle & bk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8))
                    {
                        if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                        {
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
                // queen side available
                if (castle & bq)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8) && !get_bit(occupancies[both], b8))
                    {
                        if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                        {
                            add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        }

        // generate knight moves
        if ((side == white) ? piece == N : piece == n)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);

                // init knight attacks
                // attacks are knight attacks where NOT attacking your own piece
                attacks = knight_attacks[source_square] & ((side == white) ? (~occupancies[white]) : (~occupancies[black]));

                // loop over target squares available from attacks
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    pop_bit(attacks, target_square);
                }

                pop_bit(bitboard, source_square);
            }
        }

        // generate bishop moves
        if ((side == white) ? piece == B : piece == b)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);

                // init bishop attacks
                // attacks are bishop attacks not attacking your own piece or seeing through pieces
                attacks = get_bishop_attacks(source_square, occupancies[both]) & ((side == white) ? (~occupancies[white]) : (~occupancies[black]));

                // loop over target squares available from attacks
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    pop_bit(attacks, target_square);
                }

                pop_bit(bitboard, source_square);
            }
        }

        // generate rook moves
        if ((side == white) ? piece == R : piece == r)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);

                // init rook attacks
                attacks = get_rook_attacks(source_square, occupancies[both]) & ((side == white) ? (~occupancies[white]) : (~occupancies[black]));

                // loop over target squares available from attacks
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    pop_bit(attacks, target_square);
                }

                pop_bit(bitboard, source_square);
            }
        }

        // generate queen moves
        if ((side == white) ? piece == Q : piece == q)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);

                // init queen attacks
                attacks = get_queen_attacks(source_square, occupancies[both]) & ((side == white) ? (~occupancies[white]) : (~occupancies[black]));

                // loop over target squares available from attacks
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    pop_bit(attacks, target_square);
                }

                pop_bit(bitboard, source_square);
            }
        }

        // generate king non-castling moves
        if ((side == white) ? piece == K : piece == k)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);

                // init knight attacks
                // attacks are knight attacks where NOT attacking your own piece
                attacks = king_attacks[source_square] & ((side == white) ? (~occupancies[white]) : (~occupancies[black]));

                // loop over target squares available from attacks
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    pop_bit(attacks, target_square);
                }
                pop_bit(bitboard, source_square);
            }
        }
    }
}

void init_all()
{
    // init leaper pieces attacks
    init_leapers_attacks();

    // init slider pieces attacks
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);
}

/* ***********************************
   ***********************************
              MAIN DRIVER
   ***********************************
   ***********************************/

int get_time_ms() {
    #ifdef WIN64
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return (time_value.tv_sec * 1000 + time_value.tv_usec / 1000);
    #endif
}

// leaf nodes (number of positions reached during test of move gen at given depth)
long nodes;

// perft driver at depth num of moves
static inline void perft_driver(int depth) {
    // base case
    if (depth == 0) {
        // increment count of reached positions
        nodes++;
        return;
    }

    // define new move_list and gen moves for current depth
    moves move_list[1];
    generate_moves(move_list);

    // loop over generated moves
    for (int i = 0; i < move_list->count; i++) {
        copy_board();
        // if move not valid, skip to next
        if (!make_move(move_list->moves[i], all_moves)) {
            continue;
        }
        // call perft driver recursively on lower depth
        perft_driver(depth - 1);
        take_back();
    }
}


int main(void)
{
    init_all();
    parse_fen(start_position);
    print_board();

    int start = get_time_ms();

    perft_driver(6);

    // time taken to exec program
    printf("Time taken to execute: %d ms\n", get_time_ms() - start);
    printf("Nodes found: %ld\n", nodes);
    return 0;
}