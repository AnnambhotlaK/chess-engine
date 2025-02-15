#include "bitboard_funcs.h"
#include <string.h>

// count bits in bitboard
inline int count_bits(U64 bitboard)
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
inline int get_ls1b_index(U64 bitboard)
{
    // make sure bitboard != 0
    if (bitboard)
    {
        return count_bits(((bitboard & -bitboard) - 1));
    }
    // otherwise return illegal index
    return -1;
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

// prints actual board (not bitboard)
void print_board()
{
    printf("\n");
    // loop over ranks and files
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            // print rank
            if (!file)
            {
                printf("  %d ", 8 - rank);
            }

            int piece = -1;
            // loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                // determine which piece we have on the given square
                if (get_bit(bitboards[bb_piece], square))
                {
                    piece = bb_piece;
                    break;
                }
            }
#ifdef WIN64
            printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
#else
            printf(" %s", (piece == -1) ? '.' : unicode_pieces[piece]);
#endif
        }
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // also print game state variables
    printf("     Side:       %s\n", ((!side && side != -1) ? "white" : "black"));
    printf("     En Passant: %s\n", (enpassant != no_sq) ? SQUARE_TO_COORDINATES[enpassant] : "none");
    printf("     Castling:   %c %c %c %c\n\n", ((castle & wk) ? 'K' : '-'),
           ((castle & wq) ? 'Q' : '-'),
           ((castle & bk) ? 'k' : '-'),
           ((castle & bq) ? 'q' : '-'));
}

void parse_fen(char *fen)
{

    // reset board position and gamestate vars
    memset(bitboards, 0ULL, sizeof(bitboards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = 0;
    castle = 0;
    enpassant = no_sq;

    // loop over board ranks and files
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            // match ascii pieces in the fen
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                int piece = char_pieces[*fen];

                // set piece on corresponding bitboard
                set_bit(bitboards[piece], square);

                // move to next character in fen
                *fen++;
            }

            // match empty square numbers within fen
            if (*fen >= '0' && *fen <= '9') 
            {
                // offset allows us to skip empty squares
                int offset = *fen - '0';

                int piece = -1;
                for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                    if (get_bit(bitboards[bb_piece], square)) {
                        piece = bb_piece;
                    }
                }

                // if no piece on current square
                // must decrement to avoid skipping square
                if (piece == -1) {
                    file--;
                }

                // adjust file counter
                file += offset;

                *fen++;
            }

            // match rank separator
            if (*fen == '/') {
                // just skip
                *fen++;
            }
        }
    }
    *fen++; // skip to color
    (*fen == 'w') ? (side = white) : (side = black);
    fen += 2; // skip to castling
    while (*fen != ' ') {
        switch(*fen)
        {
            case 'K': castle |= wk; break;
            case 'Q': castle |= wq; break;
            case 'k': castle |= bk; break;
            case 'q': castle |= bq; break;
            case '-': break;
        }
        *fen++;
    }
    *fen++; // skip to enpassant square
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = (rank * 8 + file);
    }
    else {
        enpassant = no_sq;
    }

    // init occupancies bitboards
    for (int piece = P; piece <= K; piece++) {
        occupancies[white] |= bitboards[piece];
    }
    for (int piece = p; piece <= k; piece++) {
        occupancies[black] |= bitboards[piece];
    }
    occupancies[both] = occupancies[white] | occupancies[black];
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