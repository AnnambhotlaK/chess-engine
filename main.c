/* ***********************************
   ***********************************
        BITBOARD CHESS ENGINE IN C
   ***********************************
   ***********************************/

#include <stdio.h>
#include <string.h>

// include important constants and enums
#include "enums/squares.h"
#include "enums/colors.h"
#include "enums/rook_or_bishop.h"
#include "constants.h"
#include "macros.h"
#include "bitboards.h"
#include "enums/castling.h"
#include "enums/pieces.h"

// include important functions and processes
#include "bitboard_functions/mask_attacks.c"
#include "pieces/init_leapers.c"
#include "pieces/slider_attacks.c"
#include "magic_numbers/magic_nums.c"

void init_all() {
    init_leapers_attacks();
}

int main(void)
{
    init_all();

    // set white pieces
    set_bit(bitboards[P], a2);
    set_bit(bitboards[P], b2);
    set_bit(bitboards[P], c2);
    set_bit(bitboards[P], d2);
    set_bit(bitboards[P], e2);
    set_bit(bitboards[P], f2);
    set_bit(bitboards[P], g2);
    set_bit(bitboards[P], h2);
    set_bit(bitboards[R], a1);
    set_bit(bitboards[R], h1);
    set_bit(bitboards[N], b1);
    set_bit(bitboards[N], g1);
    set_bit(bitboards[B], f1);
    set_bit(bitboards[B], c1);
    set_bit(bitboards[Q], d1);
    set_bit(bitboards[K], e1);

    set_bit(bitboards[p], a7);
    set_bit(bitboards[p], b7);
    set_bit(bitboards[p], c7);
    set_bit(bitboards[p], d7);
    set_bit(bitboards[p], e7);
    set_bit(bitboards[p], f7);
    set_bit(bitboards[p], g7);
    set_bit(bitboards[p], h7);
    set_bit(bitboards[r], a8);
    set_bit(bitboards[r], h8);
    set_bit(bitboards[n], b8);
    set_bit(bitboards[n], g8);
    set_bit(bitboards[b], f8);
    set_bit(bitboards[b], c8);
    set_bit(bitboards[q], d8);
    set_bit(bitboards[k], e8);

    side = black;
    enpassant = e3;
    castle |= wk;
    castle |= wq;
    castle |= bk;
    castle |= bq;

    // print chess board
    print_board();

    for (int i = P; i <= k; i++) {
        print_bitboard(bitboards[i]);
    }

    getchar();

    return 0;
}