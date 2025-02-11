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

    // set white pawn on e2
    set_bit(bitboards[P], e2);

    print_bitboard(bitboards[P]);

    // print piece (ascii on windows, unicode otherwise)
    #ifdef WIN64
        printf("piece: %c\n", ascii_pieces[char_pieces['P']]);
    #else
        printf("piece: %c\n", unicode_pieces[char_pieces['P']]);
    #endif

    return 0;
}