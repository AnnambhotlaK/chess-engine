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
    //init_all();
    parse_fen("r2q1bnr/ppp2ppp/2n5/4p3/2PkN3/3PQP1B/PP2KP1P/R1B4R b - - 2 13");
    print_board();
    return 0;
}